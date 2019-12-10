import * as xmlParser from "fast-xml-parser";
import * as iconv from "iconv-lite";
import * as request from "request-promise-native";
import { Database } from "../database";
import { NewsModel } from "./news_model";

enum UpdateInterval {
    MINS  = "mins",
    HOURS = "hours"
}

export class FeedModel {
    private id: number;
    private url: string;
    private title: string;
    private updated: number;
    private updateInterval: number;
    private updateIntervalType: UpdateInterval;
    private isUpdatingFlag: boolean;

    // =================================================================================================================

    constructor(info: any) {
        this.id                 = parseInt(info.id, 10);
        this.url                = info.url;
        this.title              = info.title;
        this.updated            = Date.parse(info.updated);
        this.updateInterval     = parseInt(info.update_interval, 10);
        this.updateIntervalType = info.update_interval_type;
        this.isUpdatingFlag     = false;
    }

    // =================================================================================================================let

    public getId() {
        return this.id;
    }

    // =================================================================================================================let

    public getUrl() {
        return this.url;
    }

    // =================================================================================================================let

    public getTitle() {
        return this.title;
    }

    // =================================================================================================================

    public isNeedForUpdate(): boolean {
        let updateInterval = this.updateInterval * 1000;

        switch (this.updateIntervalType) {
            case UpdateInterval.MINS:
                updateInterval *= 60;
                break;

             case UpdateInterval.HOURS:
                updateInterval *= 60 * 60;
                break;
        }

        if (this.updated + updateInterval < Date.now()) {
            return true;
        }

        return false;
    }

    // =================================================================================================================

    public isUpdating() {
        return this.isUpdatingFlag;
    }

    // =================================================================================================================

    public async update() {
        this.isUpdatingFlag = true;

        await this.setUpdating();

        let isError = false;
        let response;

        try {
            response = await request.get({
                uri:      this.url,
                encoding: null
            });
        }
        catch (error) {
            this.setError(error);

            isError = true;
        }

        if (!isError) {
            await this.parse(response);

            await this.setUpdated();
        }

        this.isUpdatingFlag = false;
    }

    // =================================================================================================================

    private async parse(body: string) {
        body = this.encodeToUtf8(body);

        let xml = xmlParser.parse(body, {
            attributeNamePrefix: "@",
            ignoreAttributes:    false
        });

        if (xml.rss !== undefined) { // Feed has RSS format
            xml = xml.rss && xml.rss.channel || {};

            // this.link        = json.link;
            // this.title       = json.title;
            // this.description = json.description;

            // Parse news items
            if (!Array.isArray(xml.item)) {
                return;
            }

            const xmlItems = xml.item || xml["rss:item"];
            const newsItems: NewsModel[] = [];

            for (const item of xmlItems) {
                const newsItem = new NewsModel(this.id, this.url);

                newsItem.parseRss(item);
                newsItems.push(newsItem);
            }

            const similar = await this.getSimilarItemsFromDb(newsItems);

            const db = await Database.getInstance();
            const conn = await db.getConnection();

            conn.query("LOCK TABLE news WRITE");

            for (const item of newsItems) {
                const isDuplicated = item.isDuplicate(similar.guids, similar.updated, similar.titles, similar.links);

                if (!isDuplicated) {
                    console.log("  *", item.title.padEnd(90), item.updated);

                    await item.save(conn);
                }
            }

            conn.query("UNLOCK TABLES");
            conn.release();
        }
    }

    // =================================================================================================================

    private async getSimilarItemsFromDb(newsItems: NewsModel[]) {
        const db = await Database.getInstance();

        const guidsArr: string[] = [];
        const linksArr: string[] = [];
        const updatedArr: string[] = [];

        for (const newsItem of newsItems) {
            if (newsItem.guid !== "") {
                guidsArr.push(newsItem.guid);
            }

            if (newsItem.link !== "") {
                linksArr.push(newsItem.link);
            }

            if (newsItem.updated !== "") {
                updatedArr.push(newsItem.updated);
            }
        }

        const guidsStr   = "\"" + guidsArr.join("\",\"") + "\"";
        const linksStr   = "\"" + linksArr.join("\",\"") + "\"";
        const updatedStr = "\"" + updatedArr.join("\",\"") + "\"";

        const query = `
            SELECT guid, link, published, title
            FROM news
            WHERE
               feed_id = ? AND
               (
                   guid IN (${guidsStr}) OR
                   link IN (${linksStr}) OR
                   published IN (${updatedStr})
               )
        `;

        const results = await db.execute(query, [
            this.id
        ]);

        const guidsList: string[] = [];
        const linksList: string[] = [];
        const titlesList: string[] = [];
        const updatedList: string[] = [];

        for (const item of results) {
            guidsList.push(item.guid);
            linksList.push(item.link);
            titlesList.push(item.title);
            updatedList.push(item.published);
        }

        return {
            guids:   guidsList,
            links:   linksList,
            titles:  titlesList,
            updated: updatedList
        };
    }

    // =================================================================================================================

    private encodeToUtf8(body: string) {
        const string = body.toString();
        const match = string.match(/encoding=\"([^\"]+)/);

        let result = "";

        if (match) {
            const codepage = match[1];

            if (iconv.encodingExists(codepage)) {
                result = iconv
                    .decode(Buffer.from(body), match[1])
                    .toString();
            }
        }
        else {
            result = string;
        }

        return result;
    }

    // =================================================================================================================

    private async setUpdating() {
        const db = await Database.getInstance();

        const query = `
            UPDATE feeds
            SET
                updated = NOW(),
                status = 'updating'
            WHERE id = ${this.getId()}
            LIMIT 1
        `;

        db.execute(query);
    }

    // =================================================================================================================

    private async setUpdated() {
        const db = await Database.getInstance();

        const query = `
            UPDATE feeds
            SET
                updated = NOW(),
                status = 'idle'
            WHERE id = ${this.getId()}
            LIMIT 1
        `;

        db.execute(query);
    }

    // =================================================================================================================

    private async setError(error: any) {
        const db = await Database.getInstance();

        const text: string[] = [];

        if (error.response) {
            if (error.response.statusCode) {
                text.push(error.response.statusCode);
            }

            if (error.response.statusMessage) {
                text.push(error.response.statusMessage);
            }
        }

        if (error.message) {
            text.push(error.message);
        }

        const result = text.join(" ");

        console.log(`Error fetching of ${this.title}! ${result}`);

        const query = `
            UPDATE feeds
            SET
                updated = NOW(),
                status = 'error',
                statusMessage = '${result}'
            WHERE id = ${this.getId()}
            LIMIT 1
        `;

        db.execute(query);
    }
}

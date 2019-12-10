import * as moment from "moment";
import * as mysql from "mysql2/promise";
import * as Url from "url";
import { htmlspecialchars_decode } from "../utils";

export class NewsModel {
    public feedId = 0;
    public feedUrl = "";
    public guid = "";
    public title = "";
    public updated = "";
    public author = "";
    public link = "";
    public description = "";
    public content = "";
    public category = "";
    public comments = "";
    public enclosureUrl = "";
    public enclosureType = "";
    public enclosureLength = 0;

    // =================================================================================================================

    constructor(feedId: number, feedUrl: string) {
        this.feedId  = feedId;
        this.feedUrl = feedUrl;
    }

    // =================================================================================================================

    public parseRss(item: any) {
        let guid = "";
        let isPermalink = false;

        if (typeof item.guid === "object") {
            guid        = item.guid["#text"] || "";
            isPermalink = item.guid["@isPermaLink"] === "true" || item.guid["@isPermalink"] === "true" || false;
        }
        else if (typeof item.guid === "string") {
            guid = item.guid || "";
        }

        this.guid  = guid;
        this.title = item.title;

        if (!this.title) {
            this.title = item["rss:title"];
        }

        // If title is incorrect (has tags)
        if (typeof this.title === "object") {
            const title: string[] = [];

            for (const tag in this.title as any) {
                if ((this.title as any).hasOwnProperty(tag)) {
                    title.push(this.title[tag]);
                }
            }

            this.title = title.join(" ");
        }

        let updated = item.pubDate;

        if (updated === "") {
            updated = item.pubdate;
        }

        if (updated === "") {
            updated = item["dc:date"];
        }

        updated = updated || "";

        this.updated = this.parseDate(updated);

        this.author  = item.author;

        if (this.author === "") {
            this.author = item["dc:creator"];
        }

        this.link = item.link;

        if (this.link === "") {
            this.link = item["rss:link"];

            if (this.link === "") {
                if (guid && isPermalink === true) {
                    this.link = guid;
                }
            }
        }

        if (!this.link) {
            this.link = "";
        }

        const url = Url.parse(this.link);

        if (url.host === "") {
            url.host = Url.parse(this.feedUrl).host;
        }

        if (url.protocol === "") {
            url.protocol = Url.parse(this.feedUrl).protocol;
        }

        this.description = item.description || "";
        this.content     = item.content || "";

        if (this.content === "" || this.description.length > this.content.length) {
            this.content = "";
        }
        else {
            this.description = this.content;
        }

        if (item.category) {
            if (typeof item.category === "string") {
                this.category = item.category;
            }
            else if (Array.isArray(item.category)) {
                this.category = item.category.join(", ");
            }
        }

        this.comments = item.comments;

        if (typeof item.enclosure === "object") {
            this.enclosureUrl    = item.enclosure["@url"]    || "";
            this.enclosureType   = item.enclosure["@type"]   || "";
            this.enclosureLength = parseInt(item.enclosure["@length"], 10) || 0;
        }

        if (this.title === "") {
            this.title = this.description; // toPlainText

            if (this.title.length > 50) {
                this.title = this.title.slice(0, 50);
                this.title += "...";
            }
        }

        this.title       = htmlspecialchars_decode(this.title       || "");
        this.author      = htmlspecialchars_decode(this.author      || "");
        this.link        = htmlspecialchars_decode(this.link        || "");
        this.description = htmlspecialchars_decode(this.description || "");
        this.content     = htmlspecialchars_decode(this.content     || "");
        this.category    = htmlspecialchars_decode(this.category    || "");
        this.comments    = htmlspecialchars_decode(this.comments    || "");

        this.title       = this.title.trim();
        this.author      = this.author.trim();
        this.link        = this.link.trim();
        this.description = this.description.trim();
        this.content     = this.content.trim();
        this.category    = this.category.trim();
        this.comments    = this.comments.trim();
    }

    // =================================================================================================================

    public isDuplicate(guidsList: string[], publishedList: string[], titlesList: string[], linksList: string[]) {
        let isDuplicate = false;
        const isRemoveDuplicates = false;

        for (let i = 0; i < guidsList.length; i++) {
            if (this.guid !== "") {         // search by guid if present
                if (guidsList[i] === this.guid) {
                    if (this.updated !== "") {  // search by pubDate if present
                        if (!isRemoveDuplicates) {
                            if (publishedList[i] === this.updated) {
                                isDuplicate = true;
                            }
                        }
                        else {
                            isDuplicate = true;
                        }
                    }
                    else {                            // ... or by title
                        if (this.title !== "" && (titlesList[i] === this.title)) {
                            isDuplicate = true;
                        }
                    }
                }

                if (!isDuplicate) {
                    if (this.updated !== "") {
                        if ((publishedList[i] === this.updated) && (titlesList[i] === this.title)) {
                            isDuplicate = true;
                        }
                    }
                }
            }
            else if (this.link !== "") {   // search by link_href
                if (linksList[i] === this.link) {
                    if (this.updated !== "") {  // search by pubDate if present
                        if (!isRemoveDuplicates) {
                            if (publishedList[i] === this.updated) {
                                isDuplicate = true;
                            }
                        }
                        else {
                            isDuplicate = true;
                        }
                    }
                    else {                            // ... or by title
                        if (this.title !== "" && (titlesList[i] === this.title)) {
                            isDuplicate = true;
                        }
                    }
                }

                if (!isDuplicate) {
                    if (this.updated !== "") {
                        if ((publishedList[i] === this.updated) && (titlesList[i] === this.title)) {

                            isDuplicate = true;
                        }
                    }
                }
            }
            else {                                // guid is absent
                if (this.updated !== "") {  // search by pubDate if present
                    if (!isRemoveDuplicates) {
                        if (publishedList[i] === this.updated) {
                            isDuplicate = true;
                        }
                    }
                    else {
                        isDuplicate = true;
                    }
                }
                else {                            // ... or by title
                    if (this.title !== "" && (titlesList[i] === this.title)) {
                        isDuplicate = true;
                    }
                }

                if (!isDuplicate) {
                    if (this.updated !== "") {
                        if ((publishedList[i] === this.updated) && (titlesList[i] === this.title)) {
                            isDuplicate = true;
                        }
                    }
                }
            }

            if (isDuplicate) {
                break;
            }
        }

        return isDuplicate;
    }

    // =================================================================================================================

    private parseDate(dateString: string) {
        let temp: string;
        let timeZone: string;

        if (dateString === "") {
            return "";
        }

        const nTimeShift = (-1) * new Date().getTimezoneOffset() / 60;

        let ds = dateString.trim();

        if (ds.indexOf(",") !== -1) {
            ds = ds.slice(ds.indexOf(",") + 1);
            ds = ds.trim();
        }

        let dt: moment.Moment;

        for (let i = 0; i < 2; i++) {
            temp     = ds.substr(0, 23);
            timeZone = ds.substr(temp.length, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "YYYY-MM-DDTHH:mm:ss.S");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 19);
            timeZone = ds.substr(temp.length, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "YYYY-MM-DDTHH:mm:ss");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 23);
            timeZone = ds.substr(temp.length + 1, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "YYYY-MM-dd HH:mm:ss.S");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 19);
            timeZone = ds.substr(temp.length + 1, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "YYYY-MM-dd HH:mm:ss");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 20);
            timeZone = ds.substr(temp.length + 1, 3);

            if (timeZone.indexOf("EDT") !== -1) {
                timeZone = "-4";
            }

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "DD MMM YYYY HH:mm:ss");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 19);
            timeZone = ds.substr(temp.length + 1, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "D MMM YYYY HH:mm:ss");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 11);
            timeZone = ds.substr(temp.length + 1, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "DD MMM YYYY");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 10);
            timeZone = ds.substr(temp.length + 1, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "D MMM YYYY");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            temp     = ds.substr(0, 10);
            timeZone = ds.substr(temp.length, 3);

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "YYYY-MM-DD");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }

            // @HACK(arhohryakov:2012.01.01):
            // "DD MMM YY HH:mm:ss" format doesn/t parse automatically
            // Reformat it to "DD MMM YYYY HH:mm:ss"

            let temp2: string;
            temp2 = ds;  // save ds for output in case of error

            if (70 < parseInt(ds.substr(7, 2), 10)) {
                temp2 = temp2.substr(0, 7) + "19" + temp2.substr(7);
            }
            else {
                temp2 = temp2.substr(0, 7) + "20" + temp2.substr(7);
            }

            temp = temp2.substr(0, 20);
            timeZone = ds.substr(temp.length + 1 - 2, 3);  // "-2", cause 2 symbols inserted

            if (timeZone === "") {
                timeZone = nTimeShift.toString();
            }

            dt = moment(temp, "DD MMM YYYY HH:mm:ss");

            if (dt.isValid()) {
                return moment(dt)
                        .add(parseInt(timeZone, 10) * -3600, "s")
                        .format("YYYY-MM-DD HH:mm:ss");
            }
        }

        return "";
    }

    // =================================================================================================================

    public async save(conn: mysql.PoolConnection) {
        // If duplicates not found, add news into base
        let isRead = false;

        let query;
        let result;

        const markIdenticalNewsRead = false;

        if (markIdenticalNewsRead) {
            query = `
                SELECT id FROM news WHERE feed_id != ? AND title = ?
            `;

            result = await conn.execute(query, [
                this.feedId,
                this.title
            ]);

            if ((result as mysql.RowDataPacket[]).length) {
                isRead = true;
            }
        }

        query = `
            INSERT INTO news
            SET
                feed_id = ?,
                description = ?,
                content = ?,
                guid = ?,
                title = ?,
                author_name = ?,
                published = ?,
                received = ?,
                link = ?,
                category = ?,
                comments = ?,
                enclosure_url = ?,
                enclosure_type = ?,
                enclosure_length = ?,
                is_new = ?,
                is_read = ?
            `;

        let updated = this.updated;

        if (updated === "") {
            updated = moment()
                        .utc()
                        .format("YYYY-MM-DD HH:mm:ss");
        }

        conn.execute(query, [
            this.feedId,
            this.description,
            this.content,
            this.guid,
            this.title,
            this.author,
            updated,
            updated,
            this.link,
            this.category,
            this.comments,
            this.enclosureUrl,
            this.enclosureType,
            this.enclosureLength,
            isRead ? 0 : 1,
            isRead ? 1 : 0
        ]);

        // isFeedChanged = true;
    }
}

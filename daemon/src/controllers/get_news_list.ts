import { Database } from "../database";
import { Controller, IResponse, Response, SendTo } from "./controller";

// =====================================================================================================================

interface IGetNewsListResponse extends IResponse {
    data: {
        feed_id: number;
        items: INews[];
        offset: number;
        limit: number;
    };
}

interface INews {
    id: number;
    feed_id: number;
    url: string;
    title: string;
    description: string;
    created: string;
    is_new: boolean;
    is_read: boolean;
    is_starred: boolean;
}

// =====================================================================================================================

export class GetNewsListController extends Controller {
    public async run(params: any): Promise<Response> {
        const db = await Database.getInstance();

        const news: INews[] = [];
        let results;

        let query = `SELECT type FROM feeds WHERE id = ${params.feed_id} LIMIT 1`;

        results = await db.execute(query);

        let feedsIds: number[];

        // Get all feeds ids in folders
        if (results[0].type === "folder") {
            feedsIds = await this.getChildrensIds(params.feed_id);
        }
        else {
            feedsIds = [ params.feed_id ];
        }

        const feedsIdsStr = feedsIds.join(",");

        query = `
            SELECT *
            FROM news
            WHERE
                feed_id IN (${feedsIdsStr}) AND
                is_deleted = 0
            ORDER BY published DESC
            LIMIT ${params.offset}, ${params.limit}
        `;

        results = await db.execute(query);

        for (const itemData of results) {
            const item: INews = {
                id:          itemData.id,
                feed_id:     itemData.feed_id,
                url:         itemData.url,
                title:       itemData.title,
                description: itemData.description,
                created:     itemData.published,
                is_new:      itemData.is_new === 1,
                is_read:     itemData.is_read === 1,
                is_starred:  itemData.is_starred === 1
            };

            news.push(item);
        }

        const response: IGetNewsListResponse = {
            sendTo:  SendTo.ME,
            method:  "getNewsList",
            data: {
                feed_id: params.feed_id,
                items:   news,
                offset:  params.offset,
                limit:   params.limit
            }
        };

        return response;
    }

    // =================================================================================================================

    private async getChildrensIds(categoryId: number) {
        const db = await Database.getInstance();

        const feedsIds: number[] = [];
        let query: string;
        let result;

        let foldersIds: number[] = [ categoryId ];
        let foldersIdsStr: string;

        while (foldersIds.length > 0) {
            foldersIdsStr = foldersIds.join(",");
            foldersIds    = [];

            query = `SELECT id, type FROM feeds WHERE parent_id IN (${foldersIdsStr})`;

            result = await db.execute(query);

            for (const feed of result) {
                if (feed.type === "folder") {
                    foldersIds.push(feed.id);
                }
                else {
                    feedsIds.push(feed.id);
                }
            }
        }

        return feedsIds;
    }
}

import { Database } from "../database";
import { Controller, IResponse, Response, SendTo } from "./controller";

// =====================================================================================================================

interface IRecalcFeedCountersResponse extends IResponse {
    data: {
        items: IFeed[]
    };
}

interface IFeed {
    id: number;
    new_count: number;
    news_count: number;
    unread_count: number;
}

// =====================================================================================================================

export class RecalcFeedCountersController extends Controller {
    public async run(params: any): Promise<Response> {
        const db = await Database.getInstance();

        const feeds: IFeed[] = [];
        let feedId = params.feed_id;
        let feedType: string;

        let query: string;
        let results;

        query = `
            SELECT type FROM feeds WHERE id = ? LIMIT 1
        `;

        results = await db.execute(query, [
            feedId
        ]);

        feedType = results[0].type;

        while (feedId !== 0) {
            let newsCount;
            let newCount;
            let unreadCount;

            if (feedId === params.feed_id && feedType !== "folder") {
                // Get total news count in selected feed
                query = `
                    SELECT COUNT(*) AS count
                    FROM news
                    WHERE
                        feed_id = ? AND
                        is_deleted = 0
                `;

                results = await db.execute(query, [
                    feedId
                ]);

                newsCount = results[0].count;

                // Get total new news count in selected feed
                query = `
                    SELECT COUNT(*) AS count
                    FROM news
                    WHERE
                        feed_id = ? AND
                        is_new = 1
                `;

                results = await db.execute(query, [
                    feedId
                ]);

                newCount = results[0].count;

                // Get total unread news count in selected feed
                query = `
                    SELECT COUNT(*) AS count
                    FROM news
                    WHERE
                        feed_id = ? AND
                        is_read = 0
                `;

                results = await db.execute(query, [
                    feedId
                ]);

                unreadCount = results[0].count;
            }
            else {
                // Get feed info
                query = `
                    SELECT
                        SUM(new_count) AS new_count,
                        SUM(news_count) AS news_count,
                        SUM(unread_count) AS unread_count
                    FROM feeds
                    WHERE parent_id = ?
                    LIMIT 1
                `;

                results = await db.execute(query, [
                    feedId
                ]);

                newCount    = parseInt(results[0].new_count, 10);
                newsCount   = parseInt(results[0].news_count, 10);
                unreadCount = parseInt(results[0].unread_count, 10);
            }

            // Update feed info
            query = `
                UPDATE feeds
                SET
                    new_count = ?,
                    news_count = ?,
                    unread_count = ?
                WHERE id = ?
            `;

            await db.execute(query, [
                newCount,
                newsCount,
                unreadCount,
                feedId
            ]);

            feeds.push({
                id:           feedId,
                new_count:    newCount,
                news_count:   newsCount,
                unread_count: unreadCount
            });

            // Get parent category id
            query = `
                SELECT parent_id, type
                FROM feeds
                WHERE id = ?
                LIMIT 1
            `;

            results = await db.execute(query, [
                feedId
            ]);

            feedId   = results[0].parent_id;
            feedType = results[0].type;
        }

        const response: IRecalcFeedCountersResponse = {
            sendTo: SendTo.ALL,
            method: "recalcFeedsCounters",
            data:   {
                items: feeds
            }
        };

        return response;
    }
}

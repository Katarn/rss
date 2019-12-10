import { Database } from "../database";
import { WebSocket } from "../websocket";
import { Controller, Response } from "./controller";

// =====================================================================================================================

export class SetFeedViewedController extends Controller {
    public async run(params: any): Promise<Response> {
        const db = await Database.getInstance();

        const query = `SELECT type FROM feeds WHERE id = ${params.feed_id} LIMIT 1`;

        const results = await db.execute(query);

        let feedsIds: number[];

        // Get all feeds ids in folders
        if (results[0].type === "folder") {
            feedsIds = await this.getChildrensIds(params.feed_id);

            feedsIds.push(params.feed_id);
        }
        else {
            feedsIds = [ params.feed_id ];
        }

        await this.setFeedsViewed(feedsIds);

        const webSocket = await WebSocket.getInstance();

        await webSocket.runMethod(this.connection, "recalcFeedCounters", params);
    }

    // =================================================================================================================

    private async getChildrensIds(feedId: number) {
        const db = await Database.getInstance();

        const ids: number[] = [];
        let query: string;
        let results;

        let foldersIds: number[] = [ feedId ];
        let foldersIdsStr: string;

        // Get children ids
        while (foldersIds.length > 0) {
            foldersIdsStr = foldersIds.join(",");
            foldersIds    = [];

            query = `SELECT id, type FROM feeds WHERE parent_id IN (${foldersIdsStr})`;

            results = await db.execute(query);

            for (const feed of results) {
                ids.push(feed.id);

                if (feed.type === "folder") {
                    foldersIds.push(feed.id);
                }
            }
        }

        return ids;
    }

    // =================================================================================================================

    private async setFeedsViewed(feedsIds: number[]) {
        const db = await Database.getInstance();

        const feedsIdsStr = feedsIds.join(",");
        let query: string;

        query = `UPDATE feeds SET new_count = 0 WHERE id IN (${feedsIdsStr})`;

        await db.execute(query);

        query = `UPDATE news SET is_new = 0 WHERE feed_id IN (${feedsIdsStr})`;

        await db.execute(query);
    }
}

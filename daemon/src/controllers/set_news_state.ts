import { Database } from "../database";
import { WebSocket } from "../websocket";
import { Controller, IResponse, Response, SendTo } from "./controller";

// =====================================================================================================================

interface ISetNewsState extends IResponse {
    data: {
        items: INews[];
    };
}

interface INews {
    id: number;
    is_read?: boolean;
    is_starred?: boolean;
    is_deleted?: boolean;
}

// =====================================================================================================================

export class SetNewsStateController extends Controller {
    public async run(params: any): Promise<Response> {
        const db = await Database.getInstance();

        const newsIdsArr = [];
        let value = "";

        // Check params
        if (params.is_read === true) {
            value = "is_read = 1";
        }
        else if (params.is_read === false) {
            value = "is_read = 0";
        }

        if (params.is_starred === true) {
            value = "is_starred = 1";
        }
        else if (params.is_starred === false) {
            value = "is_starred = 0";
        }

        if (params.is_deleted === true) {
            value = "is_deleted = 1";
        }
        else if (params.is_deleted === false) {
            value = "is_deleted = 0";
        }

        for (const itemId of params.items) {
            newsIdsArr.push(itemId);
        }

        const newsIdsStr = newsIdsArr.join(",");

        // Update states of all news from request
        const query = `
            UPDATE news
            SET ${value}
            WHERE id IN (${newsIdsStr})
            LIMIT ${newsIdsArr.length}
        `;

        await db.execute(query);

        const news = [];

        for (const itemData of params.items) {
            const item: INews = {
                id: itemData.id
            };

            if (params.is_read !== undefined) {
                item.is_read = params.is_read;
            }

            if (params.is_starred !== undefined) {
                item.is_starred = params.is_starred;
            }

            if (params.is_deleted !== undefined) {
                item.is_deleted = params.is_deleted;
            }

            news.push(item);
        }

        const webSocket = await WebSocket.getInstance();

        await webSocket.runMethod(this.connection, "recalcFeedCounters", params);

        const response: ISetNewsState = {
            sendTo: SendTo.ALL,
            method: "setNewsState",
            data:   {
                items: news
            }
        };

        return response;
    }
}

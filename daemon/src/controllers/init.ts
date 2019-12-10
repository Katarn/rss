import { Database } from "../database";
import { Controller, IResponse, SendTo } from "./controller";

// =====================================================================================================================

interface IInitResponse extends IResponse {
    data: {
        items: IFeed[];
    };
}

interface IFeed {
    id: number;
    parent_id: number;
    type: string;
    title: string;
    new_count: number;
    news_count: number;
    unread_count: number;

    is_expanded?: boolean;
    url?: string;
    icon?: string;
    items?: IFeed[];
}

// =====================================================================================================================

export class InitController extends Controller {
    public async run(_params: any): Promise<IInitResponse> {
        const response = {
            sendTo: SendTo.ME,
            method: "init",
            data:   {
                items: await this.getFeedsList()
            }
        };

        return response;
    }

    // =================================================================================================================

    private async getFeedsList(): Promise<any> {
        const db = await Database.getInstance();

        const tmp: IFeed[] = [];

        const getByParentId = (parentId: number) => {
            const result = tmp.filter((item) => {
                if (item.parent_id === parentId) {
                    return true;
                }

                return false;
            });

            return result;
        };

        const query = `
            SELECT * FROM feeds ORDER BY sort
        `;

        const results = await db.execute(query);

        for (const item of results) {
            const feed: IFeed = {
                id:           item.id,
                parent_id:    item.parent_id,
                type:         item.type,
                title:        item.title,
                new_count:    item.new_count,
                news_count:   item.news_count,
                unread_count: item.unread_count
            };

            if (item.type === "folder") {
                feed.is_expanded = (item.is_expanded) ? true : false;
            }
            else {
                feed.url  = item.url;
                feed.icon = item.icon;
            }

            tmp.push(feed);
        }

        for (const tmpItem of tmp) {
            if (tmpItem.type === "folder") {
                tmpItem.items = getByParentId(tmpItem.id);
            }
        }

        const feeds = tmp.filter((item) => {
            if (item.parent_id === 0) {
                return true;
            }

            return false;
        });

        return feeds;
    }
}

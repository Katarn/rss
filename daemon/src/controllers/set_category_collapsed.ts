import { Database } from "../database";
import { Controller, IResponse, Response, SendTo } from "./controller";

// =====================================================================================================================

interface ISetCategoryCollapsedResponse extends IResponse {
    data: {
        categoryId: number;
    };
}

// =====================================================================================================================

export class SetCategoryCollapsedController extends Controller {
    public async run(params: any): Promise<Response> {
        const db = await Database.getInstance();

        db.execute(`UPDATE feeds SET is_expanded = 0 WHERE id = ${params.category_id} LIMIT 1`);

        const response: ISetCategoryCollapsedResponse = {
            sendTo: SendTo.OTHERS,
            method: "setCategoryCollapsed",
            data:   {
                categoryId: params.category_id
            }
        };

        return response;
    }
}

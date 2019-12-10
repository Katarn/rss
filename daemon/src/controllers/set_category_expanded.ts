import { Database } from "../database";
import { Controller, IResponse, Response, SendTo } from "./controller";

// =====================================================================================================================

interface ISetCategoryExpandedResponse extends IResponse {
    data: {
        categoryId: number;
    };
}

// =====================================================================================================================

export class SetCategoryExpandedController extends Controller {
    public async run(params: any): Promise<Response> {
        const db = await Database.getInstance();

        db.execute(`UPDATE feeds SET is_expanded = 1 WHERE id = ${params.category_id} LIMIT 1`);

        const response: ISetCategoryExpandedResponse = {
            sendTo: SendTo.OTHERS,
            method: "setCategoryExpanded",
            data:   {
                categoryId: params.category_id
            }
        };

        return response;
    }
}

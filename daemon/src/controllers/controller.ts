import { connection } from "websocket";

// =================================================================================================================

export interface IResponse {
    sendTo: SendTo;
    method: any;

    time?: number;
    data?: any;
}

export interface IController {
    run(params: any): Promise<Response>;
}

export type Response = IResponse | void;

export enum SendTo {
    ALL,
    OTHERS,
    ME
}

// =================================================================================================================

export abstract class Controller {
    protected connection: connection;

    // =================================================================================================================

    constructor(conn: connection) {
        this.connection = conn;
    }

    // =================================================================================================================

    public abstract run(params: any): Promise<Response>;
}

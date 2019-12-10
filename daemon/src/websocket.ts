import * as http from "http";
import { connection, IMessage, request as wsRequest, server as Socket } from "websocket";
import { Response, SendTo } from "./controllers/controller";
import { GetNewsListController } from "./controllers/get_news_list";
import { InitController } from "./controllers/init";
import { RecalcFeedCountersController } from "./controllers/recalc_feed_counters";
import { SetCategoryCollapsedController } from "./controllers/set_category_collapsed";
import { SetCategoryExpandedController } from "./controllers/set_category_expanded";
import { SetFeedViewedController } from "./controllers/set_feed_viewed";
import { SetNewsStateController } from "./controllers/set_news_state";

interface IControllers {
    [index: string]: any;
}

export class WebSocket {
    private static instance: WebSocket | null;

    private server: http.Server;
    private websocket: Socket;
    private connections: connection[] = [];

    private controllers: IControllers = {
        init:                 InitController,
        getNewsList:          GetNewsListController,
        setCategoryCollapsed: SetCategoryCollapsedController,
        setCategoryExpanded:  SetCategoryExpandedController,
        setFeedViewed:        SetFeedViewedController,
        setNewsState:         SetNewsStateController,
        recalcFeedCounters:   RecalcFeedCountersController
    };

    // =================================================================================================================

    constructor() {
        this.server = new http.Server();

        http.createServer((request, response) => {
            console.log((new Date()) + ` Received request for ${request.url}`);

            response.writeHead(404);
            response.end();
        });

        this.server.listen(8080, () => {
            console.log((new Date()) + " Server is listening on port 8080");
        });

        this.websocket = new Socket({
            httpServer: this.server,

            // You should not use autoAcceptConnections for production
            // applications, as it defeats all standard cross-origin protection
            // facilities built into the protocol and the browser.  You should
            // *always* verify the connection's origin and decide whether or not
            // to accept it.
            autoAcceptConnections: false
        });
    }

    // =================================================================================================================

    public static async getInstance(): Promise<WebSocket> {
        if (!this.instance) {
            this.instance = new WebSocket();

            await this.instance.init();
        }

        return this.instance;
    }

    // =================================================================================================================

    public async init() {
        this.websocket.on("request", this.onRequest.bind(this));
    }

    // =================================================================================================================

    private onRequest(requestData: wsRequest) {
        const connect = requestData.accept(requestData.origin);

        this.connections.push(connect);

        connect.on("message", this.onMessage.bind(this, connect));
        connect.on("close",   this.onClose.bind(this, connect)  );
    }

    // =================================================================================================================

    private async onMessage(connect: connection, message: IMessage) {
        const request = JSON.parse(String(message.utf8Data));

        console.log(`\u{1b}[1;32m>> ${request.method}\u{1b}[0m`);
        console.log(request.params);

        await this.runMethod(connect, request.method, request.params);
    }

    // =================================================================================================================

    private onClose(connect: connection) {
        console.log((new Date()) + ` Peer ${connect.remoteAddress} disconnected.`);

        const connectionId = this.connections.indexOf(connect);

        this.connections.splice(connectionId, 1);
    }

    // =================================================================================================================

    public send(connect: connection, response: Response) {
        if (response === undefined) {
            return;
        }

        const sendTo      = response.sendTo;
        const responseStr = JSON.stringify(response);

        delete response.sendTo;

        switch (sendTo) {
            case SendTo.ALL: // Send to all opened connections
                for (const conn of this.connections) {
                    conn.sendUTF(responseStr);
                }

                break;

            case SendTo.OTHERS: // Send to all opened connections without current
                for (const conn of this.connections) {
                    if (conn === connect) {
                        continue;
                    }

                    conn.sendUTF(responseStr);
                }

                break;

            case SendTo.ME: // Send only to current opened connection
                connect.sendUTF(responseStr);

                break;
        }

        console.log(`\u{1b}[1;31m<< ${response.method} \u{1b}[0m(${response.time} msec)`);

        if (response.data) {
            if (response.data.items) {
                response.data.items = `...${response.data.items.length} items...`;
            }

            if (response.data.feeds) {
                response.data.feeds = `...${response.data.feeds.length} feeds...`;
            }

            if (response.data.news) {
                response.data.news = `...${response.data.news.length} news...`;
            }

            console.log(response.data);
        }
    }

    // =================================================================================================================

    public async runMethod(connect: connection, method: string, params: any) {
        if (this.controllers[method] === undefined) {
            return;
        }

        const startDate = Date.now();

        const controller = new this.controllers[method](connect);
        const response   = await controller.run(params);

        if (response !== undefined) {
            response.time = Date.now() - startDate;

            this.send(connect, response);
        }
    }
}

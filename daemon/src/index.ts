import { Database } from "./database";
import { FeedsUpdater } from "./feeds_updater";
import { Sessions } from "./sessions";
import { WebSocket } from "./websocket";

const main = async () => {
    await WebSocket.getInstance();
    await Database.getInstance();

    const sessions    = new Sessions();
    const feedUpdater = new FeedsUpdater();

    await sessions.init();
    await feedUpdater.init();
};

main();

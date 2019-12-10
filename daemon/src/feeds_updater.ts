import { Database } from "./database";
import { FeedModel } from "./models/feed_model";

const UPDATE_INTERVAL = 60 * 1000;

export class FeedsUpdater {
    private updateTimer: NodeJS.Timeout | null = null;
    private queue: FeedModel[] = [];
    private queueIds: number[] = [];
    private total = 0;

    private checkFeedsBind: () => void;

    // =================================================================================================================

    constructor() {
        this.checkFeedsBind = this.checkFeeds.bind(this);
    }

    // =================================================================================================================

    public destroy() {
        if (this.updateTimer) {
            clearInterval(this.updateTimer);
            this.updateTimer = null;
        }
    }

    // =================================================================================================================

    public async init() {
        this.updateTimer = setInterval(this.checkFeedsBind, UPDATE_INTERVAL);

        await this.checkFeeds();
    }

    // =================================================================================================================

    private async checkFeeds() {
        const startQueueLength = this.queue.length;

        const db = await Database.getInstance();

        const query = `
            SELECT
                id, url, title, updated, update_interval, update_interval_type
            FROM feeds
            WHERE
                type = 'feed' AND
                disable_update = 0 AND
                update_interval_enabled = 1
        `;

        const results = await db.execute(query);

        for (const feedInfo of results) {
            if (this.queueIds.indexOf(feedInfo.id) === -1) {
                const feed = new FeedModel(feedInfo);

                if (feed.isNeedForUpdate() && !feed.isUpdating()) {
                    this.addToQueue(feed);
                }
            }
        }

        if (startQueueLength === 0 && this.queue.length > 0) {
            this.fetchFeeds();
        }
    }

    // =================================================================================================================

    private addToQueue(feed: FeedModel) {
        this.queue.push(feed);
        this.queueIds.push(feed.getId());

        this.total++;
    }

    // =================================================================================================================

    private async fetchFeeds() {
        let num = 1;

        console.log(`Feeds in queue: ${this.total}`);

        while (this.queue.length) {
            const feed = this.queue[0];

            const feedId    = feed.getId().toString().padEnd(6);
            const feedTitle = feed.getTitle().padEnd(50);
            const feedUrl   = feed.getUrl();

            console.log(`[${num}/${this.total}]   ${feedId}${feedTitle}${feedUrl}...`);

            await feed.update();

            this.queue.shift();
            this.queueIds.shift();

            num++;
        }

        this.total = 0;
    }
}

import * as SqliteDb from "better-sqlite3";
import * as MysqlDb from "mysql2/promise";
import { config as Config } from "./config";

const main = async () => {
    const sqlite = new SqliteDb(Config.dbSqlite, {
        readonly: true
    });

    const mysql = await MysqlDb.createConnection({
        host:     Config.dbHost,
        database: Config.dbName,
        user:     Config.dbUser,
        password: Config.dbPassword
    });

    await mysql.execute("TRUNCATE TABLE feeds");
    await mysql.execute("TRUNCATE TABLE news");
    await mysql.query("LOCK TABLE feeds WRITE, news WRITE");

    console.log("Convert feeds...");

    const feeds = sqlite
                    .prepare("SELECT * FROM feeds ORDER BY id")
                    .all();

    for (const feed of feeds) {
        let isExpanded = 0;

        if (feed.xmlUrl === "" && feed.f_Expanded === 1) {
            isExpanded = 1;
        }

        let updateInterval = parseInt(feed.updateInterval, 10) || 30;
        let updateIntervalType = "mins";

        switch (parseInt(feed.updateIntervalType, 10)) {
            case -1:
                updateIntervalType = "mins";
                updateInterval *= 60;
                break;

            case 0:
                updateIntervalType = "mins";
                break;

            case 1:
                updateIntervalType = "hours";
                break;
        }

        let javascriptEnable = "global";

        switch (feed.javaScriptEnable) {
            case 0:
                javascriptEnable = "no";
                break;

            case 1:
                javascriptEnable = "global";
                break;

            case 2:
                javascriptEnable = "yes";
                break;
        }

        await mysql.execute(`
            INSERT INTO feeds
            SET
                id = ?,
                parent_id = ?,
                type = ?,
                url = ?,
                feed_url = ?,
                title = ?,
                feed_title = ?,
                feed_description = ?,
                language = ?,
                copyrights = ?,
                author_name = ?,
                author_email = ?,
                author_uri = ?,
                webmaster = ?,
                category = ?,
                contributor = ?,
                generator = ?,
                docs = ?,
                icon = ?,
                sort = ?,
                is_expanded = ?,
                unread_count = ?,
                new_count = ?,
                news_count = ?,
                labels = ?,
                tags = ?,
                disable_update = ?,
                update_interval_enabled = ?,
                update_interval = ?,
                update_interval_type = ?,
                delete_read = ?,
                never_delete_unread = ?,
                never_delete_starred = ?,
                never_delete_labeled = ?,
                updated = ?,
                created = ?,
                status = ?,
                authentication = ?,
                delete_duplicates = ?,
                javascript_enable = ?,
                layout_direction = ?
        `, [
            feed.id,
            feed.parentId,
            feed.xmlUrl ? "feed" : "folder",
            feed.xmlUrl || "",
            feed.htmlUrl || "",
            feed.text || "",
            feed.title || "",
            feed.description || "",
            feed.language || "",
            feed.copyrights || "",
            feed.author_name || "",
            feed.author_email || "",
            feed.author_uri || "",
            feed.webmaster || "",
            feed.category || "",
            feed.contributor || "",
            feed.generator || "",
            feed.docs || "",
            feed.image || "",
            feed.rowToParent || 0,
            isExpanded,
            feed.unread || 0,
            feed.newCount || 0,
            feed.undeleteCount || 0,
            feed.labels || "",
            feed.tags || "",
            feed.disableUpdate || 0,
            feed.updateIntervalEnable === 1 ? 1 : 0,
            updateInterval,
            updateIntervalType,
            feed.delete_read === 1 ? true : false,
            feed.neverDeleteUnreadNews === 1 ? true : false,
            feed.neverDeleteStarredNews === 1 ? true : false,
            feed.neverDeleteLabeledNews === 1 ? true : false,
            feed.updated,
            feed.created,
            feed.status || "",
            feed.authentication === 1 ? true : false,
            feed.duplicateNewsMode === 1 ? true : false,
            javascriptEnable,
            feed.layoutDirection === 0 ? "left_to_right" : "right_to_left"
        ]);
    }

    console.log("Convert news...");

    const news = sqlite
                    .prepare("SELECT * FROM news ORDER BY id")
                    .all();

    for (const item of news) {
        await mysql.execute(`
            INSERT INTO news
            SET
                id = ?,
                feed_id = ?,
                guid = ?,
                link = ?,
                title = ?,
                description = ?,
                published = ?,
                is_new = ?,
                is_read = ?,
                is_starred = ?,
                is_deleted = ?
        `, [
            item.id,
            item.feedId,
            item.guid || "",
            item.link_href || "",
            item.title || "",
            item.description || "",
            item.published,
            (item.deleted || !item.new) ? 0 : 1,
            (item.read > 0 || item.deleted) ? 1 : 0,
            (!item.starred || item.deleted) ? 0 : 1,
            item.deleted === 0 ? 0 : 1
        ]);
    }

    await mysql.query("UNLOCK TABLE");

    mysql.end();
};

main();

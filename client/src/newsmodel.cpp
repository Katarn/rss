#include "newsmodel.h"

#include <QHeaderView>

NewsModel::NewsModel(QObject * parent, QTreeView * view, FeedsModel * feedsModel)
    : QAbstractItemModel(parent)
    , m_view(view)
    , m_feedsModel(feedsModel)
{
    m_isInited = false;
    m_news = new QVector<NewsData>();

    m_server = Server::instance();

    connect(m_server, SIGNAL(signalServerGetNewsList(const QJsonObject&)), this, SLOT(slotGetNewsListCommand(const QJsonObject&)));
}

// =====================================================================================================================

NewsModel::~NewsModel()
{
    clear();
}

// =====================================================================================================================

void NewsModel::parseJson(const QJsonObject &json)
{
    QJsonArray items = json.value("items").toArray();
    int feedId = json.value("feed_id").toInt();

    if (m_feedItem->id() != feedId) {
        return;
    }

    int offset = json.value("offset").toInt();
//    int limit = json.value("limit").toInt();

    for (int i = 0; i < items.count(); i++) {
        const QJsonObject obj = items.at(i).toObject();
        NewsData &news = m_news->operator[](offset + i);

        news.id          = obj.value("id").toInt();
        news.feedId      = obj.value("feed_id").toInt();
        news.url         = obj.value("url").toString();
        news.title       = obj.value("title").toString();
        news.description = obj.value("description").toString();
        news.created     = obj.value("created").toString();
        news.isNew       = obj.value("is_new").toBool();
        news.isRead      = obj.value("is_read").toBool();
        news.isStarred   = obj.value("is_starred").toBool();
        news.isLoaded    = true;

        news.feedIcon = m_feedsModel->feedItem(news.feedId)->icon();
    }
/*
    QModelIndex start = createIndex(offset, 0);
    QModelIndex end   = createIndex(offset + limit, 0);

    emit dataChanged(start, end);
*/
}

// =====================================================================================================================

void NewsModel::clear()
{
    m_news->clear();
}

// =====================================================================================================================

Qt::ItemFlags NewsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    return defaultFlags;
}

// =====================================================================================================================

int NewsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_news->length();
}

// =====================================================================================================================

int NewsModel::columnCount(const QModelIndex&) const
{
    return 7;
}

// =====================================================================================================================

QModelIndex NewsModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

// =====================================================================================================================

QModelIndex NewsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

// =====================================================================================================================

QVariant NewsModel::dataField(const int index, NewsModel::Columns column) const
{
    NewsData newsData = m_news->at(index);

    switch (column) {
        case NewsModel::Columns::Id:
            return QVariant(newsData.id);

        case NewsModel::Columns::FeedId:
            return QVariant(newsData.feedId);

        case NewsModel::Columns::Title:
            return QVariant(newsData.title);

        case NewsModel::Columns::Published:
            return QVariant(newsData.created);
            break;
    }
}

// =====================================================================================================================

QVariant NewsModel::data(const QModelIndex &index, int role) const
{
    const NewsData &news = m_news->at(index.row());

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
            case NewsModel::Columns::AuthorName:
                return "AuthorName";

            case NewsModel::Columns::Category:
                return "Category";

            case NewsModel::Columns::Title:
                if (news.isLoaded) {
                    return news.title;
                }
                else {
                    return tr("Загрузка...");
                }

            case NewsModel::Columns::Published:
                QDateTime dtLocal;

                if (!news.created.isNull()) {
                    const QDateTime dtLocalTime = QDateTime::currentDateTime();
                    const QDateTime dtUTC = QDateTime(dtLocalTime.date(), dtLocalTime.time(), Qt::UTC);
                    const int nTimeShift = dtLocalTime.secsTo(dtUTC);

                    const QDateTime dt = QDateTime::fromString(news.created, Qt::ISODate);
                    dtLocal = dt.addSecs(nTimeShift);
                }

                const QString formatDate_ = "dd.MM.yy";
                const QString formatTime_ = "hh:mm";

                if (false) {
                    if (QDateTime::currentDateTime().date() <= dtLocal.date()) {
                        return dtLocal.toString(formatTime_);
                    }
                    else {
                        return dtLocal.toString(formatDate_);
                    }
                }
                else {
                    return dtLocal.toString(formatDate_ + " " + formatTime_);
                }
        }
    }
    else if (role == Qt::DecorationRole) {
        switch(index.column()) {
            case NewsModel::Columns::Starred:
                if (news.isStarred) {
                    return QPixmap(":/images/starred");
                }
                else {
                    return QPixmap(":/images/unstarred");
                }

            case NewsModel::Columns::Read:
                if (news.isNew) {
                    return QPixmap(":/images/bullet-new");
                }
                else if (news.isRead) {
                    return QPixmap(":/images/bullet-read");
                }
                else {
                    return QPixmap(":/images/bullet-unread");
                }

            case NewsModel::Columns::Icon:
                if (news.isLoaded) {
                    return news.feedIcon;
                }

                return m_feedItem->icon();
        }
    }
    if (role == Qt::FontRole) {
        QFont font;

        if (news.isLoaded && !news.isRead) {
            font.setBold(true);
        }

        return font;
    }

    return QVariant();
}

// =====================================================================================================================

bool NewsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    if (index.row() < 0 || index.row() >= m_news->length()) {
        return false;
    }

    NewsData &newsData = m_news->operator[](index.row());

    switch (index.column()) {
        case NewsModel::Columns::Id:
            newsData.id = value.toInt();
            break;

        case NewsModel::Columns::Title:
            newsData.title = value.toString();
            break;

        case NewsModel::Columns::Published:
            newsData.created = value.toString();
            break;

    case NewsModel::Columns::Read:
        newsData.isRead = value.toBool();
        break;
    }

    emit dataChanged(index, index);

    return true;
}

// =====================================================================================================================

QVariant NewsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        QString text = m_headers.value(section).value(role).toString();

        if (text.isEmpty()) {
            return QVariant();
        }

        int stopColFix = 0;

        for (int i = m_view->header()->count() - 1; i >= 0; i--) {
            int lIdx = m_view->header()->logicalIndex(i);

            if (!m_view->header()->isSectionHidden(lIdx)) {
                stopColFix = lIdx;
                break;
            }
        }

        int padding = 8;

        if (stopColFix == section) {
            padding = padding + 20;
        }

        text = m_view->header()->fontMetrics().elidedText(text, Qt::ElideRight, m_view->header()->sectionSize(section) - padding);

        return text;
    }
    else if (role == Qt::DecorationRole) {
        QVariant var = m_headers.value(section).value(role);

        return var.value<QPixmap>();
    }
    else if (role == Qt::EditRole) {
        return m_headers.value(section).value(role);
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

// =====================================================================================================================

bool NewsModel::setHeaderData(int section, Qt::Orientation orientation,
                                   const QVariant &value, int role)
{
    if (section < 0 || section >= columnCount()) {
        return false;
    }

    if (m_headers.size() <= section) {
        m_headers.resize(qMax(section + 1, 16));
    }

    if (role == Qt::DisplayRole) {
        m_headers[section][Qt::EditRole] = value;
    }

    m_headers[section][role] = value;

    emit headerDataChanged(orientation, section, section);

    return true;
}

// =====================================================================================================================

QVector<NewsData> NewsModel::news() const
{
    return *m_news;
}

// =====================================================================================================================

const NewsData &NewsModel::newsById(int id) const
{
    for (int i = 0; i < m_news->count(); i++) {
        if (m_news->at(i).id == id) {
            return m_news->at(i);
        }
    }
}

// =====================================================================================================================

const NewsData &NewsModel::newsByModel(const QModelIndex &index) const
{
    const NewsData &news = newsById(index.internalId());

    return news;
}

// =====================================================================================================================

const NewsData NewsModel::newsByIndex(const int index) const
{
    return m_news->at(index);
}

// =====================================================================================================================

void NewsModel::slotGetNewsListCommand(const QJsonObject &newsList)
{
    beginResetModel();
    parseJson(newsList);
    endResetModel();

    m_isInited = true;
}

// =====================================================================================================================

void NewsModel::setFeed(FeedItem *feedItem)
{
    m_isInited = false;

    beginResetModel();
    clear();
    m_feedItem = feedItem;
    endResetModel();

    m_news->reserve(m_feedItem->newsCount());
    m_news->squeeze();

    for (int i = 0; i < m_feedItem->newsCount(); i++) {
        NewsData news;

        news.isStarred = false;
        news.isLoaded  = false;

        m_news->append(news);
    }
}

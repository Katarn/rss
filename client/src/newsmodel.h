#ifndef NEWSMODEL_H
#define NEWSMODEL_H

#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QTreeView>

#include "server.h"
#include "feedsmodel.h"
#include "structs.h"

struct NewsData {
    int id;
    int feedId;
    QPixmap feedIcon;
    QString url;
    QString title;
    QString description;
    QString created;
    bool isNew;
    bool isRead;
    bool isStarred;
    bool isLoaded;
};

class NewsModel : public QAbstractItemModel
{
    Q_OBJECT

private:
    //QVector<int> childIds(int parentId) const;
    void parseJson(const QJsonObject &json);
    void clear();

    FeedItem *m_feedItem;
    QVector<NewsData> *m_news;

    Server *m_server;

    bool m_isInited;

    QVector<QHash<int, QVariant> > m_headers;

    QTreeView *m_view;

    FeedsModel *m_feedsModel;

public:
    enum Columns {
        Id         = 0,
        Icon       = 1,
        Title      = 2,
        Published  = 3,
        AuthorName = 4,
        Read       = 5,
        Starred    = 6,
        Category   = 7,
        Received   = 8,
        Label      = 9,
        Rights     = 10,
        Url        = 11,
        New        = 12,
        FeedId     = 13
    };

    explicit NewsModel(QObject * parent, QTreeView * view, FeedsModel *feedsModel);
    ~NewsModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant dataField(const int index, NewsModel::Columns column) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation,
                                       const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVector<NewsData> news() const;
    const NewsData &newsById(int id) const;
    const NewsData &newsByModel(const QModelIndex &index) const;
    const NewsData newsByIndex(const int index) const;

    void setFeed(FeedItem *feedItem);

public slots:
    void slotGetNewsListCommand(const QJsonObject &newsList);
};

#endif // NEWSMODEL_H

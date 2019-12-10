#ifndef FEEDSMODEL_H
#define FEEDSMODEL_H

#include <QAbstractItemModel>
#include <QFont>
#include <QPixmap>
#include <QFile>
#include <QPaintEvent>

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include "server.h"
#include "feeditem.h"

class FeedsModel : public QAbstractItemModel
{
    Q_OBJECT
private:
    void clear();

    Server *m_server;

    FeedItem *m_rootItem;

public:
    enum FeedColumn {
        Id,
        IsFolder
    };

    explicit FeedsModel(QObject *parent = nullptr);
    ~FeedsModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    FeedItem *feedItem(int feedId);
    FeedItem *feedItem(QModelIndex index);

    QModelIndex indexFromFeedId(int feedId);
    QModelIndexList folders();
    void updateFeedItemView(int feedId);

private slots:
    void slotInitCommand(const QJsonObject &json);
    void slotRecalcFeedsCountersCommand(const QJsonObject &json);
};

#endif // FEEDSMODEL_H

#ifndef FEEDSVIEW_H
#define FEEDSVIEW_H

#include <QTreeView>
#include <QHeaderView>

#include "delegatewithoutfocus.h"
#include "feedsmodel.h"
#include "server.h"

class FeedsView : public QTreeView
{
Q_OBJECT

private:
    FeedsModel *m_sourceModel;

    Server *m_server;
    bool m_isInited;

public:
    explicit FeedsView(QWidget * parent = nullptr);

    void setSourceModel(FeedsModel *model);
    void expandFeeds();

public slots:
    void refresh();

    void onCollapsed(const QModelIndex &index);
    void onExpanded(const QModelIndex &index);
};

#endif // FEEDSVIEW_H

#include <QHBoxLayout>

#include "newsheader.h"
#include "settings.h"

NewsHeader::NewsHeader(NewsModel *model, QWidget *parent)
  : QHeaderView(Qt::Horizontal, parent)
  , m_model(model)
  , m_move(false)
{
    setObjectName("newsHeader");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSectionsMovable(true);
    setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setMinimumSectionSize(22);
    setStretchLastSection(true);

    m_viewMenu = new QMenu(this);
    m_actionGroupColumnVisible = new QActionGroup(this);
    m_actionGroupColumnVisible->setExclusive(false);
    connect(m_actionGroupColumnVisible, SIGNAL(triggered(QAction*)),
            this, SLOT(slotColumnVisible(QAction*)));

    m_buttonColumnView = new QPushButton(this);
    m_buttonColumnView->setIcon(QIcon(":/images/column"));
    m_buttonColumnView->setObjectName("buttonColumnView");
    m_buttonColumnView->setFlat(true);
    m_buttonColumnView->setCursor(Qt::ArrowCursor);
    m_buttonColumnView->setFocusPolicy(Qt::NoFocus);
    m_buttonColumnView->setMaximumWidth(30);
    connect(m_buttonColumnView, SIGNAL(clicked()), this, SLOT(slotButtonColumnView()));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);
    buttonLayout->addWidget(m_buttonColumnView, 0, Qt::AlignRight|Qt::AlignVCenter);
    setLayout(buttonLayout);

    connect(this, SIGNAL(sectionMoved(int, int, int)), SLOT(slotSectionMoved(int, int, int)));

//    this->installEventFilter(this);
}

// =====================================================================================================================

void NewsHeader::init()
{
    if (count() == 0) {
        return;
    }

    for (int i = 0; i < count(); ++i) {
        hideSection(i);
    }

    showSection(NewsModel::Columns::Icon      );
    showSection(NewsModel::Columns::Title     );
    showSection(NewsModel::Columns::Published );
    //showSection(NewsModel::Columns::AuthorName);
    showSection(NewsModel::Columns::Read      );
    showSection(NewsModel::Columns::Starred   );
    //showSection(NewsModel::Columns::Category  );

    m_model->setHeaderData(NewsModel::Columns::Icon,    Qt::Horizontal, "", Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Read,    Qt::Horizontal, "", Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Read,    Qt::Horizontal, QPixmap(":/images/read"), Qt::DecorationRole);
    m_model->setHeaderData(NewsModel::Columns::Starred, Qt::Horizontal, "", Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Starred, Qt::Horizontal, QPixmap(":/images/star"), Qt::DecorationRole);

    moveSection(visualIndex(NewsModel::Columns::Starred),    0);
    moveSection(visualIndex(NewsModel::Columns::Read),       1);
    moveSection(visualIndex(NewsModel::Columns::Icon),       2);
    moveSection(visualIndex(NewsModel::Columns::Title),      3);
    moveSection(visualIndex(NewsModel::Columns::AuthorName), 4);
    moveSection(visualIndex(NewsModel::Columns::Category),   5);
    moveSection(visualIndex(NewsModel::Columns::Published),  6);

    resizeSection(NewsModel::Columns::Icon,       22 );
    resizeSection(NewsModel::Columns::AuthorName, 100);
    resizeSection(NewsModel::Columns::Title,      200);
    resizeSection(NewsModel::Columns::Starred,    22 );
    resizeSection(NewsModel::Columns::Read,       22 );

    setSectionResizeMode(NewsModel::Columns::Starred, QHeaderView::Fixed);
    setSectionResizeMode(NewsModel::Columns::Icon,    QHeaderView::Fixed);
    setSectionResizeMode(NewsModel::Columns::Read,    QHeaderView::Fixed);

    m_move = true;
}

// =====================================================================================================================

void NewsHeader::retranslateStrings()
{
    if (count() == 0) {
        return;
    }

    m_model->setHeaderData(NewsModel::Columns::Icon,       Qt::Horizontal, tr("Icon Feed"),  Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Title,      Qt::Horizontal, tr("Title"),      Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Published,  Qt::Horizontal, tr("Published"),  Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Received,   Qt::Horizontal, tr("Received"),   Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::AuthorName, Qt::Horizontal, tr("Author"),     Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Category,   Qt::Horizontal, tr("Category"),   Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Read,       Qt::Horizontal, tr("Read"),       Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Starred,    Qt::Horizontal, tr("Star"),       Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Label,      Qt::Horizontal, tr("Label"),      Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Rights,     Qt::Horizontal, tr("Feed Title"), Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Url,        Qt::Horizontal, tr("Link"),       Qt::DisplayRole);

    createMenu();

    m_model->setHeaderData(NewsModel::Columns::Icon, Qt::Horizontal, "", Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Read, Qt::Horizontal, "", Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Read, Qt::Horizontal,
                          QPixmap(":/images/read"), Qt::DecorationRole);
    m_model->setHeaderData(NewsModel::Columns::Starred, Qt::Horizontal, "", Qt::DisplayRole);
    m_model->setHeaderData(NewsModel::Columns::Starred, Qt::Horizontal,
                          QPixmap(":/images/star"), Qt::DecorationRole);

}

// =====================================================================================================================

void NewsHeader::createMenu()
{
    QListIterator<QAction *> iter(m_actionGroupColumnVisible->actions());

    while (iter.hasNext()) {
        QAction *action = iter.next();

        delete action;
    }

    for (int i = 0; i < count(); i++) {
        int lIdx = logicalIndex(i);

        if ((lIdx == NewsModel::Columns::Icon) ||
            (lIdx == NewsModel::Columns::Title) ||
            (lIdx == NewsModel::Columns::Published) ||
            (lIdx == NewsModel::Columns::Received) ||
            (lIdx == NewsModel::Columns::AuthorName) ||
            (lIdx == NewsModel::Columns::Category) ||
            (lIdx == NewsModel::Columns::Read) ||
            (lIdx == NewsModel::Columns::Starred) ||
            (lIdx == NewsModel::Columns::Label) ||
            (lIdx == NewsModel::Columns::Rights) ||
            (lIdx == NewsModel::Columns::Url)) {

            QAction *action = m_actionGroupColumnVisible->addAction(
                m_model->headerData(lIdx, Qt::Horizontal, Qt::EditRole).toString());

            action->setData(lIdx);
            action->setCheckable(true);
            action->setChecked(!isSectionHidden(lIdx));
        }
    }

    m_viewMenu->addActions(m_actionGroupColumnVisible->actions());
}


// =====================================================================================================================

QString NewsHeader::columnsList()
{
    QString indexColumnsStr;

    int columnShowCount = 0;

    for (int i = 0; i < count(); i++) {
        if (!isSectionHidden(logicalIndex(i))) {
            columnShowCount++;
            indexColumnsStr.append(",");
            indexColumnsStr.append(QString::number(logicalIndex(i)));
        }
    }

    indexColumnsStr.append(",");

    if (columnShowCount > m_actionGroupColumnVisible->actions().count()) {
        indexColumnsStr.clear();
    }

    return indexColumnsStr;
}

// =====================================================================================================================

void NewsHeader::saveStateColumns(NewsTabWidget *newsTabWidget)
{
    //if (newsTabWidget->type() == NewsTabWidget::TabType::Del) {
    //    return;
    //}

    //int feedId = newsTabWidget->feedId_;
    //MainWindow *mainWindow = mainApp->mainWindow();
    //QModelIndex indexOld = mainWindow->feedsModel_->indexById(feedId);

    Settings settings;

    settings.beginGroup("NewsHeader");

    settings.setValue("state", saveState());

    //if (mainWindow->feedsModel_->dataField(indexOld, "columns").toString().isEmpty()) {
        settings.setValue("columns", columnsList());
        //settings.setValue("sortBy", sortIndicatorSection());
        //settings.setValue("sortOrder", sortIndicatorOrder());
    //}

    settings.endGroup();
}

// =====================================================================================================================

void NewsHeader::adjustAllColumnsWidths(const int newWidth)
{
    int size = 0;
    int tWidth = 0;
    bool minSize = false;

    QVector<int> widthCol(count(), 0);
    static int idxColSize = count() - 1;

    for (int i = 0; i < count(); i++) {
        tWidth += sectionSize(i);
    }

    if (tWidth > newWidth) {
        minSize = true;
        size = tWidth - newWidth;
        int titleSectionSize = sectionSize(NewsModel::Columns::Title);

        if (titleSectionSize - size >= 40) {
            widthCol[visualIndex(NewsModel::Columns::Title)] = size;
            size = 0;
        }
        else {
            widthCol[visualIndex(NewsModel::Columns::Title)] = titleSectionSize - 40;
            size = size + 40 - titleSectionSize;
        }
    }
    else {
        size = newWidth - tWidth;
        widthCol[visualIndex(NewsModel::Columns::Title)] = size;
        size = 0;
    }

    int countCol = 0;
    bool sizeOne = false;

    while (size) {
        int lIdx = logicalIndex(idxColSize);

        if (!isSectionHidden(lIdx)) {
            if (!((NewsModel::Columns::Read == lIdx) || (NewsModel::Columns::Starred == lIdx) ||
                (NewsModel::Columns::Icon == lIdx) || (NewsModel::Columns::Title == lIdx))) {
                    if (((sectionSize(lIdx) >= 40) && !minSize) ||
                        ((sectionSize(lIdx) - widthCol[idxColSize] > 40) && minSize)) {

                            widthCol[idxColSize]++;
                            size--;
                            sizeOne = true;
                    }
            }
        }

        if (idxColSize == 0) {
            idxColSize = count() - 1;
        }
        else {
            idxColSize--;
        }

        if (++countCol == count()) {
            if (!sizeOne) {
                break;
            }

            sizeOne = false;
            countCol = 0;
        }
    }

    for (int i = count()-1; i >= 0; i--) {
        int lIdx = logicalIndex(i);

        if ((!isSectionHidden(lIdx) && (sectionSize(lIdx) >= 40)) ||
            (NewsModel::Columns::Title == lIdx)) {

            if (!minSize) {
                resizeSection(lIdx, sectionSize(lIdx) + widthCol[i]);
            }
            else {
                resizeSection(lIdx, sectionSize(lIdx) - widthCol[i]);
            }
        }
    }
}

// =====================================================================================================================

void NewsHeader::slotButtonColumnView()
{
    m_viewMenu->setFocus();
    m_viewMenu->show();

    QPoint pPoint;

    pPoint.setX(mapToGlobal(QPoint(0, 0)).x() + width() - m_viewMenu->width() - 1);
    pPoint.setY(mapToGlobal(QPoint(0, 0)).y() + height() + 1);

    m_viewMenu->popup(pPoint);
}

// =====================================================================================================================

void NewsHeader::slotColumnVisible(QAction *action)
{
    int columnShowCount = 0;

    for (int i = 0; i < count(); i++) {
        if (!isSectionHidden(i)) {
            columnShowCount++;
        }
    }

    if (columnShowCount == 1 && !action->isChecked()) {
        action->setChecked(true);

        return;
    }

    int idx = action->data().toInt();

    setSectionHidden(idx, !isSectionHidden(idx));

    if (!isSectionHidden(idx)) {
        if (NewsModel::Columns::Starred == idx ||
            NewsModel::Columns::Read == idx ||
            NewsModel::Columns::Icon == idx) {

            resizeSection(idx, 22);
        }
        else {
            resizeSection(idx, 60);
        }
    }

    adjustAllColumnsWidths(size().width() + 1);
}

// =====================================================================================================================

void NewsHeader::slotSectionMoved(int lIdx, int oldVIdx, int newVIdx)
{
    Q_UNUSED(oldVIdx)

    if (!m_move) {
        return;
    }

    if (NewsModel::Columns::Read == lIdx ||
        NewsModel::Columns::Starred == lIdx ||
        NewsModel::Columns::Icon == lIdx) {

        for (int i = count() - 1; i >= 0; i--) {
            if (!isSectionHidden(logicalIndex(i))) {
                if (i == newVIdx) {
                    resizeSection(lIdx, 45);
                    break;
                }
                else {
                    resizeSection(lIdx, 22);
                    break;
                }
            }
        }

        adjustAllColumnsWidths(size().width() + 1);
    }

    createMenu();
}

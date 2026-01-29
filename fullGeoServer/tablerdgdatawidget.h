#ifndef TABLERDGDATAWIDGET_H
#define TABLERDGDATAWIDGET_H

#include <QTableView>

class tableRdgDataWidget : public QTableView {
    Q_OBJECT

public:
      tableRdgDataWidget(QAbstractItemModel* model);
      ~tableRdgDataWidget();

private:
      QTableView* m_tableRdgDataWidget;

};

#endif // TABLERDGDATAWIDGET_H

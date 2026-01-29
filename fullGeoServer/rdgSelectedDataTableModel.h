#ifndef RDGSSELECTEDDATATABLEMODEL_H
#define RDGSSELECTEDDATATABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>
#include <QtCore/QRect>

class rdgSelectedDataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit rdgSelectedDataTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    int m_columnCount;
    int m_rowCount;
    QList<QVector<QString> > m_data;
};

#endif // RDGSSELECTEDDATATABLEMODEL_H

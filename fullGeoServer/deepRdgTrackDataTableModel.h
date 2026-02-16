#ifndef DEEPRDGTRACKDATATABLEMODEL_H
#define DEEPRDGTRACKDATATABLEMODEL_H

#include <QtCore/QAbstractTableModel>

class deepRdgTrackDataTableModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    explicit deepRdgTrackDataTableModel(QObject* parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    
    int m_columnCount;
    int m_rowCount;
    QList<QVector<QString>> m_data;
};







#endif // RDGDEEPTRACKDATATABLEMODEL_H

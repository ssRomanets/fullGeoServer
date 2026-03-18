 #include "ascanDeepDataTableModel.h"

ascanDeepDataTableModel::ascanDeepDataTableModel(QObject* parent): QAbstractTableModel(parent){}

int ascanDeepDataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowCount;
}

int ascanDeepDataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant ascanDeepDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (section == 0) return "track number";
        if (section == 1) return "ref number";
        if (section == 2) return "time delay ns";
        if (section == 3) return "Глубина m";
        if (section == 4) return "latitude";
        if (section == 5) return "longitude";
    }
    else return QString("%1").arg(section + 1);
}

QVariant ascanDeepDataTableModel::data(const QModelIndex &index, int role) const
{
    if      (role == Qt::DisplayRole) { return m_data[index.row()].at(index.column());}
    else if (role == Qt::EditRole)    { return m_data[index.row()].at(index.column());}
    return QVariant();
}

bool ascanDeepDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()].replace(index.column(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return true;
}

Qt::ItemFlags ascanDeepDataTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
















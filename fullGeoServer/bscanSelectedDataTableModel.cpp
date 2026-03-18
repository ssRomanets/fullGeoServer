#include "bscanSelectedDataTableModel.h"

#include <iostream>

#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QRect>
#include <QtGui/QColor>

bscanSelectedDataTableModel::bscanSelectedDataTableModel(QObject *parent) : QAbstractTableModel(parent){}

int bscanSelectedDataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowCount;
}

int bscanSelectedDataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant bscanSelectedDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (section  == 0) return "layerId";
        if (section  == 1) return "trackId";
        if (section  == 2) return "refId";
        if (section  == 3) return "time delay ns";
        if (section  == 4) return "latitude";
        if (section  == 5) return "longitude";
    }
    else
    {
        return QString("%1").arg(section + 1);
    }
}

QVariant bscanSelectedDataTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()].at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()].at(index.column());
    }
    return QVariant();
}

bool bscanSelectedDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()].replace(index.column(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags bscanSelectedDataTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

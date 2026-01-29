#include "rdgSelectedDataTableModel.h"

#include <iostream>

#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QRect>
#include <QtGui/QColor>

rdgSelectedDataTableModel::rdgSelectedDataTableModel(QObject *parent) : QAbstractTableModel(parent){}

int rdgSelectedDataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowCount;
}

int rdgSelectedDataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant rdgSelectedDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (section  == 0) return "layerId";
        if (section  == 1) return "trackId";
        if (section  == 2) return "impulseId";
        if (section  == 3) return "time delay ns";
        if (section  == 4) return "latitude";
        if (section  == 5) return "longitude";
    }
    else
    {
        return QString("%1").arg(section + 1);
    }
}

QVariant rdgSelectedDataTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()].at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()].at(index.column());
    }
    return QVariant();
}

bool rdgSelectedDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()].replace(index.column(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags rdgSelectedDataTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

#include "rdgTrackDataTableModel.h"

#include <iostream>

#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QRect>
#include <QtGui/QColor>

rdgTrackDataTableModel::rdgTrackDataTableModel(QObject *parent) : QAbstractTableModel(parent){}

int rdgTrackDataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowCount;
}

int rdgTrackDataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant rdgTrackDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (section == 0) return "track number";
        if (section == 1) return "time delay ns";
        if (section == 2) if (m_showLogRdg == false) return "Глубина по импульсу m"; else return "log Импульс";
        if (section == 3) return "latitude";
        if (section == 4) return "longitude";
    }
    else    return QString("%1").arg(section + 1);
}

QVariant rdgTrackDataTableModel::data(const QModelIndex &index, int role) const
{
    if      (role == Qt::DisplayRole) { return m_data[index.row()].at(index.column());} 
    else if (role == Qt::EditRole   ) { return m_data[index.row()].at(index.column());}
    return QVariant();
}

bool rdgTrackDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()].replace(index.column(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags rdgTrackDataTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}














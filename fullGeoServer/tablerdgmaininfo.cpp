#include "tableRdgMainInfo.h"

tableRdgMainInfo::tableRdgMainInfo(int id, double depth)
{
    m_id = id;
    m_depth = depth;
}

int tableRdgMainInfo::getId() const
{
    return m_id;
}

void tableRdgMainInfo::setId(int id)
{
    m_id = id;
}

double tableRdgMainInfo::getDepth() const
{
    return m_depth;
}

void tableRdgMainInfo::setDepth(double depth)
{
    m_depth = depth;
}

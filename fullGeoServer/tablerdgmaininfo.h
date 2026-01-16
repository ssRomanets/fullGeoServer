#ifndef TABLERDGMAININFO_H
#define TABLERDGMAININFO_H

struct tableRdgMainInfo
{
public:
    tableRdgMainInfo(int id, double depth);

    int getId() const;
    void setId(int value);

    double getDepth() const;
    void setDepth(double value);

private:
    int m_id;
    int m_depth;

};

#endif // TABLERDGMAININFO_H

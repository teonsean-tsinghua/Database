#ifndef DBPAGE_H_INCLUDED
#define DBPAGE_H_INCLUDED

#include"DBSlot.h"
#include"../DBInclude.h"

class DBPage
{
protected:
    DBPageInfoSlot* pis;
    BufType cache;
    BufType boundary;
    int index;

public:
    DBPage(BufType cache, int index, bool parse = false);

    BufType operator[](const int offset) const;

    int getIndex();

    const static int PAGE_INFO_SLOT_OFFSET = 0;
};

class DBDataFileDescriptionPage: public DBPage
{
protected:
    DBDataFileDescriptionSlot* dfds;

public:
    DBDataFileDescriptionPage(BufType cache, int index, bool parse = false);

    void write();

    void addField(std::string name, int type);

    void printFileDescription();
};

class DBDataPage: public DBPage
{
};

#endif // DBPAGE_H_INCLUDED

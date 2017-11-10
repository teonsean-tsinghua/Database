#ifndef DBPAGE_H_INCLUDED
#define DBPAGE_H_INCLUDED

#include"DBPageInfoSlot.h"
#include"DBDataFileDescriptionSlot.h"
#include"../DBInclude.h"

class DBPage
{
protected:
    DBPageInfoSlot* pis;
    BufType cache;
    char* boundary;
    int index;

    const static char* const name_[];
;
public:
    DBPage(BufType cache, int index, int type, bool parse = false);

    BufType operator[](const int offset) const;

    int getIndex();

    const char* name(int type);

    virtual void print();

    const static int PAGE_INFO_SLOT_OFFSET = 0;

};

class DBDataFileDescriptionPage: public DBPage
{
protected:
    DBDataFileDescriptionSlot* dfds;

public:
    DBDataFileDescriptionPage(BufType cache, int index, bool parse = false);

    int addField(std::string name, int type);

    void print();
};

class DBDataPage: public DBPage
{
};

#endif // DBPAGE_H_INCLUDED

#ifndef DBINDEXINTERNALPAGE_H_INCLUDED
#define DBINDEXINTERNALPAGE_H_INCLUDED

#include"DBIndexNodePage.h"

class DBIndexInternalPage: public DBIndexNodePage
{

public:
    DBIndexInternalPage(BufType cache, int index, int pageID, int mode, int keyLength = -1);
};

#endif // DBINDEXINTERNALPAGE_H_INCLUDED

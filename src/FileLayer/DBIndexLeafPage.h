#ifndef DBINDEXLEAFPAGE_H_INCLUDED
#define DBINDEXLEAFPAGE_H_INCLUDED

#include"DBIndexNodePage.h"

class DBIndexLeafPage: public DBIndexNodePage
{

public:
    DBIndexLeafPage(BufType cache, int index, int pageID, int mode, int keyType = -1, int keyLength = -1);
};

#endif // DBINDEXLEAFPAGE_H_INCLUDED

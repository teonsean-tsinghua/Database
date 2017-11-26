#ifndef DBINDEXLEAFPAGE_H_INCLUDED
#define DBINDEXLEAFPAGE_H_INCLUDED

#include"DBIndexNodePage.h"

class DBIndexLeafPage: public DBIndexNodePage
{

public:
    DBIndexLeafPage(BufType cache, int index, int pageID, int mode, int keyLength = -1);
};

#endif // DBINDEXLEAFPAGE_H_INCLUDED

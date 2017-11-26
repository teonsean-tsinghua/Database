#ifndef DBDATABASE_H_INCLUDED
#define DBDATABASE_H_INCLUDED

#include"../FileLayer/DBDataFile.h"
#include"../FileLayer/../FileLayer/DBIndexFile.h"

class DBDataBase
{
private:
    static DBDataBase* instance;

    DBDataBase();

public:

    static DBDataBase* getInstance();

};

#endif // DBDATABASE_H_INCLUDED



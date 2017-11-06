#ifndef DBDATACONTAINER_H_INCLUDED
#define DBDATACONTAINER_H_INCLUDED

#include"DBIndexManager.h"

class DataBucket {

public:
    int cnt;
    int next;
    int data[];
    int position;

    DataBucket(const unsigned char* raw);

    const unsigned char* toString();
};

class DBDataContainer {
private:
    bool unique;
    int position;
    int index;
    DataBucket* first_bucket;
    DataBucket* current_bucket;
    DBIndexManager* indexManager;

public:
    DBDataContainer(DBIndexManager* indexManager, int position);

    DBDataContainer(DBIndexManager* indexManager, DataBucket* bucket);

    int getNext();
};

#endif // DBDATACONTAINER_H_INCLUDED

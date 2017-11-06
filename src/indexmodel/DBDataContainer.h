#ifndef DBDATACONTAINER_H_INCLUDED
#define DBDATACONTAINER_H_INCLUDED

#include"DBIndexManager.h"

class DataBucket {

public:
    int cnt;
    int next;
    int data[];
    int position;
    int volumn;

    DataBucket(int position, unsigned int volumn, const unsigned char* raw = NULL);

    const unsigned char* toString();
};

class DBDataContainer {
private:
    bool unique;
    int position;
    int index;
    int volumn;
    DataBucket* first;
    DataBucket* current;
    DBIndexManager* indexManager;

public:
    DBDataContainer(DBIndexManager* indexManager, int position);

    DBDataContainer(DBIndexManager* indexManager, DataBucket* bucket);

    int getNext();

    int insert(int position);

    int remove(int position, int& new_first_bucket);

    const static int NULL_POINTER = -1;
    const static int SUCCEED = 0;
    const static int EMPTY_BUCKET = 1;
    const static int NEXT_BUCKET = 2;
};

#endif // DBDATACONTAINER_H_INCLUDED

#ifndef BUCKETPAGE_H_INCLUDED
#define BUCKETPAGE_H_INCLUDED

#include"Page.h"

class Bucket
{
    int next;
    int pids[];

    static bool containForInsert(Bucket* b, int v, int d, bool& athere);

    static int search(Bucket* b, int v, int d);

    static int lastIdx(Bucket* b, int d);
};

class BucketPage: public Page
{
private:
    int density;

public:
    BucketPage(char* cache, int index, int pageID, bool parse, int density);

    int size();

    int cnt();

    bool full();

    bool available(int idx);

    void setAvailable(int idx, bool b);

    void print();

    Bucket* at(int idx);

    Bucket* allocateBucket(int& idx);

    static void test();

};

#endif // BUCKETPAGE_H_INCLUDED

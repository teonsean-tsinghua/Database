#ifndef BUCKETPAGE_H_INCLUDED
#define BUCKETPAGE_H_INCLUDED

#include"Page.h"

class Bucket
{
    int next;
    int pids[];
};

class BucketPage: public Page
{
private:
    int density;

public:
    BucketPage(char* cache, int index, int pageID, bool parse, int density = 1);

    int size();

    int cnt();

    Bucket* at(int idx);

    static void test();

};

#endif // BUCKETPAGE_H_INCLUDED

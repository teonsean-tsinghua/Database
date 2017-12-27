#ifndef INDEXFILEDESCPAGE_H_INCLUDED
#define INDEXFILEDESCPAGE_H_INCLUDED

#include"Page.h"

class IndexFileDescPage: public Page
{

public:
    IndexFileDescPage(char* cache, int index, int pageID, bool parse, int type = -1, int length = -1);

    void incrementPageNumber(int type);

    int getFirstLeafPage();

    int getFirstBucketPage();

    int getPageNumber();

    int getRootPage();

    int getKeyLength();

    int getKeyType();

    int getDensity();

    bool getUnique();

    void setFirstLeafPage(int n);

    void setFirstBucketPage(int n);

    void setPageNumber(int n);

    void setRootPage(int n);

    void setKeyLength(int n);

    void setKeyType(int n);

    void setDensity(int n);

    void setUnique(bool u);

    void print();

    const static int FIRST_LEAF_PAGE_OFFSET = PAGE_CONTENT_OFFSET;
    const static int PAGE_NUMBER_OFFSET = FIRST_LEAF_PAGE_OFFSET + sizeof(int);
    const static int ROOT_PAGE_OFFSET = PAGE_NUMBER_OFFSET + sizeof(int);
    const static int KEY_TYPE_OFFSET = ROOT_PAGE_OFFSET + sizeof(int);
    const static int KEY_LENGTH_OFFSET = KEY_TYPE_OFFSET + sizeof(int);
    const static int DENSITY_OFFSET = KEY_LENGTH_OFFSET + sizeof(int);
    const static int UNIQUE_OFFSET = DENSITY_OFFSET + sizeof(int);
    const static int FIRST_BUCKET_PAGE_OFFSET = UNIQUE_OFFSET + sizeof(int);
};

#endif // INDEXFILEDESCPAGE_H_INCLUDED

#ifndef BASEFILE_H_INCLUDED
#define BASEFILE_H_INCLUDED

#include"Page.h"
#include"FileIOModel.h"

class BaseFile
{
protected:
    int fileID;
    const FileIOModel* fm;

public:
    BaseFile() {};

    ~BaseFile() {};

    virtual int allocateUsagePage() {}

    virtual int allocateNodePage(bool isLeaf) {}

    virtual int allocateDataPage() {}

    virtual int allocateBucketPage() {}

    virtual void markAsUsable(int n) {}

    virtual void setRootPage(int n) {}

    virtual Page* openPage(int pid) {return NULL;}
};

#endif // BASEFILE_H_INCLUDED

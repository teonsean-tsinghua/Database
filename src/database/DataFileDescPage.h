#ifndef DATAFILEDESCPAGE_H_INCLUDED
#define DATAFILEDESCPAGE_H_INCLUDED

#include"Page.h"

class DataFileDescPage: public Page
{
protected:
    RecordInfo* ri;

public:
    DataFileDescPage(char* cache, int index, int pageID, bool parse, RecordInfo* ri);

    int getLastDataPage();

    int getFirstDataPage();

    int getFirstLeafPage();

    int getRootPage();

    int getPageNumber();

    int getFieldCount();

    int getPrimaryKeyCount();

    void setFirstDataPage(int n);

    void setLastDataPage(int n);

    void setFirstLeafPage(int n);

    void setPageNumber(int n);

    void setFieldCount(int n);

    void setPrimaryKeyCount(int n);

    void setRootPage(int n);

    void incrementPageNumber();

    void writeFields();

    void print();

    void updateFirstAvailable();

    static int maxRecordInfoLength();

    const static int FIRST_DATA_PAGE_OFFSET = PAGE_CONTENT_OFFSET;
    const static int LAST_DATA_PAGE_OFFSET = FIRST_DATA_PAGE_OFFSET + sizeof(int);
    const static int FIRST_LEAF_PAGE_OFFSET = LAST_DATA_PAGE_OFFSET + sizeof(int);
    const static int ROOT_PAGE_OFFSET = FIRST_LEAF_PAGE_OFFSET + sizeof(int);
    const static int PAGE_NUMBER_OFFSET = ROOT_PAGE_OFFSET + sizeof(int);
    const static int PRIMARY_KEY_COUNT_OFFSET = PAGE_NUMBER_OFFSET + sizeof(int);
    const static int FIELD_COUNT_OFFSET = PRIMARY_KEY_COUNT_OFFSET + sizeof(int);
    const static int RECORD_INFO_OFFSET = FIELD_COUNT_OFFSET + sizeof(int);

    const static int RECORD_INFO_TYPE_OFFSET = 0;
    const static int RECORD_INFO_NULLABLE_OFFSET = RECORD_INFO_TYPE_OFFSET + sizeof(int);
    const static int RECORD_INFO_EXTRA_OFFSET = RECORD_INFO_NULLABLE_OFFSET + sizeof(int);
    const static int RECORD_INFO_NAME_LENGTH_OFFSET = RECORD_INFO_EXTRA_OFFSET + sizeof(int);
    const static int RECORD_INFO_NAME_OFFSET = RECORD_INFO_NAME_LENGTH_OFFSET + sizeof(int);
};

#endif // DATAFILEDESCPAGE_H_INCLUDED

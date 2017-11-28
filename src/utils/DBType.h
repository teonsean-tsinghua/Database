#ifndef DBTYPE_H_INCLUDED
#define DBTYPE_H_INCLUDED

class DBType
{
    const static int typeSize_[];
    const static char* const typeName_[];
    const static char* const pageName_[];

public:
    const static int _ID = 0;   // _id: time stamp for first 8 bytes, a random number for next 4 bytes, cache address for next 4 bytes, pid for next 2 bytes, a rand number for last 2 bytes;
    const static int INT = _ID + 1;
    const static int FLOAT = INT + 1;
    const static int DATE = FLOAT + 1;
    const static int VARCHAR = DATE + 1;
    const static int FIRST_TYPE = _ID;
    const static int LAST_TYPE = VARCHAR;

    const static int DATA_FILE_DESCRIPTION_PAGE = 0;
    const static int DATA_PAGE = DATA_FILE_DESCRIPTION_PAGE + 1;
    const static int USAGE_PAGE = DATA_PAGE + 1;
    const static int INDEX_FILE_DESCRIPTION_PAGE = USAGE_PAGE + 1;
    const static int INDEX_INTERNAL_PAGE = INDEX_FILE_DESCRIPTION_PAGE + 1;
    const static int INDEX_LEAF_PAGE = INDEX_INTERNAL_PAGE + 1;
    const static int FIRST_PAGE_TYPE = DATA_FILE_DESCRIPTION_PAGE;
    const static int LAST_PAGE_TYPE = INDEX_LEAF_PAGE;

    static int typeSize(int type);

    static const char* typeName(int type);

    static const char* pageName(int type);
};

#endif // DBTYPE_H_INCLUDED

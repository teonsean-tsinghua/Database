#ifndef DBTYPE_H_INCLUDED
#define DBTYPE_H_INCLUDED

class DBType
{
    const static int size_[];
    const static char* const name_[];

public:
    const static int INT = 0;
#define FIRST INT


#define LAST INT

    static int size(int type);

    static const char* name(int type);
};

#endif // DBTYPE_H_INCLUDED

#ifndef DBFILEINFO_H_INCLUDED
#define DBFILEINFO_H_INCLUDED

#include<vector>
#include"DBRecordInfo.h"

class DBInfoPage
{
private:
    const DBRecordInfo& record_info;

    const int page_size;

    const int record_length;
    const int record_length_offset;

    int page_number;
    const int page_number_offset;

    int record_number;
    const int record_number_offset;

    char* availability_of_page;
    const int availability_of_page_offset;

    const int max_records_per_page;

public:
    DBInfoPage(const DBRecordInfo& record_info, int record_length);
};

class DBDataPage
{
private:
    const DBRecordInfo& record_info;

    const int record_length;
    const int page_id;
    const int total_slot;
    const int info_slot;
    const int record_slot;

    int record_number;
    int first_available;
    char* availability_of_page;

public:
    DBDataPage(const DBRecordInfo& record_info, int record_length, int page_id);
};
#endif // DBFILEINFO_H_INCLUDED

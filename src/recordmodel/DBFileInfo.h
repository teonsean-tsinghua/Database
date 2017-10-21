#ifndef DBFILEINFO_H_INCLUDED
#define DBFILEINFO_H_INCLUDED

#include<vector>

class DBFileInfo
{
private
    const int page_size = 8 * 1024;

    const int record_length;
    const int record_length_offset;

    int page_number;
    const int page_number_offset;

    int record_number;
    const int record_number_offset;

    int* record_number_of_page;
    const int record_number_of_page_offset;

    const int max_records_per_page;

public:
    DBFileInfo(int record_length);

    incrementPage();

    incrementRecordOnPage(int pageID);
};

#endif // DBFILEINFO_H_INCLUDED

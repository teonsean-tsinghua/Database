#include"DBFileInfo.h"

DBFileInfo::DBFileInfo(int record_length):
    record_length(record_length),
    record_length_offset(0),
    page_number(0),
    page_number_offset(record_length_offset + sizeof(int)),
    record_number(0),
    record_number_offset(page_number_offset + sizeof(int)),
    record_number_of_page_offset(record_number_offset + sizeof(int)),
    max_records_per_page(PAGE_SIZE / record_length - 1)
{

}

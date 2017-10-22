#include"DBFileInfo.h"
#define PAGE_SIZE 8 * 1024

DBInfoPageInfo::DBInfoPageInfo(const DBRecordInfo& record_info, int record_length):
    record_info(record_info),
    page_size(PAGE_SIZE),
    record_length(record_length),
    record_length_offset(0),
    page_number(0),
    page_number_offset(record_length_offset + sizeof(int)),
    record_number(0),
    record_number_offset(page_number_offset + sizeof(int)),
    availability_of_page_offset(record_number_offset + sizeof(int)),
    max_records_per_page(page_size / record_length - 1)
{

}

DBDataPageInfo::DBDataPageInfo(const DBRecordInfo& record_info, int record_length, int page_id):
    record_info(record_info),
    record_length(record_length),
    page_id(page_id),
    total_slot(PAGE_SIZE / record_length),
    info_slot((2 * total_slot + (8 * record_length + 1)) / (2 * (8 * record_length + 1))),
    record_slot(total_slot - info_slot)
{

}

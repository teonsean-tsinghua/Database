#include"DBRecordInfo.h"

int DBRecordInfo::getFieldCount()
{
    if(indexes.size() != names.size() ||
       names.size() != types.size() ||
       types.size() != offsets.size() ||
       offsets.size() != nullables.size())
    {
        return -1;
    }
    return indexes.size();
}

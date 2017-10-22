#include"DBRecordInfo.h"

const int DBRecordInfo::SIZEOF_TYPE[] = {10 * sizeof(char),
                                        sizeof(int),
                                        sizeof(long),
                                        sizeof(char),
                                        sizeof(char),
                                        sizeof(float),
                                        sizeof(double),
                                        256 * sizeof(char),
                                        1024 * sizeof(char)};
DBRecordInfo::DBRecordInfo()
{

}

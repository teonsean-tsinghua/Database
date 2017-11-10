#include"DBDataPage.h"

DBDataPage::DBDataPage(BufType cache, int index, int recordLength, int mode):
    DBPage(cache, index, DBType::DATA_PAGE, mode), recordLength(recordLength)
{

}

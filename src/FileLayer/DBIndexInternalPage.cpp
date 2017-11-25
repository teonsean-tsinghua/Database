#include"DBIndexInternalPage.h"

DBIndexInternalPage::DBIndexInternalPage(BufType cache, int index, int pageID, int mode, int keyLength):
    DBIndexNodePage(cache, index, pageID, DBType::INDEX_INTERNAL_PAGE, mode, keyLength)
{

}

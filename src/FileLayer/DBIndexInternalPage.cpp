#include"DBIndexInternalPage.h"

DBIndexInternalPage::DBIndexInternalPage(BufType cache, int index, int pageID, int mode, int keyType, int keyLength):
    DBIndexNodePage(cache, index, pageID, DBType::INDEX_INTERNAL_PAGE, mode, keyType, keyLength)
{

}

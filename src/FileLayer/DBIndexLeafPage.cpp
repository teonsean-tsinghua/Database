#include"DBIndexLeafPage.h"

DBIndexLeafPage::DBIndexLeafPage(BufType cache, int index, int pageID, int mode, int keyType, int keyLength):
    DBIndexNodePage(cache, index, pageID, DBType::INDEX_LEAF_PAGE, mode, keyType, keyLength)
{

}

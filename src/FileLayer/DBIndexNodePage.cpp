#include"DBIndexNodePage.h"

DBIndexNodePage::DBIndexNodePage(BufType cache, int index, int pageID, int type, int mode, int keyType):
    DBPage(cache, index, pageID, type, mode), keyType(keyType), keyLength(DBType::typeSize(keyType))
{
    ins = new DBIndexNodeSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], keyType);
    if(mode == MODE_CREATE)
    {
        ins->setChildrenCount(0);
        ins->setParentNode(0);
        pis->setLengthFixed(false);
        pis->setPageType(type);
        pis->setFirstAvailableByte(pis->size() + ins->size());
        pis->setNextSamePage(-1);
    }
}

void DBIndexNodePage::calcDegree(int& minDgr, int& maxDgr)
{
    int free = PAGE_SIZE - pis->size() - sizeof(int) * 2;
    maxDgr = free / (keyLength + sizeof(int));
    minDgr = maxDgr / 2;
}

int DBIndexNodePage::search(void* key)
{
    return ins->search(key);
}

int DBIndexNodePage::insert(void* key, int pid)
{
    int re = ins->insert(key, pid);
    if(re == SUCCEED)
    {
        pis->setFirstAvailableByte(pis->getFirstAvailableByte() + keyLength + sizeof(int));
    }
    return re;
}

void* DBIndexNodePage::getMaxKey()
{
    if(ins->getChildrenCount() <= 0)
    {
        return NULL;
    }
    return (void*)(ins->getMaxKey());
}

int DBIndexNodePage::getMaxPage()
{
    if(ins->getChildrenCount() <= 0)
    {
        return -1;
    }
    return ins->getPageOfIndex(ins->getChildrenCount() - 1);
}

bool DBIndexNodePage::isLeaf()
{
    return pis->getPageType() == DBType::INDEX_LEAF_PAGE;
}

void DBIndexNodePage::setParent(int pid)
{
    ins->setParentNode(pid);
}

void DBIndexNodePage::print()
{
    DBPage::print();
    if(pis->getPageType() == DBType::INDEX_INTERNAL_PAGE)
    {
        DBPrintLine("This is an internal node.");
    }
    else if(pis->getPageType() == DBType::INDEX_LEAF_PAGE)
    {
        DBPrintLine("This is a leaf node.");
    }
    DBPrint("Keys are of length ");
    DBPrintLine(keyLength);
    ins->print();
}

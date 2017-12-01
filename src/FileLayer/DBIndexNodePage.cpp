#include"DBIndexNodePage.h"

DBIndexNodePage::DBIndexNodePage(BufType cache, int index, int pageID, int type, int mode, int keyType, int keyLength):
    DBPage(cache, index, pageID, type, mode), keyType(keyType), keyLength(keyLength)
{
    ins = new DBIndexNodeSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], keyType, keyLength);
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

void DBIndexNodePage::split(DBIndexNodePage* src, DBIndexNodePage* dest)
{
    int cnt = src->getChildrenCount();
    int mid = cnt / 2;
    int len = (cnt - mid) * (sizeof(int) + src->keyLength);
    copyData(src->ins->getKeyOfIndex(mid), dest->ins->getKeyOfIndex(0), len);
    src->ins->setChildrenCount(mid);
    src->pis->setFirstAvailableByte(src->pis->size() + src->ins->size());
    dest->ins->setChildrenCount(cnt - mid);
    dest->pis->setFirstAvailableByte(dest->pis->size() + dest->ins->size());
}

void DBIndexNodePage::update(void* key, int pid)
{
    ins->update(key, pid);
}

int DBIndexNodePage::search(void* key)
{
    return ins->search(key);
}

int DBIndexNodePage::searchEqual(void* key)
{
    return ins->searchEqual(key);
}

void DBIndexNodePage::setMaxKey(void* key)
{
    ins->setKeyOfIndex(ins->getChildrenCount() - 1, key);
}

void DBIndexNodePage::insert(void* key, int pid)
{
    ins->insert(key, pid);
    pis->setFirstAvailableByte(pis->getFirstAvailableByte() + keyLength + sizeof(int));
}

void DBIndexNodePage::remove(void* key)
{
    ins->remove(key);
    pis->setFirstAvailableByte(pis->getFirstAvailableByte() - keyLength - sizeof(int));
}

void DBIndexNodePage::changeKeyOfPage(int page, void* key)
{
    int cnt = ins->getChildrenCount();
    for(int i = 0; i < cnt; i++)
    {
        if(ins->getPageOfIndex(i) == page)
        {
            ins->setKeyOfIndex(i, key);
            return;
        }
    }
}

int DBIndexNodePage::getChildrenCount()
{
    return ins->getChildrenCount();
}

void* DBIndexNodePage::getMaxKey()
{
    if(ins->getChildrenCount() <= 0)
    {
        return NULL;
    }
    return (void*)(ins->getMaxKey());
}

int DBIndexNodePage::getMinPage()
{
    if(ins->getChildrenCount() <= 0)
    {
        return -1;
    }
    return ins->getPageOfIndex(0);
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

int DBIndexNodePage::getParent()
{
    return ins->getParentNode();
}

void DBIndexNodePage::print()
{
    DBPage::print();
    if(pis->getPageType() == DBType::INDEX_INTERNAL_PAGE)
    {
        DBPrint::printLine("This is an internal node.");
    }
    else if(pis->getPageType() == DBType::INDEX_LEAF_PAGE)
    {
        DBPrint::printLine("This is a leaf node.");
    }
    DBPrint::print("Keys are of length ").printLine(keyLength);
    ins->print();
}

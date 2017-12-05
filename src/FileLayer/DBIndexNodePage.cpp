#include"DBIndexNodePage.h"

BufType DBIndexNodePage::buffer = (BufType)(new char[PAGE_SIZE]);

DBIndexNodePage::DBIndexNodePage(BufType cache, int index, int pageID, int type, int mode, int keyType, int keyLength):
    DBPage(cache, index, pageID, type, mode), keyType(keyType), keyLength(keyLength)
{
    ins = new DBIndexNodeSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()]);
    if(mode == MODE_CREATE)
    {
        setChildrenCount(0);
        setParentNode(0);
        pis->setLengthFixed(false);
        pis->setPageType(type);
        pis->setNextSamePage(-1);
        updateFirstAvailable();
    }
}

void DBIndexNodePage::updateFirstAvailable()
{
    pis->setFirstAvailableByte(pis->size() + sizeof(int) * 2 + (keyLength + sizeof(int)) * getChildrenCount());
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
    copyData(src->getKeyOfIndex(mid), dest->getKeyOfIndex(0), len);
    src->setChildrenCount(mid);
    src->updateFirstAvailable();
    dest->setChildrenCount(cnt - mid);
    dest->updateFirstAvailable();
}

void DBIndexNodePage::update(void* key, int pid)
{
    int cnt = getChildrenCount();
    if(getChildrenCount() < 0)
    {
        throw Exception("Invalid children count of index node.");
    }
    for(int i = 0; i < cnt; i++)
    {
        if(Equal(key, getKeyOfIndex(i), keyType, keyLength))
        {
            setPageOfIndex(i, pid);
        }
    }
}

int DBIndexNodePage::search(void* key)
{
    int cnt = getChildrenCount();
    if(getChildrenCount() < 0)
    {
        throw Exception("Invalid children count of index node.");
    }
    if(larger(key, getMaxKey(), keyType, keyLength))
    {
        return -1;
    }
    int i;
    for(i = 0; i < cnt; i++)
    {
        if(!larger(key, getKeyOfIndex(i), keyType, keyLength))
        {
            break;
        }
    }
    return getPageOfIndex(i);
}

int DBIndexNodePage::searchEqual(void* key)
{
    int cnt = getChildrenCount();
    if(getChildrenCount() < 0)
    {
        throw Exception("Invalid children count of index node.");
    }
    for(int i = 0; i < cnt; i++)
    {
        if(Equal(key, getKeyOfIndex(i), keyType, keyLength))
        {
            return getPageOfIndex(i);
        }
    }
    return -1;
}

void DBIndexNodePage::setMaxKey(void* key)
{
    setKeyOfIndex(getChildrenCount() - 1, key);
}

void DBIndexNodePage::insert(void* key, int pid)
{
    if(searchEqual(key) > 0)
    {
        //TODO:
        throw Exception("Attempted to insert duplicate key into unique index.");
    }
    int cnt = getChildrenCount();
    if(getChildrenCount() < 0)
    {
        throw Exception("Invalid children count of index node.");
    }
    if(cnt == 0 || larger(key, getMaxKey(), keyType, keyLength))
    {
        setPageOfIndex(cnt, pid);
        setKeyOfIndex(cnt, key);
        setChildrenCount(cnt + 1);
    }
    int i;
    for(i = 0; i < cnt; i++)
    {
        if(!larger(key, getKeyOfIndex(i), keyType, keyLength))
        {
            break;
        }
    }
    int len = (cnt - i) * (sizeof(int) + keyLength);
    copyData(getKeyOfIndex(i), buffer, len);
    copyData(buffer, getKeyOfIndex(i + 1), len);
    setKeyOfIndex(i, key);
    setPageOfIndex(i, pid);
    setChildrenCount(cnt + 1);
    updateFirstAvailable();
}

void DBIndexNodePage::remove(void* key)
{
    int cnt = getChildrenCount();
    if(getChildrenCount() < 0)
    {
        throw Exception("Invalid children count of index node.");
    }
    for(int i = 0; i < cnt; i++)
    {
        if(Equal(key, getKeyOfIndex(i), keyType, keyLength))
        {
            int len = (cnt - i - 1) * (sizeof(int) + keyLength);
            copyData(getKeyOfIndex(i + 1), buffer, len);
            copyData(buffer, getKeyOfIndex(i), len);
            setChildrenCount(cnt - 1);
            break;
        }
    }
    updateFirstAvailable();
}

void DBIndexNodePage::changeKeyOfPage(int page, void* key)
{
    int cnt = getChildrenCount();
    for(int i = 0; i < cnt; i++)
    {
        if(getPageOfIndex(i) == page)
        {
            setKeyOfIndex(i, key);
            return;
        }
    }
}

int DBIndexNodePage::getChildrenCount()
{
    int re;
	readInt((*ins)[CHILDREN_COUNT_OFFSET], &re);
	return re;
}

void DBIndexNodePage::setChildrenCount(int n){
	writeInt((*ins)[CHILDREN_COUNT_OFFSET], n);
}

int DBIndexNodePage::getPageOfIndex(int index)
{
    int re;
    readInt((*ins)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength], &re);
    return re;
}

void DBIndexNodePage::setPageOfIndex(int index, int pid)
{
    writeInt((*ins)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength], pid);
}

BufType DBIndexNodePage::getKeyOfIndex(int index)
{
    return (*ins)[DATA_OFFSET + index * (keyLength + sizeof(int))];
}

void DBIndexNodePage::setKeyOfIndex(int index, void* key)
{
    writeData((*ins)[DATA_OFFSET + index * (keyLength + sizeof(int))], (char*)key, keyLength);
}

void* DBIndexNodePage::getMaxKey()
{
    if(getChildrenCount() <= 0)
    {
        return NULL;
    }
    return (*ins)[DATA_OFFSET + (sizeof(int) + keyLength) * (getChildrenCount() - 1)];
}

void* DBIndexNodePage::getMinKey()
{
    return (*ins)[DATA_OFFSET];
}

int DBIndexNodePage::getMinPage()
{
    if(getChildrenCount() <= 0)
    {
        return -1;
    }
    return getPageOfIndex(0);
}

int DBIndexNodePage::getMaxPage()
{
    if(getChildrenCount() <= 0)
    {
        return -1;
    }
    return getPageOfIndex(getChildrenCount() - 1);
}

bool DBIndexNodePage::isLeaf()
{
    return pis->getPageType() == DBType::INDEX_LEAF_PAGE;
}

void DBIndexNodePage::setParentNode(int pid)
{
    writeInt((*ins)[PARENT_NODE_OFFSET], pid);
}

int DBIndexNodePage::getParentNode()
{
    int re;
	readInt((*ins)[PARENT_NODE_OFFSET], &re);
	return re;
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
    DBPrint::print("Parent node is at ").printLine(getParentNode())
            .print("This node has children of number ").printLine(getChildrenCount());
    for(int i = 0; i < getChildrenCount(); i++)
    {
        BufType key = getKeyOfIndex(i);
        int page = getPageOfIndex(i);
        int re;
        readInt(key, &re);
        DBPrint::log("Key: ").log(re).log(", page: ").logLine(page);
    }
}

DBIndexNodePage::DBIndexNodeSlot::DBIndexNodeSlot(BufType cache):
    DBSlot(cache)
{
};

DBIndexInternalPage::DBIndexInternalPage(BufType cache, int index, int pageID, int mode, int keyType, int keyLength):
    DBIndexNodePage(cache, index, pageID, DBType::INDEX_INTERNAL_PAGE, mode, keyType, keyLength)
{
}

DBIndexLeafPage::DBIndexLeafPage(BufType cache, int index, int pageID, int mode, int keyType, int keyLength):
    DBIndexNodePage(cache, index, pageID, DBType::INDEX_LEAF_PAGE, mode, keyType, keyLength)
{
}

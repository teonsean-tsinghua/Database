#include"IndexNodePage.h"

BufType IndexNodePage::buffer = (BufType)(new char[PAGE_SIZE]);

IndexNodePage::IndexNodePage(BufType cache, int index, int pageID, int type, bool parse, int keyType, int keyLength):
    Page(cache, index, pageID, type), keyType(keyType), keyLength(keyLength)
{
    ins = new IndexNodeSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()]);
    if(!parse)
    {
        setChildrenCount(0);
        setParentNode(0);
        pis->setLengthFixed(false);
        pis->setPageType(type);
        pis->setNextSamePage(-1);
        updateFirstAvailable();
    }
}

void IndexNodePage::updateFirstAvailable()
{
    pis->setFirstAvailableByte(pis->size() + sizeof(int) * 2 + (keyLength + sizeof(int)) * getChildrenCount());
}

void IndexNodePage::calcDegree(int& minDgr, int& maxDgr)
{
    int free = PAGE_SIZE - pis->size() - sizeof(int) * 2;
    maxDgr = free / (keyLength + sizeof(int));
    minDgr = maxDgr / 2;
}

void IndexNodePage::split(IndexNodePage* src, IndexNodePage* dest)
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

void IndexNodePage::update(void* key, int pid)
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

int IndexNodePage::search(void* key)
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

int IndexNodePage::searchEqual(void* key)
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

void IndexNodePage::setMaxKey(void* key)
{
    setKeyOfIndex(getChildrenCount() - 1, key);
}

void IndexNodePage::insert(void* key, int pid)
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

void IndexNodePage::remove(void* key)
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

void IndexNodePage::changeKeyOfPage(int page, void* key)
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

int IndexNodePage::getChildrenCount()
{
    int re;
	readInt((*ins)[CHILDREN_COUNT_OFFSET], &re);
	return re;
}

void IndexNodePage::setChildrenCount(int n){
	writeInt((*ins)[CHILDREN_COUNT_OFFSET], n);
}

int IndexNodePage::getPageOfIndex(int index)
{
    int re;
    readInt((*ins)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength], &re);
    return re;
}

void IndexNodePage::setPageOfIndex(int index, int pid)
{
    writeInt((*ins)[DATA_OFFSET + index * (keyLength + sizeof(int)) + keyLength], pid);
}

BufType IndexNodePage::getKeyOfIndex(int index)
{
    return (*ins)[DATA_OFFSET + index * (keyLength + sizeof(int))];
}

void IndexNodePage::setKeyOfIndex(int index, void* key)
{
    writeData((*ins)[DATA_OFFSET + index * (keyLength + sizeof(int))], (char*)key, keyLength);
}

void* IndexNodePage::getMaxKey()
{
    if(getChildrenCount() <= 0)
    {
        return NULL;
    }
    return (*ins)[DATA_OFFSET + (sizeof(int) + keyLength) * (getChildrenCount() - 1)];
}

void* IndexNodePage::getMinKey()
{
    return (*ins)[DATA_OFFSET];
}

int IndexNodePage::getMinPage()
{
    if(getChildrenCount() <= 0)
    {
        return -1;
    }
    return getPageOfIndex(0);
}

int IndexNodePage::getMaxPage()
{
    if(getChildrenCount() <= 0)
    {
        return -1;
    }
    return getPageOfIndex(getChildrenCount() - 1);
}

bool IndexNodePage::isLeaf()
{
    return pis->getPageType() == Type::INDEX_LEAF_PAGE;
}

void IndexNodePage::setParentNode(int pid)
{
    writeInt((*ins)[PARENT_NODE_OFFSET], pid);
}

int IndexNodePage::getParentNode()
{
    int re;
	readInt((*ins)[PARENT_NODE_OFFSET], &re);
	return re;
}

void IndexNodePage::print()
{
    Page::print();
    if(pis->getPageType() == Type::INDEX_INTERNAL_PAGE)
    {
        Print::printLine("This is an internal node.");
    }
    else if(pis->getPageType() == Type::INDEX_LEAF_PAGE)
    {
        Print::printLine("This is a leaf node.");
    }
    Print::print("Keys are of length ").printLine(keyLength);
    Print::print("Parent node is at ").printLine(getParentNode())
            .print("This node has children of number ").printLine(getChildrenCount());
    for(int i = 0; i < getChildrenCount(); i++)
    {
        BufType key = getKeyOfIndex(i);
        int page = getPageOfIndex(i);
        int re;
        readInt(key, &re);
        Print::log("Key: ").log(re).log(", page: ").logLine(page);
    }
}

IndexNodePage::IndexNodeSlot::IndexNodeSlot(BufType cache):
    Slot(cache)
{
};

IndexInternalPage::IndexInternalPage(BufType cache, int index, int pageID, int mode, int keyType, int keyLength):
    IndexNodePage(cache, index, pageID, Type::INDEX_INTERNAL_PAGE, mode, keyType, keyLength)
{
}

IndexLeafPage::IndexLeafPage(BufType cache, int index, int pageID, int mode, int keyType, int keyLength):
    IndexNodePage(cache, index, pageID, Type::INDEX_LEAF_PAGE, mode, keyType, keyLength)
{
}

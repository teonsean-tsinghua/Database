#include"IndexFileDescriptionPage.h"

IndexFileDescriptionPage::IndexFileDescriptionPage(BufType cache, int index, int pageID, bool parse, int type, int length):
    Page(cache, index, pageID, Type::INDEX_FILE_DESCRIPTION_PAGE)
{
    ifds = new IndexFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()]);
    if(!parse)
    {
        pis->setPageType(Type::INDEX_FILE_DESCRIPTION_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(false);
        pis->setFirstAvailableByte(pis->size() + ifds->size());
        setFirstLeafPage(-1);
        setPageNumber(1);
        setKeyType(type);
        setKeyLength(length);
        setRootPage(-1);
    }
}

void IndexFileDescriptionPage::incrementPageNumber()
{
    setPageNumber(getPageNumber() + 1);
}

void IndexFileDescriptionPage::print()
{
    Page::print();
    Print::print("This file has pages of number ").printLine(getPageNumber())
            .print("First leaf page is at ").printLine(getFirstLeafPage())
            .print("Root page is at ").printLine(getRootPage())
            .print("Keys are of type ").printLine(Type::typeName(getKeyType()));
}

IndexFileDescriptionPage::IndexFileDescriptionSlot::IndexFileDescriptionSlot(BufType cache):
    Slot(cache)
{

}

int IndexFileDescriptionPage::IndexFileDescriptionSlot::size()
{
    return sizeof(int) * 4;
}

int IndexFileDescriptionPage::getFirstLeafPage()
{
    int re;
    readInt((*ifds)[FIRST_LEAF_PAGE_OFFSET], &re);
    return re;
}

int IndexFileDescriptionPage::getPageNumber()
{
    int re;
    readInt((*ifds)[PAGE_NUMBER_OFFSET], &re);
    return re;
}

int IndexFileDescriptionPage::getRootPage()
{
    int re;
    readInt((*ifds)[ROOT_PAGE_OFFSET], &re);
    return re;
}

int IndexFileDescriptionPage::getKeyType()
{
    int re;
    readInt((*ifds)[KEY_TYPE_OFFSET], &re);
    return re;
}

int IndexFileDescriptionPage::getKeyLength()
{
    int re;
    readInt((*ifds)[KEY_LENGTH_OFFSET], &re);
    return re;
}

void IndexFileDescriptionPage::setFirstLeafPage(int n)
{
    writeInt((*ifds)[FIRST_LEAF_PAGE_OFFSET], n);
}

void IndexFileDescriptionPage::setPageNumber(int n)
{
    writeInt((*ifds)[PAGE_NUMBER_OFFSET], n);
}

void IndexFileDescriptionPage::setRootPage(int n)
{
    writeInt((*ifds)[ROOT_PAGE_OFFSET], n);
}

void IndexFileDescriptionPage::setKeyType(int n)
{
    writeInt((*ifds)[KEY_TYPE_OFFSET], n);
}

void IndexFileDescriptionPage::setKeyLength(int n)
{
    writeInt((*ifds)[KEY_LENGTH_OFFSET], n);
}

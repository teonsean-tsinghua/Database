#include"IndexFileDescPage.h"

IndexFileDescPage::IndexFileDescPage(char* cache, int index, int pageID, bool parse, int type, int length):
    Page(cache, index, pageID)
{
    if(!parse)
    {
        setPageType(Type::INDEX_FILE_DESC_PAGE);
        setNextSamePage(-1);
        setPrevSamePage(-1);
        setFirstLeafPage(-1);
        setFirstBucketPage(-1);
        setPageNumber(1);
        setKeyType(type);
        setKeyLength(length);
        setRootPage(-1);
        setFirstAvailableByte(FIRST_BUCKET_PAGE_OFFSET + sizeof(int));
    }
    else
    {
        assert(getPageType() == Type::INDEX_FILE_DESC_PAGE);
    }
}

void IndexFileDescPage::incrementPageNumber()
{
    setPageNumber(getPageNumber() + 1);
}

void IndexFileDescPage::print()
{
    Page::print();
    std::cout << "Page number:                      " << getPageNumber() << std::endl;
    std::cout << "First leaf page:                  " << getFirstLeafPage() << std::endl;
    std::cout << "First bucket page:                " << getFirstBucketPage() << std::endl;
    std::cout << "Last bucket page:                 " << getLastBucketPage() << std::endl;
    std::cout << "Root page:                        " << getRootPage() << std::endl;
    std::cout << "Key type:                         " << Type::typeName(getKeyType()) << std::endl;
    std::cout << "Density level:                    " << getDensity() << std::endl;
    std::cout << "Unique:                           " << (getUnique() ? "true" : "false") << std::endl;
}

int IndexFileDescPage::getFirstLeafPage()
{
    return readInt((*this)[FIRST_LEAF_PAGE_OFFSET]);
}

int IndexFileDescPage::getFirstBucketPage()
{
    return readInt((*this)[FIRST_BUCKET_PAGE_OFFSET]);
}

int IndexFileDescPage::getLastBucketPage()
{
    return readInt((*this)[LAST_BUCKET_PAGE_OFFSET]);
}

int IndexFileDescPage::getPageNumber()
{
    return readInt((*this)[PAGE_NUMBER_OFFSET]);
}

int IndexFileDescPage::getRootPage()
{
    return readInt((*this)[ROOT_PAGE_OFFSET]);
}

int IndexFileDescPage::getKeyType()
{
    return readInt((*this)[KEY_TYPE_OFFSET]);
}

int IndexFileDescPage::getKeyLength()
{
    return readInt((*this)[KEY_LENGTH_OFFSET]);
}

int IndexFileDescPage::getDensity()
{
    return readInt((*this)[DENSITY_OFFSET]);
}

bool IndexFileDescPage::getUnique()
{
    return readInt((*this)[UNIQUE_OFFSET]) == 1;
}

void IndexFileDescPage::setFirstLeafPage(int n)
{
    writeInt((*this)[FIRST_LEAF_PAGE_OFFSET], n);
}

void IndexFileDescPage::setFirstBucketPage(int n)
{
    writeInt((*this)[FIRST_BUCKET_PAGE_OFFSET], n);
}

void IndexFileDescPage::setLastBucketPage(int n)
{
    writeInt((*this)[LAST_BUCKET_PAGE_OFFSET], n);
}

void IndexFileDescPage::setPageNumber(int n)
{
    writeInt((*this)[PAGE_NUMBER_OFFSET], n);
}

void IndexFileDescPage::setRootPage(int n)
{
    writeInt((*this)[ROOT_PAGE_OFFSET], n);
}

void IndexFileDescPage::setKeyType(int n)
{
    writeInt((*this)[KEY_TYPE_OFFSET], n);
}

void IndexFileDescPage::setKeyLength(int n)
{
    writeInt((*this)[KEY_LENGTH_OFFSET], n);
}

void IndexFileDescPage::setDensity(int n)
{
    writeInt((*this)[DENSITY_OFFSET], n);
}

void IndexFileDescPage::setUnique(bool u)
{
    writeInt((*this)[UNIQUE_OFFSET], u ? 1 : 0);
}


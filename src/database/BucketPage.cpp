#include"BucketPage.h"

static int bucket_sizes[] = {10, 32, 50, 72};
static int bucket_cnts[] = {185, 63, 40, 28};

BucketPage::BucketPage(char* cache, int index, int pageID, bool parse, int density):
    Page(cache, index, pageID), density(density)
{
    assert(bucket_cnts[density] * (bucket_sizes[density] + 1) + PAGE_CONTENT_OFFSET <= PAGE_SIZE);
    assert(density >= 0 && density < 4);
    if(parse)
    {
        assert(getPageType() == Type::BUCKET_PAGE);
    }
    else
    {
        setFirstAvailableByte(PAGE_CONTENT_OFFSET);
        setPageType(Type::BUCKET_PAGE);
        setNextSamePage(-1);
        setLengthFixed(true);
        memset((*this)[PAGE_CONTENT_OFFSET], 0, PAGE_SIZE - PAGE_CONTENT_OFFSET);
    }
}

void BucketPage::print()
{
    Page::print();
}

Bucket* BucketPage::at(int idx)
{
    assert(idx >= 0 && idx < cnt());
    return (Bucket*)(*this)[PAGE_CONTENT_OFFSET + size() * idx];
}

int BucketPage::cnt()
{
    return bucket_cnts[density];
}

int BucketPage::size()
{
    return bucket_sizes[density];
}

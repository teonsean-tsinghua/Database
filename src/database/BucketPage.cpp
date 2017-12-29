#include"BucketPage.h"

static int bucket_sizes[] = {10, 32, 50, 72};
static int bucket_cnts[] = {181, 61, 39, 27};

bool Bucket::containForInsert(Bucket* b, int v, int d, bool& athere)
{
    int volumn = bucket_sizes[d];
    for(int i = 0; i < volumn; i++)
    {
        if(v == b->pids[i])
        {
            athere = true;
            return true;
        }
        if(b->pids[i] == 0)
        {
            b->pids[i] = v;
            athere = false;
            return true;
        }
    }
    return false;
}

void Bucket::print(Bucket* b, int d)
{
    int volumn = bucket_sizes[d];
    std::cout << b << ":[";
    for(int i = 0; i < volumn; i++)
    {
        if(b->pids[i] > 0)
        {
            std::cout << b->pids[i] << ", ";
        }
        else
        {
            break;
        }
    }
    std::cout << "]\n";
    return;
}

int Bucket::lastIdx(Bucket* b, int d)
{
    int volumn = bucket_sizes[d];
    for(int i = 0; i < volumn; i++)
    {
        if(b->pids[i] == 0)
        {
            return i - 1;
        }
    }
    return volumn - 1;
}

int Bucket::search(Bucket* b, int v, int d)
{
    int volumn = bucket_sizes[d];
    for(int i = 0; i < volumn; i++)
    {
        if(v == b->pids[i])
        {
            return i;
        }
    }
    return -1;
}

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
        setPrevSamePage(-1);
        memset((*this)[PAGE_CONTENT_OFFSET], 0, PAGE_SIZE - PAGE_CONTENT_OFFSET);
        for(int i = 0; i < cnt(); i++)
        {
            writeBool((*this)[PAGE_CONTENT_OFFSET + i], true);
        }
    }
}

bool BucketPage::full()
{
    for(int i = 0; i < cnt(); i++)
    {
        if(available(i))
        {
            return false;
        }
    }
    return true;
}

bool BucketPage::available(int idx)
{
    return readBool((*this)[PAGE_CONTENT_OFFSET + idx]);
}

void BucketPage::setAvailable(int idx, bool b)
{
    writeBool((*this)[PAGE_CONTENT_OFFSET + idx], b);
}

Bucket* BucketPage::allocateBucket(int& idx)
{
    for(idx = 0; idx < cnt(); idx++)
    {
        if(available(idx))
        {
            writeBool((*this)[PAGE_CONTENT_OFFSET + idx], false);
            return at(idx);
        }
    }
    return NULL;
}

void BucketPage::print()
{
    Page::print();
}

Bucket* BucketPage::at(int idx)
{
    assert(idx >= 0 && idx < cnt());
    return (Bucket*)(*this)[PAGE_CONTENT_OFFSET + cnt() + (size() + 1) * idx * sizeof(int)];
}

int BucketPage::cnt()
{
    return bucket_cnts[density];
}

int BucketPage::size()
{
    return bucket_sizes[density];
}


#include"DBPage.h"

DBPage::DBPage(BufType cache):
    cache(cache)
{

}

BufType DBPage::operator[](const int offset) const
{
    return (BufType)((char*)cache + offset);
}


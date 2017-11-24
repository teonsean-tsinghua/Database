#include"DBIndexFileDescriptionPage.h"

DBIndexFileDescriptionPage::DBIndexFileDescriptionPage(BufType cache, int index, int pageID, int mode, int type):
    DBPage(cache, index, pageID, DBType::DATA_FILE_DESCRIPTION_PAGE, mode)
{
    ifds = new DBIndexFileDescriptionSlot((*this)[PAGE_INFO_SLOT_OFFSET + pis->size()], mode);
    if(mode == MODE_CREATE)
    {
        pis->setPageType(DBType::INDEX_FILE_DESCRIPTION_PAGE);
        pis->setNextSamePage(-1);
        pis->setLengthFixed(false);
        pis->setFirstAvailableByte(pis->size() + ifds->size());
        ifds->setFirstLeafPage(-1);
        ifds->setPageNumber(1);
        ifds->setKeyType(type);
        ifds->setRootPage(-1);
    }
}

void DBIndexFileDescriptionPage::incrementPageNumber()
{
    ifds->setPageNumber(ifds->getPageNumber() + 1);
}

void DBIndexFileDescriptionPage::setFirstLeafPage(int pid)
{
    ifds->setFirstLeafPage(pid);
}

void DBIndexFileDescriptionPage::setRootPage(int pid)
{
    ifds->setRootPage(pid);
}

int DBIndexFileDescriptionPage::getFirstLeafPage()
{
    return ifds->getFirstLeafPage();
}

int DBIndexFileDescriptionPage::getPageNumber()
{
    return ifds->getPageNumber();
}

int DBIndexFileDescriptionPage::getRootPage()
{
    return ifds->getRootPage();
}

void DBIndexFileDescriptionPage::print()
{
    DBPage::print();
    ifds->print();
}

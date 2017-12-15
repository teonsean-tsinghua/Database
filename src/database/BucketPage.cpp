#include"BucketPage.h"

BucketPage::BucketPage(BufType cache, int index, int pageID, bool parse):
	Page(cache, index, pageID, Type::BUCKET_PAGE)
{
	if(!parse)
	{
        pis->setLengthFixed(true);
        pis->setPageType(Type::BUCKET_PAGE);
        pis->setNextSamePage(-1);
        pis->setFirstAvailableByte(pis->size());
		memset((char*)cache + pis->size(), 0, PAGE_SIZE - pis->size());
	}
	else
	{
		int i = 0;
		int offset = pis->size();
		while(offset < pis->getFirstAvailableByte())
		{
			slots.push_back(new BucketSlot((*this)[offset]));
			offset += BucketSlot::size();
		}
	}
}

int BucketPage::allocateNewSlot()
{
	if(pis->getFirstAvailableByte() == PAGE_SIZE)
	{
		return -1;
	}
	slots.push_back(new BucketSlot((*this)[pis->getFirstAvailableByte()]));
	memset((char*)(*(slots.back()))[0], 0, BucketSlot::size());
	pis->setFirstAvailableByte(pis->getFirstAvailableByte() + BucketSlot::size());
	return slots.size() - 1;
}

BucketPage::BucketSlot* BucketPage::getSlot(int i)
{
	if(i < 0 || i >= slots.size())
	{
		return NULL;
	}
	return slots[i];
}

int BucketPage::isAvailable(int i)
{
	BucketSlot* slot = getSlot(i);
	if(slot == NULL)
	{
		return -1;
	}
	return slot->findAvailable();
}

int BucketPage::getNext(int i)
{
	BucketSlot* slot = getSlot(i);
	if(slot == NULL)
	{
		return -1;
	}
	int re;
	readInt((*slot)[BUCKET_SIZE * sizeof(int)], &re);
	return re;
}

void BucketPage::setNext(int i, int pid)
{
	BucketSlot* slot = getSlot(i);
	if(slot == NULL)
	{
		return;
	}
	writeInt((*slot)[BUCKET_SIZE * sizeof(int)], pid);
}

BucketPage::BucketSlot::BucketSlot(BufType cache):
	Slot(cache)
{

}

int BucketPage::BucketSlot::findAvailable()
{
	int* n = (int*)cache;
	for(int i = 0; i < BUCKET_SIZE; i++)
	{
		if(*(n + i) == 0)
		{
			return i;
		}
	}
	return -1;
}

int BucketPage::BucketSlot::size()
{
	return sizeof(int) * (1 + BUCKET_SIZE);
}
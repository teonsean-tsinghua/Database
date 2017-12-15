#ifndef BUCKETPAGE_H_INCLUDED
#define BUCKETPAGE_H_INCLUDED

#define BUCKET_SIZE 50
#define BUCKET_NUMBER 20

#include"Page.h"
#include"Slot.h"
#include"Include.h"

class BucketPage: public Page
{
protected:

	class BucketSlot: public Slot
	{
	public:
		BucketSlot(BufType cache);

		static int size();

		int findAvailable();

	};

	std::vector<BucketSlot*> slots;

public:
	BucketPage(BufType cache, int index, int pageID, bool parse);

	BucketSlot* getSlot(int i);

	int isAvailable(int i);

	int getNext(int i);

	void setNext(int i, int pid);

	int allocateNewSlot();
};

#endif // BUCKETPAGE_H_INCLUDED
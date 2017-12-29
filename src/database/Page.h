#ifndef PAGE_H_INCLUDED
#define PAGE_H_INCLUDED

#include"Include.h"

class Page
{
protected:
    char* cache;
    int index;
    int pageID;

public:
    Page(char* cache, int index, int pageID);

    char* operator[](const int offset) const;

    int getIndex();

    int getPageID();

    virtual void print();

	int getPageType();

	int getFirstAvailableByte();

	int getPrevSamePage();

	int getNextSamePage();

	void setPageType(int n);

	void setFirstAvailableByte(int n);

	void setLengthFixed(bool n);

	void setNextSamePage(int n);

	void setPrevSamePage(int n);

	static int getPageTypeStatik(char* buf);

	const static int PAGE_TYPE_OFFSET = 0;
	const static int FIRST_AVAILABLE_BYTE_OFFSET = PAGE_TYPE_OFFSET + sizeof(int);
	const static int PREV_SAME_PAGE_OFFSET = FIRST_AVAILABLE_BYTE_OFFSET + sizeof(int);
	const static int NEXT_SAME_PAGE_OFFSET = PREV_SAME_PAGE_OFFSET + sizeof(int);

    const static int PAGE_CONTENT_OFFSET = NEXT_SAME_PAGE_OFFSET + sizeof(int);

};

#endif // PAGE_H_INCLUDED

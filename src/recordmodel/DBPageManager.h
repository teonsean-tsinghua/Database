#ifndef DBPAGEMANAGER_H_INCLUDED
#define DBPAGEMANAGER_H_INCLUDED
#include "../utils/pagedef.h"

class DBPageManager{
private:
	unsigned char data[PAGE_SIZE];
public:
	DBPageManager(short previous_page_id, short father_page_id, short father_page_offset, short data_size);
	void setnextpage(short next_page_id);
	unsigned char* getdata();
	bool setdata(unsigned char* newdata, int len);
	void debug();
	~DBPageManager();
};
#endif
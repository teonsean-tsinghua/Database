#ifndef DBSINGLEPAGEBASE_H_INCLUDED
#define DBSINGLEPAGEBASE_H_INCLUDED
#include "../utils/pagedef.h"

class DBSinglePageBase{
private:
	unsigned char *data;
public:
	DBSinglePageBase(unsigned short previous_page_id, unsigned short father_page_id, unsigned short father_page_offset, unsigned short data_size);
	void setnextpage(unsigned short next_page_id);
	unsigned char* getdata();
	bool setdata(unsigned char* newdata, int len);
	void debug();
	~DBSinglePageBase();
};
#endif
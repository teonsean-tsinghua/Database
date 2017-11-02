#ifndef DBSINGLEPAGEBASE_H_INCLUDED
#define DBSINGLEPAGEBASE_H_INCLUDED
#include "../utils/pagedef.h"

class DBSinglePageBase{
private:
	unsigned char *data;
public:
	DBSinglePageBase(short previous_page_id, short father_page_id, short father_page_offset, short data_size);
	void setnextpage(short next_page_id);
	unsigned char* getdata();
	bool setdata(unsigned char* newdata, int len);
	void debug();
	~DBSinglePageBase();
};
#endif
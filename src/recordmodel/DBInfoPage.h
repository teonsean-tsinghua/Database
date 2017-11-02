#ifndef DBINFOPAGE_H_INCLUDED
#define DBINFOPAGE_H_INCLUDED
#include "../utils/pagedef.h"

class DBInfoPage{
private:
	unsigned char *data;
public:
	DBInfoPage();
	void setbit(unsigned short bit_id, bool val);
	bool getbit(unsigned short bit_id);
	void debug();
	~DBInfoPage();
};
#endif
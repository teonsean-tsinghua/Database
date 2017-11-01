#ifndef DBBASERECORD_H_INCLUDED
#define DBBASERECORD_H_INCLUDED
#include <vector>
class DBBaseRecord{
private:
	std::vector<short> _datalen;
	std::vector<short> data;
public:
	DBBaseRecord();
	void frombytestring(unsigned char* data);
	void setdatatype(std::vector<short>* vdata);
	unsigned char* tobytestring();
};
#endif
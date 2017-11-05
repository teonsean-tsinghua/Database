#ifndef DBINDEXUSAGE_H_INCLUDED
#define DBINDEXUSAGE_H_INCLUDED

class DBIndexUsage{
public:
	DBIndexUsage();
	int allocateNewRecord(int size);// TODO
	int getUsageByPage(int pageID);// TODO
};

#endif
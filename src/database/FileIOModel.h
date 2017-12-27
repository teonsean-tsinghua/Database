#ifndef FILEIOMODEL_H_INCLUDED
#define FILEIOMODEL_H_INCLUDED

#include"Include.h"

class FileIOModel
{
	const static std::string TAG;
	std::string root;

    static FileIOModel* instance;
    BufPageManager* bpm;
    FileManager* fm;

    FileIOModel();

public:
    static FileIOModel* getInstance();

    ~FileIOModel();

    void setRoot(std::string r);

    void rmDir(const char* path);

    bool isFile(std::string fname);

    bool isDir(std::string dname);

    bool exist(std::string name);

    void checkDb(std::string dbname, bool exist);

    void checkIdxDir(std::string dbname, std::string tbname, bool exist);

    void checkDataFile(std::string dbname, std::string tbname, bool exist);

    void checkIdxFile(std::string dbname, std::string tbname, std::string fdname, bool exist);

    void createDb(std::string dbname);

    void createDataFile(std::string dbname, std::string tbname);

    void createIndexFile(std::string dbname, std::string tbname, std::string fdname);

    void showTables(std::string dbname);

    void showDbs();

    void dropDb(std::string dbname);

    void dropTable(std::string dbname, std::string tbname);

    void dropIdx(std::string dbname, std::string tbname, std::string fdname);

    void openDataFile(std::string dbname, std::string tbname, int& fileID);

    void openIdxFile(std::string dbname, std::string tbname, std::string fdname, int& fileID);

    void closeFile(int fileID);

    char* getPage(int fileID, int pageID, int& index);

    void flush(int index);

    void release(int index);

    static void test();

};

#endif // FILEIOMODEL_H_INCLUDED


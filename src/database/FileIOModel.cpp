#include"FileIOModel.h"

FileIOModel* FileIOModel::instance;

const std::string FileIOModel::TAG = "FileIOModel";

FileIOModel::FileIOModel()
{
    fm = new FileManager();
    bpm = new BufPageManager(fm);
}

FileIOModel::~FileIOModel()
{
	delete fm;
	delete bpm;
}

FileIOModel* FileIOModel::getInstance()
{
    if(instance == NULL)
    {
        instance = new FileIOModel();
    }
    return instance;
}

void FileIOModel::showTables(std::string dbname)
{
	checkDb(dbname, true);
	std::string path = root + "/" + dbname;
	struct dirent * filename;
	DIR * dir = opendir(path.c_str());
	std::cout << "All tables of database " << dbname << " are listed as below:\n";
	while((filename = readdir(dir)) != NULL)
	{
		std::string name(filename->d_name);
		if(!isDir(path + "/" + name) && name.size() >= 4 && name.substr(name.size() - 4, name.size()) == ".dat")
		{
			std::cout << name.substr(0, name.size() - 4) << " ";
		}
	}
	std::cout << std::endl;
}

void FileIOModel::showDbs()
{
	struct dirent * filename;
	DIR * dir = opendir(root.c_str());
	std::cout << "All databases are listed as below:\n";
	while((filename = readdir(dir)) != NULL)
	{
		std::string name(filename->d_name);
		if(name != "." && name != ".." && isDir(root + "/" + name))
		{
			std::cout << name << " ";
		}
	}
	std::cout << std::endl;
}

void FileIOModel::setRoot(std::string r)
{
    if(!(exist(r) && isDir(r)))
    {
    	std::cout << "Cannot open root directory.\n";
    	exit(0);
    }
    root = r;
}

bool FileIOModel::exist(std::string name)
{
	return (access(name.c_str(), F_OK) == 0);
}

bool FileIOModel::isDir(std::string dname)
{
	return (opendir(dname.c_str()) != NULL);
}

bool FileIOModel::isFile(std::string fname)
{
	return (opendir(fname.c_str()) == NULL);
}

void FileIOModel::checkDb(std::string dbname, bool e)
{
	assert(dbname != "");
	std::string path = root + "/" + dbname;
	if(e)
	{
		if(!(exist(path) && isDir(path)))
		{
			throw Exception(TAG, "Database " + dbname + " does not exist.");
		}
	}
	else
	{
		if(exist(path))
		{
			if(isDir(path))
			{
				throw Exception(TAG, "Database " + dbname + " already exists.");
			}
			else
			{
				throw Exception(TAG, path + " is not a directory.");
			}
		}
	}
}

void FileIOModel::checkIdxDir(std::string dbname, std::string tbname, bool e)
{
	assert(dbname != "");
	assert(tbname != "");
	std::string path = root + "/" + dbname + "/" + tbname + ".idx";
	if(e)
	{
		if(exist(path))
		{
			if(!isDir(path))
			{
				throw Exception(TAG, path + " is not a directory.");
			}
		}
		else
		{
			mkdir(path.c_str(), S_IRWXU|S_IRWXG|S_IROTH);
		}
	}
	else
	{
		if(exist(path))
		{
			if(!isDir(path))
			{
				throw Exception(TAG, path + " is not a directory.");
			}
			else
			{
				rmDir(path.c_str());
				rmdir(path.c_str());
			}
		}
	}
}

void FileIOModel::checkDataFile(std::string dbname, std::string tbname, bool e)
{
	assert(dbname != "");
	assert(tbname != "");
	std::string path = root + "/" + dbname + "/" + tbname + ".dat";
	if(e)
	{
		if(exist(path))
		{
			if(!isFile(path))
			{
				throw Exception(TAG, path + " is not a file.");
			}
		}
		else
		{
			throw Exception(TAG, "Table " + tbname + " does not exist.");
		}
	}
	else
	{
		if(exist(path))
		{
			if(isFile(path))
			{
				throw Exception(TAG, "Table " + tbname + " already exists.");
			}
			else
			{
				throw Exception(TAG, path + " is not a file.");
			}
		}
	}
}

void FileIOModel::checkIdxFile(std::string dbname, std::string tbname, std::string fdname, bool e)
{
	assert(dbname != "");
	assert(tbname != "");
	assert(fdname != "");
	std::string path = root + "/" + dbname + "/" + tbname + ".idx/" + fdname + ".idx";
	if(e)
	{
		if(exist(path))
		{
			if(!isFile(path))
			{
				throw Exception(TAG, path + " is not a file.");
			}
		}
		else
		{
			throw Exception(TAG, "Index " + fdname + " does not exist.");
		}
	}
	else
	{
		if(exist(path))
		{
			if(isFile(path))
			{
				throw Exception(TAG, "Index " + tbname + " already exists.");
			}
			else
			{
				throw Exception(TAG, path + " is not a file.");
			}
		}
	}
}

void FileIOModel::release(int index)
{
    bpm->release(index);
}

void FileIOModel::flush(int index)
{
    bpm->access(index);
    bpm->markDirty(index);
    bpm->writeBack(index);
}

void FileIOModel::createDb(std::string dbname)
{
	checkDb(dbname, false);
	std::string path = root + "/" + dbname;
	mkdir(path.c_str(), S_IRWXU|S_IRWXG|S_IROTH);
}

void FileIOModel::createDataFile(std::string dbname, std::string tbname)
{
	checkDb(dbname, true);
	checkDataFile(dbname, tbname, false);
	std::string path = root + "/" + dbname + "/" + tbname + ".dat";
	if(!fm->createFile(path.c_str()))
	{
		throw Exception(TAG, "File " + path + " cannot be created.");
	}
}

void FileIOModel::openDataFile(std::string dbname, std::string tbname, int& fileID)
{
	checkDb(dbname, true);
	checkDataFile(dbname, tbname, true);
	std::string path = root + "/" + dbname + "/" + tbname + ".dat";
    if(!fm->openFile(path.c_str(), fileID))
    {
        throw Exception(TAG, "File " + path + " cannot be opened.");
    }
}

void FileIOModel::openIdxFile(std::string dbname, std::string tbname, std::string fdname, int& fileID)
{
	checkDb(dbname, true);
	checkDataFile(dbname, tbname, true);
	checkIdxDir(dbname, tbname, true);
	checkIdxFile(dbname, tbname, fdname, true);
	std::string path = root + "/" + dbname + "/" + tbname + ".idx/" + fdname + ".idx";
    if(!fm->openFile(path.c_str(), fileID))
    {
        throw Exception(TAG, "File " + path + " cannot be opened.");
    }
}

void FileIOModel::createIndexFile(std::string dbname, std::string tbname, std::string fdname)
{
	checkDb(dbname, true);
	checkDataFile(dbname, tbname, true);
	checkIdxDir(dbname, tbname, true);
	checkIdxFile(dbname, tbname, fdname, false);
	std::string path = root + "/" + dbname + "/" + tbname + ".idx/" + fdname + ".idx";
	if(!fm->createFile(path.c_str()))
	{
		throw Exception(TAG, "File " + path + " cannot be created.");
	}
}

void FileIOModel::rmDir(const char* path)
{
    DIR *pDir = NULL;
    struct dirent *dmsg;
    char szFileName[128];
    char szFolderName[128];
    strcpy(szFolderName, path);
    strcat(szFolderName, "/%s");
    if((pDir = opendir(path)) != NULL)
    {
        while((dmsg = readdir(pDir)) != NULL)
        {
            if(strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
            {
                sprintf(szFileName, szFolderName, dmsg->d_name);
                if(opendir(szFileName) != NULL){
                    rmDir(szFileName);
                }
                remove(szFileName);
            }
        }
        closedir(pDir);
    }
}

void FileIOModel::dropDb(std::string dbname)
{
	checkDb(dbname, true);
	std::string path = root + "/" + dbname;
	rmDir(path.c_str());
	rmdir(path.c_str());
}

void FileIOModel::dropTable(std::string dbname, std::string tbname)
{
	checkDb(dbname, true);
	checkDataFile(dbname, tbname, true);
	std::string path = root + "/" + dbname + "/" + tbname + ".dat";
	if(!remove(path.c_str()) == 0)
	{
		throw Exception(TAG, "File " + path + " cannot be deleted.");
	}
	checkIdxDir(dbname, tbname, false);
}

void FileIOModel::dropIdx(std::string dbname, std::string tbname, std::string fdname)
{
	checkDb(dbname, true);
	checkDataFile(dbname, tbname, true);
	checkIdxDir(dbname, tbname, true);
	checkIdxFile(dbname, tbname, fdname, true);
	std::string path = root + "/" + dbname + "/" + tbname + ".idx/" + fdname + ".idx";
	if(!remove(path.c_str()) == 0)
	{
		throw Exception(TAG, "File " + path + " cannot be deleted.");
	}
}

void FileIOModel::closeFile(int fileID)
{
    if(!fm->closeFile(fileID) == 0)
    {
        throw Exception(TAG, "File cannot be closed.");
    }
}

char* FileIOModel::getPage(int fileID, int pageID, int& index)
{
    return (char*)(bpm->getPage(fileID, pageID, index));
}

void FileIOModel::test()
{
	FileIOModel* fm = FileIOModel::getInstance();
	fm->setRoot("/home/teon/Documents/Database");
	//fm->createDb("test");
	fm->createDataFile("test", "test");
	fm->createIndexFile("test", "test", "test");
	fm->dropIdx("test", "test", "test");
}

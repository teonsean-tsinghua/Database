#include"Table.h"
#include"../database/DataFile.h"

std::vector<int> Table::idx;
std::vector<int> Table::width;

void Table::printHeader(std::vector<bool>& selected, RecordInfo* ri)
{
    idx.clear();
    width.clear();
	for(int i = 0; i < selected.size(); i++)
	{
		if(selected[i])
		{
			idx.push_back(i);
			width.push_back(std::max(ri->displayLength(i), (int)ri->name(i).size()));
		}
	}
	std::cout << '+';
	for(int i = 0; i < idx.size(); i++)
	{
		for(int j = 0; j < width[i]; j++)
		{
			std::cout << "—";
		}
		std::cout << "+";
	}
	std::cout << std::endl;
	std::cout << '|';
	for(int i = 0; i < idx.size(); i++)
	{
		std::cout << std::setfill(' ') << std::setw(width[i]) << ri->name(idx[i]) << '|';
	}
	std::cout << std::endl;
	std::cout << '+';
	for(int i = 0; i < idx.size(); i++)
	{
		for(int j = 0; j < width[i]; j++)
		{
			std::cout << "—";
		}
		std::cout << "+";
	}
	std::cout << std::endl;
}

void Table::printRow(std::vector<void*>& data, RecordInfo* ri)
{
    std::cout.setf(std::ios::right);
    std::cout << '|';
    for(int i = 0; i < idx.size(); i++)
    {
        std::cout << std::setfill(' ') << std::setw(width[i]);
        if(data[i] == NULL)
        {
            std::cout << "NULL";
        }
        else
        {
            switch(ri->type(idx[i]))
            {
            case Type::INT:
                std::cout << *(IntType*)data[i];
                break;
            case Type::_ID:
                std::cout << *(IDType*)data[i];
                break;
            case Type::FLOAT:
                std::cout << *(FloatType*)data[i];
                break;
            case Type::DATE:
                std::cout << *(DateType*)data[i];
                break;
            case Type::VARCHAR:
                std::cout << *(VarcharType*)data[i];
                break;
            }
        }
        std::cout << '|';
    }
    std::cout << std::endl;
    std::cout << '+';
    for(int i = 0; i < idx.size(); i++)
    {
        for(int j = 0; j < width[i]; j++)
        {
            std::cout << "—";
        }
        std::cout << "+";
    }
    std::cout << std::endl;
}

void Table::print(std::vector<bool>& selected, RecordInfo* ri, SelectResult& sr, DataFile* df)
{
//	std::vector<void*> data;
//	for(int i = 0; i < sr.results.size(); i++)
//	{
//		int result = sr.results[i];
//		df->openDataPage(result / 8192)->read(result % 8192, data);
//		std::cout << '|';
//		for(int i = 0; i < idx.size(); i++)
//		{
//			std::cout << std::setw(width[i]);
//			if(data[idx[i]] == NULL)
//			{
//				std::cout << "NULL";
//			}
//			else
//			{
//				switch(ri->type(idx[i]))
//				{
//				case Type::INT:
//					std::cout << *(IntType*)data[idx[i]];
//					break;
//                case Type::_ID:
//                    std::cout << *(IDType*)data[idx[i]];
//                    break;
//                case Type::FLOAT:
//                    std::cout << *(FloatType*)data[idx[i]];
//                    break;
//                case Type::DATE:
//                    std::cout << *(DateType*)data[idx[i]];
//                    break;
//                case Type::VARCHAR:
//                    std::cout << *(VarcharType*)data[idx[i]];
//                    break;
//				}
//			}
//			std::cout << '|';
//		}
//		std::cout << std::endl;
//		std::cout << '+';
//		for(int i = 0; i < idx.size(); i++)
//		{
//			for(int j = 0; j < width[i]; j++)
//			{
//				std::cout << "—";
//			}
//			std::cout << "+";
//		}
//		std::cout << std::endl;
//	}
}

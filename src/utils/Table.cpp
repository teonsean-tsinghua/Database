#include"Table.h"
#include"../FileLayer/DataFile.h"

void Table::print(std::vector<bool>& selected, RecordInfo* ri, SelectResult& sr, DataFile* df)
{
	std::cout << std::setfill(' ');
	std::vector<int> idx;
	std::vector<int> width;
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
		std::cout << std::setw(width[i]) << ri->name(idx[i]) << '|';
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
	std::vector<void*> data;
	for(int i = 0; i < sr.results.size(); i++)
	{
		int result = sr.results[i];
		df->openDataPage(result / 8192)->read(result % 8192, data);
		std::cout << '|';
		for(int i = 0; i < idx.size(); i++)
		{
			std::cout << std::setw(width[i]);
			if(data[idx[i]] == NULL)
			{
				std::cout << "NULL";
			}
			else
			{
				switch(ri->type(idx[i]))
				{
				case Type::INT:
					std::cout << *(int*)data[idx[i]];
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
}
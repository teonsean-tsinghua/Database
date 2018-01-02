#include"Table.h"
#include"../database/DataFile.h"

std::vector<int> Table::idx;
std::vector<int> Table::width;
std::vector<std::vector<int> > Table::idxes;
std::vector<std::vector<int> > Table::widths;

void Table::printHeader(std::vector<std::vector<bool> >& selected, std::vector<RecordInfo*>& ri, std::vector<std::string>& tbnames)
{
    idxes.clear();
    widths.clear();
    for(int j = 0; j < selected.size(); j++)
    {
        idxes.push_back(std::vector<int>());
        widths.push_back(std::vector<int>());
        for(int i = 0; i < selected[j].size(); i++)
        {
            if(selected[j][i])
            {
                idxes[j].push_back(i);
                widths[j].push_back(std::max(ri[j]->displayLength(i), (int)tbnames[j].size() + 1 + (int)ri[j]->name(i).size()));
            }
        }
    }
    std::cout << '+';
    for(int k = 0; k < idxes.size(); k++)
    {
        for(int i = 0; i < idxes[k].size(); i++)
        {
            for(int j = 0; j < widths[k][i]; j++)
            {
                std::cout << "—";
            }
            std::cout << "+";
        }
    }
	std::cout << std::endl;
	std::cout << '|';
	for(int k = 0; k < idxes.size(); k++)
    {
        for(int i = 0; i < idxes[k].size(); i++)
        {
            std::cout << std::setfill(' ') << std::setw(widths[k][i]) << (tbnames[k] + "." + ri[k]->name(idxes[k][i])) << '|';
        }
    }
	std::cout << std::endl;
	std::cout << '+';
    for(int k = 0; k < idxes.size(); k++)
    {
        for(int i = 0; i < idxes[k].size(); i++)
        {
            for(int j = 0; j < widths[k][i]; j++)
            {
                std::cout << "—";
            }
            std::cout << "+";
        }
    }
	std::cout << std::endl;
}

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

void Table::printRow(std::vector<std::vector<void*> >& row, std::vector<RecordInfo*>& ri)
{
    std::cout << '|';
    for(int k = 0; k < idxes.size(); k++)
    {
        for(int i = 0; i < idxes[k].size(); i++)
        {
            std::cout << std::setfill(' ') << std::setw(widths[k][i]);
            if(row[k][idxes[k][i]] == NULL)
            {
                std::cout << "NULL";
            }
            else
            {
                switch(ri[k]->type(idxes[k][i]))
                {
                case Type::INT:
                    std::cout << *(IntType*)row[k][idxes[k][i]];
                    break;
                case Type::_ID:
                    std::cout << *(IDType*)row[k][idxes[k][i]];
                    break;
                case Type::FLOAT:
                    std::cout << *(FloatType*)row[k][idxes[k][i]];
                    break;
                case Type::DATE:
                    std::cout << *(DateType*)row[k][idxes[k][i]];
                    break;
                case Type::VARCHAR:
                    std::cout << *(VarcharType*)row[k][idxes[k][i]];
                    break;
                }
            }
            std::cout << '|';
        }
    }
    std::cout << std::endl;
    std::cout << '+';
    for(int k = 0; k < idxes.size(); k++)
    {
        for(int i = 0; i < idxes[k].size(); i++)
        {
            for(int j = 0; j < widths[k][i]; j++)
            {
                std::cout << "—";
            }
            std::cout << "+";
        }
    }
    std::cout << std::endl;
}

void Table::printRow(std::vector<void*>& data, RecordInfo* ri)
{
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

#include"DBDatabase.h"

DBDataBase* DBDataBase::instance = new DBDataBase();

DBDataBase::DBDataBase()
{

}

DBDataBase* DBDataBase::getInstance()
{
    if(instance == NULL)
    {
        instance = new DBDataBase();
    }
    return instance;
}

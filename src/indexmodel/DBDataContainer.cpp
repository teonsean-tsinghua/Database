#include"DBDataContainer.h"

DBDataContainer::DBDataContainer(DBIndexManager* indexManager, int position)
{
    unique = true;
    this->indexManager = indexManager;
    this->position = position;
}

DBDataContainer::DBDataContainer(DBIndexManager* indexManager, DataBucket* bucket)
{
    unique = false;
    this->indexManager = indexManager;
    this->first_bucket = bucket;
    this->current_bucket = bucket;
    index = 0;
}

int getNext()
{
    if(unique)
    {
        return position;
    }
    if(current_bucket == NULL)
    {
        return -1;
    }
    if(index < current_bucket->key_cnt)
    {
        return current_bucket->data[index++];
    }
    current_bucket = (DataBucket*)(indexManager->visit(current_bucket->next, false));
    if(current_bucket == NULL)
    {
        return -1;
    }
    index = 0;
    return current_bucket->data[index++];
}

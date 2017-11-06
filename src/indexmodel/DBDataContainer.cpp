#include"DBDataContainer.h"
#include<cstring>

DataBucket::DataBucket(int position, unsigned int volumn, const unsigned char* raw)
{
    this->position = position;
    this->volumn = volumn;
    data = new int[volumn];
    memset(data, -1, sizeof(int) * volumn);
    if(raw == NULL)
    {
        cnt = 0;
        next = -1;
    }
    else
    {
        sscanf(raw, "%x%x", &cnt, &next);
        for(int i = 0; i < cnt; i++)
        {
            sscanf(raw + sizeof(int) * (i + 2), "%x", data + sizeof(int) * i);
        }
    }
}

const unsigned char* DataBucket::toString()
{
    unsigned char* data = new unsigned char[sizeof(int) * (volumn + 2)];
    sprintf(data, "%x%x", cnt, next);
    for(int i = 0; i < volumn; i++)
    {
        sprintf(data + sizeof(int) * (i + 2), "%x", data[i]);
    }
    return data;
}

int DBDataContainer::insert(int position)
{
    if(unique)
    {
        return 0;
    }
    current = first;
    while(true)
    {
        if(current == NULL)
        {
            return -1;
        }
        if(current->cnt < current->volumn)
        {
            current->data[current->cnt] = position;
            current->cnt++;
            return 0;
        }
        if(current->next < 0)
        {
            current->next = indexManager->allocateNewBucket();
        }
        current = (DataBucket*)(indexManager->visit(current->next));
    }
}

int DBDataContainer::remove(int position, int& new_first_bucket)
{
    if(unique)
    {
        return 0;
    }
    current = first;
    DataBucket* prev = NULL;
    while(true)
    {
        if(current == NULL)
        {
            return -1;
        }
        for(int i = 0; i < current->cnt; i++)
        {
            if(position == current->data[i])
            {
                for(int j = i; j < current->cnt - 1; j++)
                {
                    current->data[j] = current->data[j + 1];
                }
                current->cnt--;
                if(current->cnt == 0)
                {
                    if(prev != NULL)
                    {
                        prev->next = current->next;
                        indexManager->erase(current->position);
                        delete current;
                    }
                    else
                    {
                        if(current->next < 0)
                        {
                            return EMPTY_BUCKET;
                        }
                        else
                        {
                            new_first_bucket = current->next;
                            return NEXT_BUCKET;
                        }
                    }
                }
                return 0;
            }
        }
        prev = current;
        current = (DataBucket*)(indexManager->visit(current->next));
    }
}

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
    this->first = bucket;
    this->current = bucket;
    index = 0;
}

int DBDataContainer::getNext()
{
    if(unique)
    {
        return position;
    }
    if(current == NULL)
    {
        return -1;
    }
    if(index < current->key_cnt)
    {
        return current->data[index++];
    }
    current = (DataBucket*)(indexManager->visit(current->next, false));
    if(current == NULL)
    {
        return -1;
    }
    index = 0;
    return current->data[index++];
}

#include"DBIndex.h"

template<typename T>
void* DBIndex<T>::visit(int position)
{
    if(position < 0)
    {
        return NULL;
    }
    if(cache.count(position))
    {
        if(cache[position] != NULL)
        {
            return cache[position];
        }
    }
    void* result = (void*)(fileManager->readIndexPage(name, position / PAGE_SIZE, position % PAGE_SIZE, length));
    cache[position] = result;
    return result;
}

template<typename T>
DBIndex<T>::DBIndex(DBFileManager* fileManager): fileManager(fileManager) {}

template<typename T>
DBIndex<T>::~DBIndex()
{
    clearCache();
}

template<typename T>
bool DBIndex<T>::isUnique()
{
    return unique;
}

template<typename T>
void DBIndex<T>::clearCache()
{
    typename std::map<int, Node<T>*>::iterator iter;
    for(iter = cache.begin(); iter != cache.end(); iter++)
    {
        delete iter->second;
    }
    cache.clear();
}

template<typename T>
void DBIndex<T>::insert(T& key, int pageID, int offset)
{
    if(root == NULL)
    {
        return;
    }
    Node<T>* current = root;
    //TODO
}

template<typename T>
void DBIndex<T>::remove(T& key, int pageID, int offset)
{
    remove(key, pageID * PAGE_SIZE + offset);
}

template<typename T>
void DBIndex<T>::remove(T& key, int record_position)
{
    int position = search(key, root);
    if(position < 0)
    {
        return;
    }
    if(!unique)
    {
        DataBucket* bucket = (DataBucket*)visit(position);
        bool removed = false;
        while(!removed && bucket != NULL)
        {
            for(int i = 0; i < cnt; i++)
            {
                if(record_position == bucket->data[i])
                {
                    for(int j = i + 1; j < cnt; j++)
                    {
                        bucket->data[j - 1] = bucket->data[j];
                    }
                    bucket->cnt--;
                    removed = true;
                    if(bucket->cnt == 0)
                    {
                        DataBucket* prev = (DataBucket*)visit(bucket->prev);
                        if(prev != NULL)
                        {
                            prev->next = bucket->next;
                        }
                        DataBucket* next = (DataBucket*)visit(bucket->next);
                        if(next != NULL)
                        {
                            next->prev = bucket->prev;
                        }
                    }
                }
            }
            bucket = (DataBucket*)visit(bucket->next);
        }
    }
    //TODO
}

template<typename T>
void* DBIndex<T>::search(T& key)
{
    bool found = false;
    int position = search(key, root);
    if(position < 0)
    {
        return NOT_FOUND;
    }
    if(unique)
    {
        // If unique, return the pointer to the data.
        return fileManager->readDataPage(position / PAGE_SIZE, position % PAGE_SIZE);
    }
    // If not unique, return a pointer bucket.
    return visit(position);
}

template<typename T>
int DBIndex<T>::search(T& key, Node<T>* current)
{
    if(current == NULL)
    {
        return -1;
    }
    if(current->isLeaf)
    {
        for(int i = 0; i < current->key_cnt; i++)
        {
            if(key == current->keys[i])
            {
                found = true;
                return children[i];
            }
        }
        return -1;
    }
    for(int i = 0; i < current->key_cnt; i++)
    {
        if(key <= current->keys[i])
        {
            return search(key, visit(current->children[i]));
        }
    }
    return search(key, visit(current->children[key_cnt]));
}

template<typename T>
int DBIndex<T>::createIndex(bool _unique, int key_length, const char* name)
{
    clearCache();
    unique = _unique;
    length = sizeof(int) * (3 + TREE_LEVEL) + sizeof(bool) * 2 + key_length * (TREE_LEVEL - 1);
    bucket_volumn = length / sizeof(int) - 3;
    unsigned int root_pageID = 0, root_offset = 0;
    //Iterate through all the records, insert one by one.
    unsigned char* data = new char[length];
    sprintf(data, "%x%x%x%x", unique, length, root_pageID, root_offset);
    fileManager->writeToIndexPage(name, data, true);
    return SUCCEED;
}

template<typename T>
int DBIndex<T>::parseIndex(const char* name)
{
    clearCache();
    unsigned char* data = fileManager->readIndexPage(this->name = name, 0, 0, 4 * sizeof(int));
    unsigned int root_pageID = 0, root_offset = 0;
    sscanf(data, "%x%x%x%x", &unique, &length, &root_pageID, &root_offset);
    root = visit(root_offset + root_pageID * PAGE_SIZE);
    bucket_volumn = length / sizeof(int) - 3;
    return SUCCEED;
}

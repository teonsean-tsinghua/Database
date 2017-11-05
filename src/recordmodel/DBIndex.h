#ifndef DBINDEX_H_INCLUDED
#define DBINDEX_H_INCLUDED

#include<map>
#include"DBFileManager.h"
#define TREE_LEVEL 5

template<typename T>
class Node {

public:
    int parent;
    int data;
    bool isLeaf;
    bool isRoot;
    int key_cnt;
    int children[TREE_LEVEL];
    T keys[TREE_LEVEL - 1];
};

class DataBucket {

public:
    int cnt;
    int prev;
    int next;   // TODO: should be initialized to -1
    int data[];
};

template<typename T>
class DBIndex {
    // Methods in this class does not manipulate real data, but only the nodes and buckets in index file.
    Node<T>* root;
    bool unique;
    unsigned int length;
    int bucket_volumn;
    char* name;
    DBFileManager* fileManager;
    std::map<int, Node<T>*> cache;

    void* visit(int position);

    void clearCache();

    int search(T& key, Node<T>* current);

    void remove(T& key, int record_position);

public:
    DBIndex(DBFileManager* fileManager);

    int createIndex(bool _unique, int key_length, const char* name);

    int parseIndex(const char* name);

    void* search(T& key);

    void insert(T& key, int pageID, int offset);

    void remove(T& key, int pageID, int offset);

    bool isUnique();

    ~DBIndex();

    const static int SUCCEED = 0;
    const static int NOT_FOUND = 404;
};

#endif // DBINDEX_H_INCLUDED

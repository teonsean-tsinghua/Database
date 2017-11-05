#ifndef DBINDEX_H_INCLUDED
#define DBINDEX_H_INCLUDED

class DBIndex<T> {
    Node<T>* root;
    bool unique;
    DBFileManager* fileManager;

    Node<T>* visit(Position<T>* position);

public:
    DBIndex();

    int createIndex(bool _unique);

    int parseIndex(const char* filename);

    void* search(T& key);

    void insert(T& key, int pageID, int offset);

    void remove(T& key);

    bool isUnique();

    ~DBIndex();
};

class Position<T> {
public:
    int pageID;
    int offset;
    Node<T>* ptr;
};

class Node<T> {

public:
    Position* parent;
    Position* data;
    bool isLeaf;
    bool isRoot;
    int key_cnt;
    Position<T>** children;
    T keys[];
};

class DataBucket<T> {

public:
    int cnt;
    Position<T>** data;
};

#endif // DBINDEX_H_INCLUDED

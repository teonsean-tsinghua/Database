#ifndef INDEXTREE_H_INCLUDED
#define INDEXTREE_H_INCLUDED

#include"FileIOModel.h"
#include"NodePage.h"
#include"BaseFile.h"

template<typename T>
class IndexTree
{
private:
    BaseFile* bf;
    int root;
    int keyLength;
    int minDgr;

    int insert(T& key, int value, NodePage<T>* page);  // return: if this page conducts a splitting.

    bool remove(T& key, NodePage<T>* page);

public:
    IndexTree(BaseFile* bf, int root, int keyLength);

    NodePage<T>* node(int pid);

    void insert(T& key, int value);

    bool remove(T& key);

    bool update(T& key, int value);

    int search(T& key);
};

template<typename T>
IndexTree<T>::IndexTree(BaseFile* bf, int root, int keyLength):
    bf(bf), root(root), keyLength(keyLength)
{
    minDgr = (PAGE_SIZE - Page::PAGE_CONTENT_OFFSET) / (keyLength + sizeof(int));
    minDgr /= 2;
}

template<typename T>
bool IndexTree<T>::update(T& key, int value)
{
    NodePage<T>* cur = node(root);
    while(!cur->isLeaf())
    {
        Entry<T>* child = cur->search(key);
        if(child == NULL)
        {
            return false;
        }
        cur = node(child->value);
    }
    Entry<T>* child = cur->search(key);
    if(child == NULL || child->key != key)
    {
        return false;
    }
    child->value = value;
    return true;
}

template<typename T>
bool IndexTree<T>::remove(T& key, NodePage<T>* page)
{
    if(page->isLeaf())
    {
        page->remove(key);
    }
    else
    {
        Entry<T>* entry = page->search(key);
        if(entry == NULL)
        {
            return false;
        }
        NodePage<T>* selectedChild = node(entry->value);
        if(!remove(key, selectedChild))
        {
            return false;
        }
    }
    if(page->getChildCnt() >= minDgr)
    {
        return true;
    }
    NodePage<T>* left, * right;
    left = page->getPrevSamePage() > 0 ? node(page->getPrevSamePage()) : NULL;
    right = page->getNextSamePage() > 0 ? node(page->getNextSamePage()) : NULL;
    if(left != NULL && left->getChildCnt() > minDgr)
    {
        Entry<T>* entry = left->at(left->getChildCnt() - 1);
        page->insert(entry->key, entry->value);
        if(left->getParent() > 0)
        {
            Entry<T>* parentEntry = node(left->getParent())->search(entry->key);
            assert(parentEntry->value == left->getPageID());
            parentEntry->key << left->at(left->getChildCnt() - 2)->key;
        }
        left->remove(entry->key);
        if(!left->isLeaf())
        {
            node(entry->value)->setParent(page->getPageID());
        }
        return true;
    }
    if(right != NULL && right->getChildCnt() > minDgr)
    {
        Entry<T>* entry = right->at(0);
        page->insert(entry->key, entry->value);
        if(page->getParent() > 0)
        {
            Entry<T>* parentEntry = node(page->getParent())->search(page->at(page->getChildCnt() - 1)->key);
            assert(parentEntry->value = page->getPageID());
            parentEntry->key << entry->key;
        }
        right->remove(entry->key);
        if(!right->isLeaf())
        {
            node(entry->value)->setParent(page->getPageID());
        }
        return true;
    }
    if(left != NULL)
    {
        int cnt = page->getChildCnt();
        copyData((char*)&(page->at(0)->value), (char*)&(left->at(left->getChildCnt() - 1)->value), cnt * (keyLength + sizeof(int)));
        if(left->getParent() > 0)
        {
            Entry<T>* parentEntry = node(left->getParent())->search(left->at(left->getChildCnt() - 1)->key);
            assert(parentEntry->value == left->getPageID());
            parentEntry->key << page->at(cnt - 1)->key;
            parentEntry = node(page->getParent())->search(page->at(cnt - 1)->key);
            assert(parentEntry->value == page->getPageID());
            node(page->getParent())->remove(parentEntry->key);
        }
        if(!left->isLeaf())
        {
            for(int i = 0; i < cnt; i++)
            {
                node(page->at(i)->value)->setParent(left->getPageID());
            }
        }
        if(right != NULL)
        {
            right->setPrevSamePage(left->getPageID());
            left->setNextSamePage(right->getPageID());
        }
        else
        {
            left->setNextSamePage(-1);
        }
        left->setChildCnt(left->getChildCnt() + cnt);
        left->setFirstAvailableByte(Page::PAGE_CONTENT_OFFSET + (keyLength + sizeof(int)) * left->getChildCnt());
        bf->markAsUsable(page->getPageID());
        return true;
    }
    if(right != NULL)
    {
        int cnt = right->getChildCnt();
        copyData((char*)&(right->at(0)->value), (char*)&(page->at(page->getChildCnt() - 1)->value), cnt * (keyLength + sizeof(int)));
        if(page->getParent() > 0)
        {
            Entry<T>* parentEntry = node(page->getParent())->search(page->at(page->getChildCnt() - 1)->key);
            assert(parentEntry->value == page->getPageID());
            parentEntry->key << right->at(cnt - 1)->key;
            parentEntry = node(right->getParent())->search(right->at(cnt - 1)->key);
            assert(parentEntry->value == right->getPageID());
            node(right->getParent())->remove(parentEntry->key);
        }
        if(!page->isLeaf())
        {
            for(int i = 0; i < cnt; i++)
            {
                node(right->at(i)->value)->setParent(page->getPageID());
            }
        }
        if(right->getNextSamePage() > 0)
        {
            page->setNextSamePage(right->getNextSamePage());
            node(right->getNextSamePage())->setPrevSamePage(page->getPageID());
        }
        else
        {
            page->setNextSamePage(-1);
        }
        page->setChildCnt(page->getChildCnt() + cnt);
        page->setFirstAvailableByte(Page::PAGE_CONTENT_OFFSET + (keyLength + sizeof(int)) * page->getChildCnt());
        bf->markAsUsable(right->getPageID());
        return true;
    }
    assert(root == page->getPageID());
    return true;
}

template<typename T>
bool IndexTree<T>::remove(T& key)
{
    NodePage<T>* rootp = node(root);
    int re = remove(key, rootp);
    if(rootp->getChildCnt() == 1 && !rootp->isLeaf())
    {
        int oldroot = root;
        root = rootp->at(0)->value;
        bf->setRootPage(root);
        bf->markAsUsable(oldroot);
    }
    return re;
}

template<typename T>
int IndexTree<T>::search(T& key)
{
    NodePage<T>* cur = node(root);
    while(!cur->isLeaf())
    {
        Entry<T>* child = cur->search(key);
        if(child == NULL)
        {
            return 0;
        }
        cur = node(child->value);
    }
    Entry<T>* child = cur->search(key);
    if(child == NULL || child->key != key)
    {
        return 0;
    }
    return child->value;
}

template<typename T>
int IndexTree<T>::insert(T& key, int value, NodePage<T>* page)
{
    if(page->isLeaf())
    {
        page->insert(key, value);
    }
    else
    {
        bool updateMax = false;
        int fallInto = -1;
        Entry<T>* entry = page->search(key);
        if(entry == NULL)
        {
            updateMax = true;
            entry = page->at(page->getChildCnt() - 1);
            fallInto = entry->value;
        }
        else
        {
            updateMax = false;
            fallInto = entry->value;
        }
        NodePage<T>* selectedChild = node(fallInto);
        int new_childid = insert(key, value, selectedChild);
        if(updateMax)
        {
            entry->key << key;
        }
        if(new_childid != 0)
        {
            entry->value = new_childid;
            page->insert(selectedChild->at(selectedChild->getChildCnt() - 1)->key, fallInto);
        }
    }
    if(!page->isFull())
    {
        return 0;
    }
    NodePage<T>* new_page = node(bf->allocateNodePage(page->isLeaf()));
    int cnt = page->getChildCnt();
    int left = cnt / 2;
    int right = cnt - left;
    copyData((char*)&(page->at(left)->value), (char*)&(new_page->at(0)->value), right * (sizeof(int) + keyLength));
    page->setChildCnt(left);
    new_page->setChildCnt(right);
    page->setFirstAvailableByte(Page::PAGE_CONTENT_OFFSET + (sizeof(int) + keyLength) * left);
    new_page->setFirstAvailableByte(Page::PAGE_CONTENT_OFFSET + (sizeof(int) + keyLength) * right);
    if(page->getParent() > 0)
    {
        new_page->setParent(page->getParent());
    }
    new_page->setNextSamePage(page->getNextSamePage());
    if(page->getNextSamePage() > 0)
    {
        node(page->getNextSamePage())->setPrevSamePage(new_page->getPageID());
    }
    page->setNextSamePage(new_page->getPageID());
    new_page->setPrevSamePage(page->getPageID());
    if(!page->isLeaf())
    {
        for(int i = 0; i < right; i++)
        {
            node(new_page->at(i)->value)->setParent(new_page->getPageID());
        }
    }
    return new_page->getPageID();
}

template<typename T>
void IndexTree<T>::insert(T& key, int value)
{
    int new_pageid = insert(key, value, node(root));
    if(new_pageid == 0)
    {
        return;
    }
    int new_root = bf->allocateNodePage(false);
    NodePage<T>* new_rpage = node(new_root);
    NodePage<T>* old_rpage = node(root);
    NodePage<T>* new_node = node(new_pageid);
    new_node->setParent(new_root);
    old_rpage->setParent(new_root);
    bf->setRootPage(new_root);
    root = new_root;
    new_rpage->insert(old_rpage->at(old_rpage->getChildCnt() - 1)->key, old_rpage->getPageID());
    new_rpage->insert(new_node->at(new_node->getChildCnt() - 1)->key, new_node->getPageID());
}

template<typename T>
NodePage<T>* IndexTree<T>::node(int pid)
{
    return (NodePage<T>*)bf->openPage(pid);
}

#endif // INDEXTREE_H_INCLUDED

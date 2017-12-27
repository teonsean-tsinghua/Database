#include"NodePage.h"

template<typename T>
int NodePage<T>::getChildCnt()
{
	return readInt((*this)[CHILDREN_COUNT_OFFSET]);
}

template<typename T>
void NodePage<T>::setChildCnt(int n)
{
	writeInt((*this)[CHILDREN_COUNT_OFFSET], n);
}

template<typename T>
bool NodePage<T>::isLeaf()
{
    return getPageType() == Type::LEAF_PAGE;
}

template<typename T>
void NodePage<T>::setParent(int pid)
{
    writeInt((*this)[PARENT_NODE_OFFSET], pid);
}

template<typename T>
int NodePage<T>::getParent()
{
	return readInt((*this)[PARENT_NODE_OFFSET]);
}

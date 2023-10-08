#ifndef SEARCH_H
#define SEARCH_H

#include "qlist.h"

template<typename T>
bool contains_deref(QList<T*> *list, T *item)
{
    for(int i = 0; i < list->size(); i++)
    {
        if(*(list->at(i)) == *item)
        {
            return true;
        }
    }
    return false;
}

template<typename T>
void remove_deref(QList<T*> *list, T *item)
{
    for(int i = 0; i < list->size(); i++)
    {
        if(*(list->at(i)) == *item)
        {
            list->remove(i);
            i--;
        }
    }
}

#endif // SEARCH_H

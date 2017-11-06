/***************************************************
*
* file: implicit_als_model.h
*
* Copyright (C) Angela Burova and Egor Smirnov 2017
*
****************************************************
*/


#ifndef _IMPLICIT_ALS_MODEL_
#define _IMPLICIT_ALS_MODEL_

#include "numeric_table.h"

namespace als
{

template<typename FPType>
class Model
{
public:
    Model(TablePtr<FPType> usersFactors, TablePtr<FPType> itemsFactors) :
        _usersFactors(usersFactors), _itemsFactors(itemsFactors)
    {
        _nFactors = usersFactors->getNumberOfRows();
        _nUsers = usersFactors->getNumberOfColumns();
        _nItems = itemsFactors->getNumberOfColumns();
    }

    size_t getNumberOfFactors() { return _nFactors; }
    size_t getNumberOfUsers() { return _nUsers; }
    size_t getNumberOfItems() { return _nItems; }

    TablePtr<FPType> getUsersFactors()
    {
        return _usersFactors;
    }

    TablePtr<FPType> getItemsFactors()
    {
        return _itemsFactors;
    }

protected:
    TablePtr<FPType> _usersFactors;
    TablePtr<FPType> _itemsFactors;
    size_t _nFactors;
    size_t _nUsers;
    size_t _nItems;
};

template<typename T>
using ModelPtr = std::shared_ptr<Model<T>>;

} // als

#endif // _IMPLICIT_ALS_MODEL_

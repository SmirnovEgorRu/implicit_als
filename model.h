#include "table.h"

template<typename FPType>
class AlsModel
{
public:
    AlsModel(TablePtr<T> usersFactors, TablePtr<T> itemsFactors):
        _usersFactors(usersFactors), _itemsFactors(itemsFactors)
    {
        _nFactors = usersFactors->getNumberOfRows();
        _nUsers = usersFactors->getNumberOfColumns();
        _nItems = itemsFactors->getNumberOfColumns();
    }

    size_t getNumberOfFactors() { return _nFactors; }
    size_t getNumberOfUsers() { return _nUsers; }
    size_t getNumberOfItems() { return _nItems; }

    TablePtr<T> getUsersFactors()
    {
        return _usersFactors;
    }

    TablePtr<T> getItemsFactors()
    {
        return _itemsFactors;
    }

protected:
	TablePtr<T> _usersFactors;
	TablePtr<T> _itemsFactors;
	size_t _nFactors;
	size_t _nUsers;
	size_t _nItems;
};

tempale<typename T>
using AlsModelPtr<T> = std::shared_ptr<AlsModel<T>>;


#include "implicit_als_train.h"
#include <random>


namespace als
{
    template<float> class Train<float>;
    template<double> class Train<double>;

    template<typename T>
    void Train<T>::compute(TablePtr<T> data)
    {
        initModel(data);

        computeInternal(data);
    }

    template<typename T>
    void Train<T>::initModel(TablePtr<T>& data)
    {
        this->_nFactors = this->parameter.nFactors;
        this->_nUsers = data->getNumberOfRows();
        this->_nItems = data->getNumberOfColumns();

        TablePtr<T> usersFactors(this->_nFactors, this->_nUsers);
        TablePtr<T> itemsFactors(this->_nFactors, this->_nItems);

        initItemsFactors(itemsFactors, data);

        this->_model = AlsModelPtr<T>(new AlsModel<T>(usersFactors, itemsFactors));
    }

    template<typename T>
    void Train<T>::initItemsFactors(TablePtr<T>& itemsFactors, TablePtr<T>& dataPtr)
    {
        const T* const data = dataPtr->getPtr();
        T* const items = itemsFactors->getPtr();

        for (size_t i = 0u; i < this->_nItems; ++i)
        {
            T sum = 0.0f;
            size_t ratedUsers = 0u;
            for(size_t j = 0u; j < this->_nUsers; ++j)
            {
                if (data[j * this->_nItems + i])
                {
                    sum += data[j * this->_nItems + i];
                    ++ratedUsers;
                }
            }

            items[i] = ratedUsers > 0 ? sum / ratedUsers : 0;
        }

        std::default_random_engine generator (777);
        std::uniform_real_distribution<T> distribution (0.0,1.0);
        for (size_t i = 0u; i < this->_nItems; ++i)
        {
            for(size_t j = 0u; j < this->_nFactors; ++j)
            {
                items[j*this->_nItems + i] << distribution(generator);
            }
        }
    }

    template<typename T>
    void Train<T>::computeInternal(TablePtr<T>& dataPtr)
    {
        const size_t nIter = this->parameter.nIteration;

        TablePtr<T> tData = getTransposeMatrix(dataPtr);

        for (size_t i = 0u; i < nIter; ++i)
        {

        }
    }

    template<typename T>
    TablePtr<T> Train<T>::getTransposeMatrix(TablePtr<T>& data)
    {
        const size_t nRows = data->getNumberOfRows();
        const size_t nCols = data->getNumberOfColumns();

        TablePtr<T> tData(new Table<T>(nCols, nRows));
        for (size_t i = 0u; i < nRows; ++i)
        {
            for(size_t j = 0u; j < nCols; ++j)
            {
                tData[j * nRows + i] = data[i * nCols + j];
            }
        }

        return tData;
    }

    template<typename T>
    void Train<T>::updateFactors(TablePtr<T> TablePtr<T>& dataPtr)
    {

    }
}
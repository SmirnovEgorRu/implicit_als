/***************************************************
*
* file: implicit_als_train_train.cpp
*
* Copyright (C) Angela Burova and Egor Smirnov 2017
*
****************************************************
*/

#include "implicit_als_train.h"
#include "matrix_operations.h"

#include <random>
#include <vector>


namespace als
{

template class Train<float>;
template class Train<double>;


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

    TablePtr<T> usersFactors(new Table<T>(this->_nFactors, this->_nUsers));
    TablePtr<T> itemsFactors(new Table<T>(this->_nFactors, this->_nItems));

    initItemsFactors(itemsFactors, data);

    this->_model = ModelPtr<T>(new Model<T>(usersFactors, itemsFactors));
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
            items[j*this->_nItems + i] = distribution(generator);
        }
    }
}

template<typename T>
void Train<T>::computeInternal(TablePtr<T>& dataPtr)
{
    const size_t nIter = this->parameter.nIteration;
    TablePtr<T> tData = utils::getTransposeMatrix(dataPtr);

    TablePtr<T> usersFactors = this->_model->getUsersFactors();
    TablePtr<T> itemsFactors = this->_model->getItemsFactors();

    for (size_t i = 0u; i < nIter; ++i)
    {
        updateFactors(itemsFactors, usersFactors, dataPtr);
        updateFactors(usersFactors, itemsFactors, tData);
    }
}

template<typename T>
void Train<T>::updateFactors(TablePtr<T> otherFactors, TablePtr<T> currentFactors, TablePtr<T>& dataPtr)
{
    const size_t nCols = currentFactors->getNumberOfColumns();
    for(size_t j = 0u; j < nCols; ++j)
    {
        auto pair = getSubMatrixes(dataPtr, otherFactors, j);
        TablePtr<T> subMatrix = pair.first;
        TablePtr<T> R = pair.second;
        if (subMatrix->getNumberOfColumns() == 0) return; // case when Users hasn't ratings

        TablePtr<T> subTransposeMatrix = utils::getTransposeMatrix(subMatrix);
        TablePtr<T> regularization = getRegularization(subMatrix->getNumberOfColumns());
        TablePtr<T> MMt = utils::matrixMultiplication(subMatrix, subTransposeMatrix);
        TablePtr<T> A = utils::matrixSum(MMt, regularization);
        TablePtr<T> A_1 = utils::invertMatrix(A);
        TablePtr<T> V = utils::matrixMultiplication(subMatrix, R);
        TablePtr<T> U = utils::matrixMultiplication(A_1, V);

        for(size_t i = 0u; i < this->_nFactors; ++i)
        {
            (*currentFactors)[i * nCols + j] = (*U)[i];
        }
    }
}

template<typename T>
TablePtr<T> Train<T>::getRegularization(const size_t nonZero)
{
    TablePtr<T> regularization(new Table<T>(this->_nFactors, this->_nFactors));
    for(size_t i = 0u; i < this->_nFactors; ++i)
    for(size_t j = 0u; j < this->_nFactors; ++j)
    {
        (*regularization)[i*this->_nFactors + j] = 0;
    }

    const T lambda = (T)this->parameter.lambda;
    const T mult = lambda* (T)nonZero;
    for(size_t i = 0u; i < this->_nFactors; ++i)
    {
        (*regularization)[i*this->_nFactors + i] = mult;
    }

    return regularization;
}

template<typename T>
std::pair<TablePtr<T>, TablePtr<T>> Train<T>::getSubMatrixes(TablePtr<T>& dataPtr, TablePtr<T> factors, const size_t idx)
{
    const size_t nCols = dataPtr->getNumberOfColumns();
    std::vector<size_t> dimensions;

    for (size_t j = 0u; j < nCols; ++j) // TODO: case when dimensions.size = 0
    {
        if ((*dataPtr)[nCols * idx + j])
            dimensions.push_back(j);
    }
    const size_t nonZero = dimensions.size();

    if (!nonZero)
        std::runtime_error("dimensions are null");

    TablePtr<T> R(new Table<T>(nonZero, 1));
    for (size_t j = 0u; j < nonZero; ++j)
    {
        (*R)[j] = (*dataPtr)[nCols * idx + dimensions[j]];
    }

    TablePtr<T> subMatrix(new Table<T>(this->_nFactors, nonZero));

    for (size_t i = 0u; i < this->_nFactors; ++i)
    for (size_t j = 0u; j < nonZero; ++j)
    {
        (*subMatrix)[i * nonZero + j] = (*factors)[i * nCols + dimensions[j]];
    }

    return std::pair<TablePtr<T>, TablePtr<T>>(subMatrix, R);
}

} //als

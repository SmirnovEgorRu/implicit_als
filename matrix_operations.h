/***************************************************
*
* file: matrix_operations.h
*
* Copyright (C) angela Burova and egor Smirnov 2017
*
****************************************************
*/

#ifndef _MATRIX_OPERATIONS_
#define _MATRIX_OPERATIONS_

#include "numeric_table.h"
#include <exception>

namespace utils
{

template<typename T>
TablePtr<T> getTransposeMatrix(TablePtr<T>& data)
{
    const size_t nRows = data->getNumberOfRows();
    const size_t nCols = data->getNumberOfColumns();

    TablePtr<T> tData(new Table<T>(nCols, nRows));
    for (size_t i = 0u; i < nRows; ++i)
    {
        for (size_t j = 0u; j < nCols; ++j)
        {
            (*tData)[j * nRows + i] = (*data)[i * nCols + j];
        }
    }

    return tData;
}


template<typename T>
TablePtr<T> matrixMultiplication(TablePtr<T> a, TablePtr<T> b, const T alpha = 1.0f)
{
    if (a->getNumberOfColumns() != b->getNumberOfRows())
    {
        throw std::exception("matrixMultiplication: inconsistent dimensions");
    }

    TablePtr<T> c(new Table<T>(a->getNumberOfRows(), b->getNumberOfColumns()));

    size_t m = a->getNumberOfRows(), n = a->getNumberOfColumns(), k = b->getNumberOfColumns();
    size_t i, j, t;

    for (i = 0u; i < m; ++i)
    {
        for (j = 0u; j < k; ++j)
        {
            (*c)[i*k + j] = 0;
        }
    }

    for (i = 0u; i < m; ++i) for (t = 0u; t < k; ++t) for (j = 0u; j < n; ++j) (*c)[i*k + t] += alpha * (*a)[i*n + j] * (*b)[k*j + t];
    return c;
}


template<typename T>
TablePtr<T> matrixSum(TablePtr<T> a, TablePtr<T> b)
{
    if (a->getNumberOfRows() != b->getNumberOfRows() || a->getNumberOfColumns() != b->getNumberOfColumns())
    {
        std::runtime_error("matrixSum - inconsistent dimensions");
    }

    TablePtr<T> c(new Table<T>(a->getNumberOfRows(), a->getNumberOfColumns()));

    const size_t nRows = a->getNumberOfRows();
    const size_t nCols = a->getNumberOfColumns();

    for (size_t i = 0u; i < nRows; ++i)
        for (size_t j = 0u; j < nCols; ++j)
        {
            (*c)[i * nCols + j] = (*a)[i * nCols + j] + (*b)[i * nCols + j];
        }

    return c;
}

template<typename T>
TablePtr<T> invertMatrix(TablePtr<T> a)
{
    if (a->getNumberOfColumns() != a->getNumberOfRows())
    {
        std::runtime_error("invertMatrix - inconsistent dimensions");
    }

    const size_t N = a->getNumberOfRows();
    T temp;

    TablePtr<T> e(new Table<T>(N, N));

    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
        {
            (*e)[i*N + j] = 0.0;

            if (i == j)
                (*e)[i*N + j] = 1.0;
        }

    for (size_t k = 0; k < N; k++)
    {
        temp = (*a)[k*N + k];

        for (size_t j = 0; j < N; j++)
        {
            (*a)[k*N + j] /= temp;
            (*e)[k*N + j] /= temp;
        }

        for (size_t i = k + 1; i < N; i++)
        {
            temp = (*a)[i*N + k];

            for (size_t j = 0; j < N; j++)
            {
                (*a)[i*N + j] -= (*a)[k*N + j] * temp;
                (*e)[i*N + j] -= (*e)[k*N + j] * temp;
            }
        }
    }

    for (int k = (int)N - 1; k > 0; k--)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            temp = (*a)[i*N + k];

            for (int j = 0; j < N; j++)
            {
                (*a)[i*N + j] -= (*a)[k*N + j] * temp;
                (*e)[i*N + j] -= (*e)[k*N + j] * temp;
            }
        }
    }
    return e;
}

} // utils

#endif // _MATRIX_OPERATIONS_

/***************************************************
*
* file: numeric_table.h
*
* Copyright (C) Angela Burova and Egor Smirnov 2017
*
****************************************************
*/
#pragma once
#ifndef _NUMERIC_TABLE_
#define _NUMERIC_TABLE_

#include <memory>
#include "stdio.h"

template<typename T>
class Table
{
public:
    Table(const size_t nRows, const size_t nCols): _nRows(nRows), _nCols(nCols)
    {
        _data = (T*)malloc(sizeof(T) * _nRows * _nCols);
    }

    size_t getNumberOfRows() const
    {
        return _nRows;
    }

    size_t getNumberOfColumns() const
    {
        return _nCols;
    }

    inline const T& operator[](const size_t idx) const
    {
        return _data[idx];
    }

    inline T& operator[](const size_t idx)
    {
        return _data[idx];
    }

    inline Table<T>& operator=(const Table<T>& right)
    {
        for(size_t i=0;i<_nRows;i++)
        {
            for(size_t j=0;j<_nCols;j++)
            {
                (*this)[i*_nCols+j]=(*right)[i*_nCols+j];
            }
        }
        return *this;
    }

    const T*  getPtr() const
    {
        return _data;
    }

    T*  getPtr()
    {
        return _data;
    }

    ~Table()
    {
        free(_data);
    }

protected:
    T* _data;
    const size_t _nRows;
    const size_t _nCols;
};

template<typename T>
using TablePtr = std::shared_ptr<Table<T>>;

template<typename T>
void printTable(TablePtr<T> nt, char* const name = nullptr)
{
    const size_t nRows = nt->getNumberOfRows();
    const size_t nCols = nt->getNumberOfColumns();

    if (name) printf("%s:\n", name);
    for (size_t i = 0u; i < nRows; ++i)
    {
        for (size_t j = 0u; j < nCols; ++j)
        {
            printf("%3f\t", (*nt)[i*nCols + j]);
        }
        printf("\n");
    }
}

#endif //_NUMERIC_TABLE_

#include <utility>
#include "stdio.h"
#include<iostream>
#include "implicit_als_model.h"
#include "numeric_table.h"
#pragma once

namespace als
{
template<typename FPType>
class Prediction
{
public:
    Prediction(ModelPtr<FPType> model,TablePtr<FPType> oldTable ):_model(model),_oldTable(oldTable)
    {
        compute();

    }
    FPType getRating(const size_t nUser, const size_t nItem) const
    {
        return (*_table)[nUser*(_table->getNumberOfColumns())+nItem];
    }
    std::pair<FPType,FPType> getBestItem() const
    {
        FPType _sumRating=0;
        size_t _rows=_table->getNumberOfRows();
        size_t _cols=_table->getNumberOfColumns();
        std::pair<FPType,FPType> _bestItem(0,0);//number item and rating

        for(size_t i=0;i<_cols;i++)
        {
            for(size_t j=0;j<_rows;j++)
            {
                _sumRating+=(*_table)[j*_cols+i];

            }
            if(_sumRating>_bestItem.second)
            {
                _bestItem.second=_sumRating;
                _bestItem.first=i;
            }
            _sumRating=0;
        }
        _bestItem.second=_bestItem.second/_rows;
       return _bestItem;
    }

    std::pair<FPType,FPType> getBestItem(size_t const _user) const
    {
        std::pair<FPType,FPType> _bestItem (0,0);
        size_t _cols=_table->getNumberOfColumns();
        for(size_t i=0;i<_cols;i++)
        {
            if((*_oldTable)[_user*_cols+i]==0)
            {

                if ((*_table)[_user*_cols+i]>_bestItem.second)
                {
                    _bestItem.second=(*_table)[_user*_cols+i];
                    _bestItem.first=i;
                }
            }
        }
        return _bestItem;
    }

    FPType maxItem()
    {
        size_t _rows=_table->getNumberOfRows();
        size_t _cols=_table->getNumberOfColumns();
        FPType maxItem=0;

        for(size_t i=0;i<_cols;i++)
        {
            for(size_t j=0;j<_rows;j++)
            {
                if((*_table)[j*_cols+i]>maxItem)
                {
                    maxItem=(*_table)[j*_cols+i];
                }

            }
    }
    return maxItem;
    }

     FPType minItem()
    {
        size_t _rows=_table->getNumberOfRows();
        size_t _cols=_table->getNumberOfColumns();
        FPType minItem=0;

        for(size_t i=0;i<_cols;i++)
        {
            for(size_t j=0;j<_rows;j++)
            {
                if((*_table)[j*_cols+i]<minItem)
                {
                    minItem=(*_table)[j*_cols+i];
                }

            }
    }
    return minItem;
    }

    FPType avg()
    {
        size_t _rows=_table->getNumberOfRows();
        size_t _cols=_table->getNumberOfColumns();
        FPType sum=0;

        for(size_t i=0;i<_cols;i++)
        {
            for(size_t j=0;j<_rows;j++)
            {
                    sum+=(*_table)[j*_cols+i];
            }
    }
    return sum/(_cols*_rows);
    }


private:
    ModelPtr<FPType> _model;
    TablePtr<FPType> _usersFactors;
    TablePtr<FPType> _itemsFactors;
    TablePtr<FPType> _oldTable;
    TablePtr<FPType> _table;


    void compute()
    {
        TablePtr<FPType> usersFactors=_model->getUsersFactors();
        _usersFactors=utils::getTransposeMatrix(usersFactors);
        _itemsFactors=_model->getItemsFactors();
        _table=utils::matrixMultiplication(_usersFactors,_itemsFactors);
    }




};
}


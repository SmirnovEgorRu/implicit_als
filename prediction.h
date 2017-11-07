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
        getMatrix();
    }
    FPType getRating(const size_t nUser, const size_t nItem) const
    {
        size_t rating=0;
        size_t num=_usersFactors->getNumberOfColumns();
        for(size_t i=0;i<num;i++)
        {
            rating+=(*_usersFactors)[nUser*num+i]*(*_itemsFactors)[i*num+nItem];
        }
        return rating;
    }
    std::pair<size_t,size_t> getBestItem() const
    {
        if(_table==nullptr) compute();
        size_t _sumRating=0;
        size_t _rows=_table.getNumberOfRows();
        size_t _cols=_table.getNumberOfColumns();
        std::pair<size_t,size_t> _bestItem(0,0);//number item and rating

        for(size_t i=0;i<_cols;i++)
        {
            for(size_t j=0;j<_rows;j++)
            {
                _sumRating+=_table[j*_cols+i];
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

    std::pair<size_t,size_t> getBestItem(size_t const _user) const
    {
        std::pair<size_t,size_t> _bestItem (0,0);
        size_t _cols=_table.getNumberOfColumns();
        if(_table==nullptr) compute();
        for(size_t i=0;i<_cols;i++)
        {
            if(_oldTable[_user*_cols+i]==0)
            {
                if (_table[_user*_cols+i]>_bestItem.second)
                {
                    _bestItem.second=_table[_user*_cols+i];
                    _bestItem.first=i;
                }
            }
        }
        return _bestItem;
    }



private:
    const ModelPtr<FPType> _model;
    const TablePtr<FPType> _usersFactors;
    const TablePtr<FPType> _itemsFactors;
    const TablePtr<FPType> _table;
    const TablePtr<FPType> _oldTable;


    void getMatrix()
    {
        TablePtr<FPType> usersFactors=_model->getUsersFactors();
        _usersFactors=utils::getTransposeMatrix(usersFactors);
        _itemsFactors=_model->getItemsFactors();
    }

    void compute()
    {
        _table=utils::matrixMultiplication(_usersFactors,_itemsFactors);
    }



};
}


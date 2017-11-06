/***************************************************
*
* file: implicit_als_train_train.h
*
* Copyright (C) Angela Burova and Egor Smirnov 2017
*
****************************************************
*/


#ifndef _IMPLICIT_ALS_TRAIN_
#define _IMPLICIT_ALS_TRAIN_

#include "implicit_als_model.h"

namespace als
{

struct Parameter
{
    double lambda = 0.001f;
    size_t nIteration = 20;
    size_t nFactors = 10;
};

template<typename FPType>
class Train
{
public:
    Train(): _model(nullptr) {}

    void compute(TablePtr<FPType> data);

    ModelPtr<FPType> getModel()
    {
        return _model;
    }

    Parameter parameter;

protected:
    ModelPtr<FPType> _model;
    size_t _nFactors;
    size_t _nUsers;
    size_t _nItems;

    void computeInternal(TablePtr<FPType>& dataPtr);
    void initModel(TablePtr<FPType>& data);
    void initItemsFactors(TablePtr<FPType>& itemsFactors, TablePtr<FPType>& dataPtr);
    void updateFactors(TablePtr<FPType> otherFactors, TablePtr<FPType> currentFactors, TablePtr<FPType>& dataPtr);
    TablePtr<FPType> getRegularization(const size_t nonZero);
    std::pair<TablePtr<FPType>, TablePtr<FPType>> getSubMatrixes(TablePtr<FPType>& dataPtr, TablePtr<FPType> factors, const size_t idx);
};

} // als

#endif // _IMPLICIT_ALS_TRAIN_

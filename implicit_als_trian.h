
#include "model.h"

namespace als
{

struct Parameter
{
    double lambda = 0.001f;
    size_t nIteration = 20;
    size_t nFactors = 10;
};

template<typename FPType = float>
class Train
{
public:
    Train(): _model(nullptr) {}

    void compute(TablePtr<T> data);

    AlsModelPtr<T> getModel()
    {
        return _model;
    }

    Parameter parameter;

protected:
    AlsModelPtr<T> _model;
    size_t _nFactors;
    size_t _nUsers;
    size_t _nItems;
};

}

#include <memory>

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
		retuen nRows;
	}

	size_t getNumberOfColumns() const
	{
		retuen nCols;
	}

	const T& operator[](const size_t idx) const
	{
		return _data[idx];
	}

	T& operator[](const size_t idx)
	{
		return _data[idx];
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
	size_t _nRows;
	size_t _nCols;
};

tempale<typename T>
using TablePtr<T> = std::shared_ptr<Table<T>>;

/***************************************************
*
* file: main.cpp
*
* Copyright (C) Angela Burova and Egor Smirnov 2017
*
****************************************************
*/

#include <fstream>
#include <vector>
#include <cstdlib>

#include "matrix_operations.h"
#include "implicit_als_train.h"

using FPType = float;
TablePtr<FPType> readCsv(const char* const name);


int main()
{
    TablePtr<FPType> data = readCsv("als_train.csv");

    als::Train<FPType> algorithm;
    algorithm.parameter.nFactors = 3;
    algorithm.parameter.nIteration = 5;

    try
    {
        algorithm.compute(data);
    }
    catch (std::exception exc)
    {
        printf("exception: %s\n", exc.what());
        exit(-1);
    }
    auto model = algorithm.getModel();

    auto itemsFactors = model->getItemsFactors();
    auto usersFactors = model->getUsersFactors();

    printTable(itemsFactors, "ItemsFactors");
    printTable(usersFactors, "UsersFactors");
    printTable(utils::matrixMultiplication(utils::getTransposeMatrix(usersFactors), itemsFactors), "Result");

    return 0;
}

TablePtr<FPType> readCsv(const char* const name)
{
    std::ifstream fin(name);
    if (!fin)
    {
        printf("file was not found: %s\n", name);
        exit(-1);
    }

    fin.seekg(0, fin.end);
    size_t length = fin.tellg();
    fin.seekg(0, fin.beg);

    char* buff = (char*)malloc(length * sizeof(char));
    fin.read(buff, length);

    size_t nCols = 1;
    for (size_t i = 0; i < length; ++i)
    {
        if (buff[i] == ',') ++nCols;
        if (buff[i] == '\n' || buff[i] == '\r') break;
    }

    for (size_t i = 0; i < length; ++i)
        if (buff[i] == '\n' || buff[i] == ',' || buff[i] == '\r')
            buff[i] = '\0';

    std::vector<FPType> values;
    for (size_t i = 0; i < length; ++i)
    {
        values.push_back(std::atof(buff + i));
        while (i < length && buff[i] != '\0') ++i;
        while (i < length && buff[i] == '\0') ++i;
    }

    free(buff);
    const size_t nRows = values.size() / nCols;
    TablePtr<FPType> data(new Table<FPType>(nRows, nCols));

    for (size_t i = 0; i < nRows; ++i)
        for (size_t j = 0; j < nCols; ++j)
            (*data)[i*nCols + j] = values[i*nCols + j];

    return data;
}

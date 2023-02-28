#pragma once
#include <iostream>
#include <vector>
#include "Cell.h"
#include <cstdlib>

using namespace std;

double getRandomNumber(int min, int max);
void buildTable(vector<vector<Cell>> &costMat, int countSuppliers, int countConsumers);
void showTable(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers);
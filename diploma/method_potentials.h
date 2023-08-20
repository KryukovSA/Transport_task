#pragma once
#include <iostream>
#include <vector>
#include "Cell.h"
#include <cstdlib>

using namespace std;

double getRandomNumber(int min, int max);
void buildTable(vector<vector<Cell>> &costMat, int countSuppliers, int countConsumers);
void showTable(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers);
void methodMinElem(vector<vector<Cell>>& costMat, vector<double> stocks, vector<double> needs, bool closeTypeTask);
void showPostavki(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers);
double findMax(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers);
bool checkDegeneratePlan(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers);
void calculatePotencials(vector<vector<Cell>>& costMat, vector<double>& suppliersPotincials, vector<double>& ÒonsumerPotincials);
bool checkOptimal(vector<vector<Cell>>& costMat, vector<double>& suppliersPotincials, vector<double>& ÒonsumerPotincials);
void redistributionSupplies(vector<vector<Cell>>& costMat, vector<double>& suppliersPotincials, vector<double>& ÒonsumerPotincials);
double calculating—osts(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers);
void updateStatuses(vector<vector<Cell>>& costMat, vector<int>& indexIinChain, vector<int>& indexJinChain);
void addNullTransportation(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers);
bool ignoreCell(int indI, int indJ, vector<int> ignoreIndexI, vector<int> ignoreIndexJ);
bool checkCloseTypeTask(vector<double> stocks, vector<double> needs);
void addDataForClosingTask(vector<vector<Cell>>& costMat, vector<double>& stocks, vector<double>& needs, int& countSuppliers, int& countConsumers);
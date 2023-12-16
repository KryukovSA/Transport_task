#pragma once
#include <vector>
#include "Cell.h"
#include <iostream>
#include "libxl.h"



using namespace std;

class Method_potentials {

    int countSuppliers;
    int countConsumers;
    vector<double> stocks;
    vector<double> needs;
    bool closeTypeTask = true;
    vector<double> suppliersPotincials;
    vector<double> ÒonsumerPotincials;
    //vector<vector<Cell>>costMat(countSuppliers, vector<Cell>(countConsumers));
    vector<vector<Cell>>costMat;
public:
    int get_countConsumers() { return countConsumers; }
    int get_countSuppliers() { return countSuppliers; }
    void set_countConsumers(int countConsumers_) { countConsumers = countConsumers_; }
    void set_countSuppliers(int countSuppliers_) { countSuppliers = countSuppliers_; }
    const vector<vector<Cell>>& getCostMat() const {
        return costMat;
    }
    const vector<double>& get_suppliersPotincials() const { return suppliersPotincials; }
    const vector<double>& get_ÒonsumerPotincials() const { return ÒonsumerPotincials; }
    void Method_potentials_init();

    Method_potentials(int flag, int listNum);
    double getRandomNumber(int min, int max);
    void showTable();
    void methodMinElem();
    void showPostavki();
    void save_example();
    void generate_transport_task();
    double findMax();
    bool checkDegeneratePlan();
    void calculatePotencials();
    bool checkOptimal();
    void redistributionSupplies();
    double calculating—osts();
    void updateStatuses(vector<int>& indexIinChain, vector<int>& indexJinChain);
    void addNullTransportation();
    bool ignoreCell(int indI, int indJ, vector<int> ignoreIndexI, vector<int> ignoreIndexJ);
    bool checkCloseTypeTask();
    void addDataForClosingTask();
    void solve();
    int get_value(int i, int j);
};

bool containIndexes(int i, int j, int size, const vector<int>& indexIinChain, const vector<int>& indexJinChain);
#pragma once
#include <vector>
#include "Cell.h"
#include <iostream>
#include "libxl.h"
#include <omp.h>



using namespace std;

class Method_potentials {

    int countSuppliers;
    int countConsumers;
    vector<double> stocks;
    vector<double> needs;
    bool closeTypeTask = true;
    vector<double> suppliersPotincials;
    vector<double> сonsumerPotincials;
    //vector<vector<Cell>>costMat(countSuppliers, vector<Cell>(countConsumers));
    vector<vector<Cell>>costMat;
public:
    Method_potentials() {
        set_countSuppliers(400);
        set_countConsumers(400);
    }
    int get_countConsumers() { return countConsumers; }
    int get_countSuppliers() { return countSuppliers; }
    void set_countConsumers(int countConsumers_) { countConsumers = countConsumers_; }
    void set_countSuppliers(int countSuppliers_) { countSuppliers = countSuppliers_; }
    const vector<vector<Cell>>& getCostMat() const {
        return costMat;
    }
    const vector<double>& get_suppliersPotincials() const { return suppliersPotincials; }
    const vector<double>& get_сonsumerPotincials() const { return сonsumerPotincials; }
    //void Method_potentials_init();

    void method_potentials_init(int flag, int listNum, vector<vector<Cell>>& costMat_);
    double getRandomNumber(int min, int max);
    void showTable();
    void methodMinElem();
    void showPostavki();
    void save_example();


    vector<vector<Cell>> generate_transport_task();


    double findMax();
    bool checkDegeneratePlan();
    void calculatePotencials();

    bool checkOptimal();
    void redistributionSupplies();
    double calculatingСosts();
    void updateStatuses(vector<int>& indexIinChain, vector<int>& indexJinChain);
    void addNullTransportation();
    bool ignoreCell(int indI, int indJ, vector<int> ignoreIndexI, vector<int> ignoreIndexJ);
    bool checkCloseTypeTask();
    void addDataForClosingTask();
    void solve();
    int get_value(int i, int j);


    //для параллельной
    //метод минимального эл для электротранспорта
    void methodMinElem_parallel();

    //солве для параллельной
    void solve_parallel(int electric_count);

    void redistributionSupplies_parallel();

    void addNullTransportation_parallel();

    //помечает электроперевозки не более чем поставщиков
    void add_electric(int count);
    void redistributionSupplies_elctric();

    void calculatePotencials_parallel();

    void solve_electric_sequence(int electric_count);



};

bool containIndexes(int i, int j, int size, const vector<int>& indexIinChain, const vector<int>& indexJinChain);
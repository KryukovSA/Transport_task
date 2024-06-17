#pragma once
#include <vector>
#include "Cell.h"
#include <iostream>
#include "libxl.h"
#include <omp.h>
#include <unordered_set>
#include <regex>
#include <locale>
#include <codecvt>

using namespace std;

enum Direction {
    RIGHT, DOWN, LEFT, UP
};

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
    int electric_count = 0;
public:
    double clean_time = 0;
    Method_potentials() {
        set_countSuppliers(150);
        set_countConsumers(100);
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
    void showTarifs();
    void methodMinElem();
    void showPostavki();
    void showEstimation();
    void save_example();

    void updateStatuses1(vector<int>& indexIinChain, vector<int>& indexJinChain);
    vector<vector<Cell>> generate_transport_task();
    void redistributionSuppliesNewShema();
    void solve1();

    double findMax();
    bool checkDegeneratePlan();
    void calculatePotencials();

    void printLine() { for (int i = 0; i < 150; i++) { cout << "_"; } cout << endl; };
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
    void methodMinElem_electric();

    //солве для параллельной
    void solve_parallel(int electric_count, double  economic_koef);

    //работает с параллельным вычисл потенциалов
    void redistributionSupplies_parallel();

    //помечает электроперевозки не более чем поставщиков
    void add_electric(int count, double economic_koef);

    void redistributionSupplies_elctric();

    void correct_electric_tarifs(double economic_koef);

    void calculatePotencials_parallel();

    void solve_electric_sequence(int electric_count, double  economic_koef);

    void redistributionSuppliesNewShemaElectric();

    bool containIndexes1(int i, int j, const vector<int>& indexIinChain, const vector<int>& indexJinChain);

    void solve_for_bigsize();
    void solve_parallel_for_bigsize(int electric_count, double  economic_koef);
};

bool containIndexes(int i, int j, int size, const vector<int>& indexIinChain, const vector<int>& indexJinChain);
bool isForbidden(int i, int j, const unordered_set<int>& forbiddenCells, int countConsumers);
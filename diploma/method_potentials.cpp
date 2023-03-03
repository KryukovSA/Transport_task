#include "method_potentials.h"
#include <iostream>

//запасы - строки, потребности столбцы
double getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<double>(rand() * fraction * (max - min + 1) + min);
}


void buildTable(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            costMat[i][j].set_tarif(static_cast<int>(getRandomNumber(2, 15)));
        }
    }
}


void showTable(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            std::cout << costMat[i][j].get_tarif() << " "; 
        }
        cout << endl;
    }
}

void methodMinElem(vector<vector<Cell>>& costMat, vector<double> stocks, vector<double> needs) {
    int min = costMat[0][0].get_tarif();
    int minIndexI = 0;
    int minIndexJ = 0;
    int a = 0;
    vector<int> busyStrIndex;
    vector<int> busyColIndex;
    while (true) {
        a++;
        for (int i = 0; i < stocks.size(); i++) {
            if (!count(busyStrIndex.begin(), busyStrIndex.end(), i))
                for (int j = 0; j < needs.size(); j++) {
                    if (costMat[i][j].get_tarif() <= min && costMat[i][j].get_status() == free_ && !count(busyColIndex.begin(), busyColIndex.end(), j)) {
                        minIndexI = i;//минимум тут  после первого не обновляется
                        minIndexJ = j;
                        min = costMat[i][j].get_tarif();
                    }
                }
        }
        if (stocks[minIndexI] > needs[minIndexJ] && needs[minIndexJ] != 0) {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs[minIndexJ]);
            stocks[minIndexI] -= needs[minIndexJ];
            needs[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);
        }
        else if(stocks[minIndexI] == needs[minIndexJ] && needs[minIndexJ] != 0){
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs[minIndexJ]);
            stocks[minIndexI] -= needs[minIndexJ];
            needs[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);
            busyStrIndex.push_back(minIndexI);
        }
        else {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(stocks[minIndexI]);
            needs[minIndexJ] -= stocks[minIndexI];
            stocks[minIndexI] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyStrIndex.push_back(minIndexI);
        }
        showPostavki(costMat, stocks.size(), needs.size());
        cout << endl;
        min = findMax(costMat, stocks.size(), needs.size());
        if (busyStrIndex.size() == stocks.size() && busyColIndex.size() == needs.size()) {
            return;
        }
    }
}

void showPostavki(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    for (int i = 0; i < countSuppliers; i++)
    {
        for (int j = 0; j < countConsumers; j++)
            std::cout << costMat[i][j].get_cargoVolueme() << " ";
        cout << endl;
    }
}

double findMax(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    double max = costMat[0][0].get_tarif();
    for (int i = 0; i < countSuppliers; i++)
    {
        for (int j = 0; j < countConsumers; j++) {
            if (costMat[i][j].get_tarif() >= max)
                max = costMat[i][j].get_tarif();
        }
    }
    return max;
}
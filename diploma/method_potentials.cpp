#include "method_potentials.h"
#include <iostream>

//çàïàñû - ñòðîêè, ïîòðåáíîñòè ñòîëáöû
double getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<double>(rand() * fraction * (max - min + 1) + min);
}


void buildTable(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    costMat[0][0].set_tarif(7);
    costMat[0][1].set_tarif(8);
    costMat[0][2].set_tarif(1);
    costMat[0][3].set_tarif(2);
    costMat[1][0].set_tarif(4);
    costMat[1][1].set_tarif(5); 
    costMat[1][2].set_tarif(9);
    costMat[1][3].set_tarif(8);
    costMat[2][0].set_tarif(9);
    costMat[2][1].set_tarif(2); 
    costMat[2][2].set_tarif(3);
    costMat[2][3].set_tarif(6); 

    //for (int i = 0; i < countSuppliers; i++) {
    //    for (int j = 0; j < countConsumers; j++) {
    //        costMat[i][j].set_tarif(static_cast<int>(getRandomNumber(2, 15)));
    //    }
    //}
}


void showTable(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    cout << "\n";
    cout << "tarifs" << endl;
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            std::cout << costMat[i][j].get_tarif() << " "; 
        }
        cout << endl;
    }
    cout << endl;
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
                        minIndexI = i;//ìèíèìóì òóò  ïîñëå ïåðâîãî íå îáíîâëÿåòñÿ
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

bool checkDegeneratePlan(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    int countBasisCell = 0;
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countSuppliers; j++) {
            if (costMat[i][j].get_status() == basic)
                countBasisCell++;
        }
    }
    if (countBasisCell < countConsumers + countSuppliers - 1) {
        return true;
    }
    return false;
}

void calculatePotencials(vector<vector<Cell>>& costMat, vector<double> &suppliersPotincials, vector<double> &ñonsumerPotincials) {
    double identefikator = -100.1;

    for (int i = 0; i < suppliersPotincials.size(); i++) {
        suppliersPotincials[i] = identefikator;
    }
    for (int i = 0; i < ñonsumerPotincials.size(); i++) {
        ñonsumerPotincials[i] = identefikator;
    }
    int counter = 0;
    suppliersPotincials[0] = 0;
    while (counter< suppliersPotincials.size()-1) {
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            if (suppliersPotincials[i] != identefikator) {
                for (int j = 0; j < ñonsumerPotincials.size(); j++)
                {
                    if (costMat[i][j].get_status() == basic)
                        ñonsumerPotincials[j] = costMat[i][j].get_tarif() - suppliersPotincials[i];
                }
            }
            if (suppliersPotincials[i] == identefikator) {
                for (int j = 0; j < ñonsumerPotincials.size(); j++)
                {
                    if (costMat[i][j].get_status() == basic && ñonsumerPotincials[j] != identefikator)
                        suppliersPotincials[i] = costMat[i][j].get_tarif() - ñonsumerPotincials[j];
                }
            }
        }
        counter++;
    }
}


bool checkOptimal(vector<vector<Cell>>& costMat, vector<double>& suppliersPotincials, vector<double>& ñonsumerPotincials) {

    bool optimal = true;
    for (int i = 0; i < suppliersPotincials.size(); i++) {
        for (int j = 0; j < ñonsumerPotincials.size(); j++) {
            if (costMat[i][j].get_status() == free_)
                costMat[i][j].set_defferncTarifAndPotincials(costMat[i][j].get_tarif() - suppliersPotincials[i] - ñonsumerPotincials[j]);
        }
    }

    for (int i = 0; i < suppliersPotincials.size(); i++) {
        for (int j = 0; j < ñonsumerPotincials.size(); j++) {
            if (costMat[i][j].get_status() == free_) {
                if (costMat[i][j].get_defferncTarifAndPotincials() >= 0)
                    continue;
                else
                    optimal = false;
            }
              
        }
    }
    return optimal;

}

void redistributionSupplies(vector<vector<Cell>>& costMat, vector<double>& suppliersPotincials, vector<double>& ñonsumerPotincials) {
    if (checkOptimal(costMat, suppliersPotincials, ñonsumerPotincials) == false) {
        Cell min = costMat[0][0];
        int minIndexI = 0;
        int minIndexJ = 0;
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            for (int j = 0; j < ñonsumerPotincials.size(); j++) {
                if (costMat[i][j].get_status() == free_)
                    if (costMat[i][j].get_defferncTarifAndPotincials() < min.get_defferncTarifAndPotincials()) {
                        min = costMat[i][j];
                        minIndexI = i;
                        minIndexJ = j;
                    }
            }
        }
        //build halfchain
        //öèêë êàê òî ñòðîèì +- âûäåëÿåì, âñå êëåòêè ñ ìèíèìàëüíîé ïàñòàâêîé ñòóíóò íóëåâûìè
        costMat[minIndexI][minIndexJ].set_signInHalfChain(positive);






    }
    else {
        return;
    }
}


#include "method_potentials.h"
#include <iostream>



double getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<double>(rand() * fraction * (max - min + 1) + min);
}

void buildTable(vector<vector<Cell>> &costMat, int countSuppliers, int countConsumers) {
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            costMat[i][j].set_tarif(static_cast<int>(getRandomNumber(2, 9)));
        }
    }
}

void showTable(vector<vector<Cell>>& costMat, int countSuppliers, int countConsumers) {
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {

            cout << costMat[i][j].get_tarif() << endl;
        }
    }
}
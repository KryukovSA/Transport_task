#include <ctime>
#include "method_potentials.h"
#include "Cell.h"
#include <fstream>



using namespace std;


int main() {

    int countSuppliers;
    int countConsumers;

    ifstream out("zapas.txt");double k = 0;
    ifstream need("needs.txt");
    cout << "input count Suppliers" << "\n";
    cin >> countSuppliers;
    cout << "input count Consumers" << "\n";
    cin >> countConsumers;
    vector<double> stocks;
    vector<double> needs;
    bool closeTypeTask = true;
    while (!out.eof()) {
        out >> k;
        stocks.push_back(k);
        cout <<" zapas from file "<< k << endl;
    }
    while (!need.eof()) {
        need >> k;
        needs.push_back(k);
        cout << " potrebnosti from file " << k << endl;
    }
   

    srand(static_cast<unsigned int>(time(0)));

    vector<Cell> v(countConsumers);
    vector < vector<Cell>> costMat(countSuppliers, v);

    buildTable(costMat, countSuppliers, countConsumers);
    showTable(costMat, countSuppliers, countConsumers);
    if (!checkCloseTypeTask(stocks, needs)) {//если не закрытая
        addDataForClosingTask(costMat, stocks, needs, countSuppliers, countConsumers);
        closeTypeTask = false;
    }
    methodMinElem(costMat, stocks, needs, closeTypeTask);
    showPostavki(costMat, countSuppliers, countConsumers);//первый опорный план

    cout << "result cost after minimal elem method: " << calculatingСosts(costMat, countSuppliers, countConsumers) << endl;
    if (checkDegeneratePlan(costMat, countSuppliers, countConsumers)) {//вырожденность убирает
        addNullTransportation(costMat, countSuppliers, countConsumers);
    }
    vector<double> suppliersPotincials(countSuppliers); //U
    vector<double> сonsumerPotincials(countConsumers); //V
    calculatePotencials(costMat, suppliersPotincials, сonsumerPotincials);
    
    redistributionSupplies(costMat, suppliersPotincials, сonsumerPotincials);//до тех пор пока не будет оптимальным, вычисления потенциалов внутри метода

    cout << "result minimal cost: " << calculatingСosts(costMat, countSuppliers, countConsumers) << endl;


    return 0;
}
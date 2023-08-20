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
    if (!checkCloseTypeTask(stocks, needs)) {//���� �� ��������
        addDataForClosingTask(costMat, stocks, needs, countSuppliers, countConsumers);
        closeTypeTask = false;
    }
    methodMinElem(costMat, stocks, needs, closeTypeTask);
    showPostavki(costMat, countSuppliers, countConsumers);//������ ������� ����

    cout << "result cost after minimal elem method: " << calculating�osts(costMat, countSuppliers, countConsumers) << endl;
    if (checkDegeneratePlan(costMat, countSuppliers, countConsumers)) {//������������� �������
        addNullTransportation(costMat, countSuppliers, countConsumers);
    }
    vector<double> suppliersPotincials(countSuppliers); //U
    vector<double> �onsumerPotincials(countConsumers); //V
    calculatePotencials(costMat, suppliersPotincials, �onsumerPotincials);
    
    redistributionSupplies(costMat, suppliersPotincials, �onsumerPotincials);//�� ��� ��� ���� �� ����� �����������, ���������� ����������� ������ ������

    cout << "result minimal cost: " << calculating�osts(costMat, countSuppliers, countConsumers) << endl;


    return 0;
}
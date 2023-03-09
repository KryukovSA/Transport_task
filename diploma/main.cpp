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
    vector<double> suppliersPotincials(countSuppliers); //U
    vector<double> ñonsumerPotincials(countConsumers); //V

    srand(static_cast<unsigned int>(time(0)));

    vector<Cell> v(countConsumers);
    vector < vector<Cell>> costMat(countSuppliers, v);

    buildTable(costMat, countSuppliers, countConsumers);
    showTable(costMat, countSuppliers, countConsumers);
    methodMinElem(costMat, stocks, needs);
    showPostavki(costMat, countSuppliers, countConsumers);//ïåğâûé îïîğíûé ïëàí
    //òóò åùå ïğîâåğèòü íà âûğîæäåííîñòü
    calculatePotencials(costMat, suppliersPotincials, ñonsumerPotincials);
    for (int i = 0; i < suppliersPotincials.size(); i++) {
        cout << suppliersPotincials[i] << " ";
    }
    cout << endl;
    
    for (int i = 0; i < ñonsumerPotincials.size(); i++)
        cout << ñonsumerPotincials[i] << " ";


    std::cout << "hello";

    return 0;
}
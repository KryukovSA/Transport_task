#include <ctime>
#include "method_potentials.h"
#include "Cell.h"


using namespace std;




int main() {

    int countSuppliers;
    int countConsumers;

    cout << "input count Suppliers" << "\n";
    cin >> countSuppliers;
    cout << "input count Consumers" << "\n";
    cin >> countConsumers;
    vector<double> stocks(countSuppliers);
    vector<double> needs(countConsumers);

    srand(static_cast<unsigned int>(time(0)));
    //for (int i = 0; i < stocks.size(); i++) {
    //    stocks[i] = static_cast<int>(getRandomNumber(5, 20));
    //    cout << stocks[i] << endl;
    //} 
    // 
    //manual input
    cout << "input Suppliers" << "\n";
    for (int i = 0; i < stocks.size(); i++) {
        cin >> stocks[i];
    }
    cout << "input Consumers" << "\n";
    for (int i = 0; i < needs.size(); i++) {
        cin >> needs[i];
    }

    vector<Cell> v(countConsumers);
    vector < vector<Cell>> costMat(countSuppliers, v);

    buildTable(costMat, countSuppliers, countConsumers);
    showTable(costMat, countSuppliers, countConsumers);




    std::cout << "hello";

    return 0;
}
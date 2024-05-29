#include <SFML/Graphics.hpp>
#include <sstream> 
#include <fstream>
#include <iomanip>
#include <vector>
#include "Method_potentials.h" // Подключите ваш заголовочный файл Method_potentials.h
#include"math.h"


int main() {
    int num_procs = omp_get_num_procs();
    //std::cout << "Number of available processors: " << num_procs << std::endl;
    omp_set_num_threads(4);


    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));
    Method_potentials method_potencials;
    int electric_count = 5;//количество не более половины числа потербителей например
    int flag = 0;// 1 - из табл тарифы     0 - автогенерация
    method_potencials.set_countSuppliers(200);
    method_potencials.set_countConsumers(200);


    vector<vector<int>> result_volumes(method_potencials.get_countSuppliers(), std::vector<int>(method_potencials.get_countConsumers()));
    vector<vector<Cell>> main_matrix;
    if (flag == 0)
        main_matrix = method_potencials.generate_transport_task();


    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve1();
    for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
        for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
            result_volumes[i][j] = method_potencials.get_value(i, j);
        }
    }

    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve();
    for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
        for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
            result_volumes[i][j] = method_potencials.get_value(i, j);
        }
    }

    return 0;
}
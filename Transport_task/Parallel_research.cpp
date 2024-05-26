//#include <sstream> 
//#include <fstream>
//#include <iomanip>
//#include <vector>
//#include "Method_potentials.h" // Подключите ваш заголовочный файл Method_potentials.h
//#include"math.h"
//
//
//int main() {
//    setlocale(LC_ALL, "Russian");
//
//    srand(static_cast<unsigned int>(time(0)));
//    Method_potentials method_potencials;
//    int electric_count = 250;//количество не более половины числа потербителей например
//    int flag = 0;// 1 - из табл тарифы     0 - автогенерация
//
//    if (flag == 0) {
//        method_potencials.set_countSuppliers(600);
//        method_potencials.set_countConsumers(600);
//    }
//
//    vector<vector<Cell>> main_matrix;
//    if (flag == 0)
//        main_matrix = method_potencials.generate_transport_task();
//
//    omp_set_num_threads(1);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(2);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(3);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(4);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(5);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(6);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(7);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(8);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(9);  
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(10);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(11);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//    method_potencials.clean_time = 0;
//    omp_set_num_threads(12);
//    method_potencials.method_potentials_init(flag, 7, main_matrix);
//    method_potencials.solve_parallel(electric_count, 0.4);
//    cout << "clean time: " << method_potencials.clean_time << " second." << std::endl;
//
//
//}

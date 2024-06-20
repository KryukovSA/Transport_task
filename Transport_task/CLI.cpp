//#include <SFML/Graphics.hpp>
//#include <sstream> 
//#include <fstream>
//#include <iomanip>
//#include <vector>
//#include "Method_potentials.h" // Подключите ваш заголовочный файл Method_potentials.h
//#include"math.h"
//
//
//int main() {
//    int num_procs = omp_get_num_procs();
//    //std::cout << "Number of available processors: " << num_procs << std::endl;
//    omp_set_num_threads(2);
//
//
//    setlocale(LC_ALL, "Russian");
//    srand(static_cast<unsigned int>(time(0)));
//    Method_potentials method_potencials_standart;
//    Method_potentials method_potencials_electric;
//    int electric_count = 5;//количество не более половины числа потербителей например
//    int flag = 1;// 1 - из табл тарифы     0 - автогенерация
//    vector<vector<Cell>> main_matrix;
//    method_potencials_standart.method_potentials_init(flag, 8, main_matrix);
//
//
//    cout << "Решение транспортной задачи с одним видом транспорта" << endl;
//    method_potencials_standart.printLine();
//    cout << "Исходные данные: " << endl;
//    method_potencials_standart.showTarifs();
//    method_potencials_standart.solve_standart_transport();
//
//
//
//    float econom_parametr = 0.6;
//    while (true) {
//        cout << "С каким параметром экономической выгоды вы хотите увидеть оптимальный план? (1 если без него)" << endl;
//        cin >> econom_parametr;
//        if (cin.fail() || econom_parametr < 0.1 || econom_parametr > 1.0) {
//            cin.clear(); // Сброс состояния потока
//            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода
//            cout << "Пожалуйста, введите число в диапазоне от 0.1 до 1.0." << endl;
//        }
//        else {
//            break; // Корректное значение введено, выход из цикла
//        }
//    }
//
//    method_potencials_electric.method_potentials_init(flag, 9, main_matrix);
//    cout << "Решение транспортной задачи с двумя видами транспорта" << endl;
//    //method_potencials_electric.printLine();
//    //cout << "Исходные данные: " << endl;
//    //method_potencials_electric.showTarifs();
//    method_potencials_electric.solve_parallel(0,econom_parametr);
//
//
//    cout << endl;
//
//    return 0;
//}

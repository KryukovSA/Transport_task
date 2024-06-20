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


    if (flag == 0) {
        int a, b;
        cout << "введите количество поставщиков" << endl;
        cin >> a;
        method_potencials.set_countSuppliers(a);
        cout << "введите количество потребителей" << endl;
        cin >> b;
        method_potencials.set_countConsumers(b);
    }

    // Проверка на ввод неотрицательного числа электрогрузовиков
    while (true) {
        cout << "какое количество электрогрузовиков планируется задействовать?" << endl;
        cin >> electric_count;
        if (cin.fail() || electric_count < 0) {
            cin.clear(); // Сброс состояния потока
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода
            cout << "Пожалуйста, введите неотрицательное целое число." << endl;
        }
        else {
            break; // Корректное значение введено, выход из цикла
        }
    }

    float econom_parametr = 0.6;
    while (true) {
        cout << "С каким параметром экономической выгоды вы хотите увидеть оптимальный план? (1 если без него)" << endl;
        cin >> econom_parametr;
        if (cin.fail() || econom_parametr < 0.4 || econom_parametr > 1.0) {
            cin.clear(); // Сброс состояния потока
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода
            cout << "Пожалуйста, введите число в диапазоне от 0.4 до 1.0." << endl;
        }
        else {
            break; // Корректное значение введено, выход из цикла
        }
    }

    vector<vector<int>> result_volumes(method_potencials.get_countSuppliers(), std::vector<int>(method_potencials.get_countConsumers()));
    vector<vector<Cell>> main_matrix;
    if(flag == 0)
        main_matrix = method_potencials.generate_transport_task();


   method_potencials.method_potentials_init(flag, 8, main_matrix); 
   method_potencials.solve_for_bigsize();
   if (econom_parametr - 0.001 < 1 && econom_parametr + 0.001 > 1) {//так как функции округления не работают
       for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
           for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
               result_volumes[i][j] = method_potencials.get_value(i, j);
           }
       }
   }
    //method_potencials.method_potentials_init(flag, 7, main_matrix);
    //method_potencials.solve_electric_sequence(electric_count);
   
    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve_parallel_for_bigsize(electric_count, 0.9);
    if (econom_parametr-0.001 < 0.9  && econom_parametr + 0.001 > 0.9) {
        for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
            for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
                result_volumes[i][j] = method_potencials.get_value(i, j);
            }
        }
    }

    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve_parallel_for_bigsize(electric_count, 0.8);
    if (econom_parametr - 0.001 < 0.8 && econom_parametr + 0.001 > 0.8) {
        for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
            for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
                result_volumes[i][j] = method_potencials.get_value(i, j);
            }
        }
    }

    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve_parallel_for_bigsize(electric_count, 0.7);
    if (econom_parametr - 0.001 < 0.7 && econom_parametr + 0.001 > 0.7) {
        for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
            for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
                result_volumes[i][j] = method_potencials.get_value(i, j);
            }
        }
    }

    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve_parallel_for_bigsize(electric_count, 0.6);
    if (econom_parametr - 0.001 < 0.6 && econom_parametr + 0.001 > 0.6) {
        for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
            for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
                result_volumes[i][j] = method_potencials.get_value(i, j);
            }
        }
    }

    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve_parallel_for_bigsize(electric_count, 0.5);
    if (econom_parametr - 0.001 < 0.5 && econom_parametr + 0.001 > 0.5) {
        for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
            for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
                result_volumes[i][j] = method_potencials.get_value(i, j);
            }
        }
    }


    method_potencials.method_potentials_init(flag, 8, main_matrix);
    method_potencials.solve_parallel_for_bigsize(electric_count, 0.4);
    if (econom_parametr - 0.001 < 0.4 && econom_parametr + 0.001 > 0.4) {
        for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
            for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
                result_volumes[i][j] = method_potencials.get_value(i, j);
            }
        }
    }
    //-------------------------------------ИНТЕРФЕЙС---------------------------------------
    //method_potencials.method_potentials_init();
    std::cin.ignore(); 
    cout << "Вычисления завершены " << endl << endl << endl;
    //method_potencials.solve();
    while (true) {
        cout << "Выбериет режим вывода" << endl << endl;
        cout << "1 В формате матрицы" << endl;
        cout << "2 Просмотр конкретного потребителя" << endl;
        cout << "Введите '0', чтобы выйти." << endl;
        int regim = 0;
        cin >> regim;

        if (regim == 0) {
            cout << "Программа завершена." << endl;
            break;
        }

        switch (regim)
        {
        case 1: {
            while (true) {
                cout << "Введите диапазоны значений для отображения оптимальных объемов например (100 - 110) (10 - 30) " << endl;//потом он вводит напрмиер (100 - 110) (10 - 30)
                std::string input;
            
                //std::cin.ignore();//(40 - 49) (40 - 49)
                std::getline(std::cin >> std::ws, input);
                
                if (input == "exit") {
                    //std::cout << "Программа завершена." << std::endl;
                    break; // Выход из цикла
                }
                std::pair<int, int> rowRange, colRange;
                //std::stringstream ss(input);
       
                std::istringstream ss(input);
                char delimiter;
                int rowLower, rowUpper, colLower, colUpper;

                ss >> delimiter >> rowLower >> delimiter >> rowUpper >> delimiter;
                rowRange = std::make_pair(rowLower, rowUpper);

                // Чтение диапазона столбцов
                ss >> delimiter >> colLower >> delimiter >> colUpper >> delimiter;
                colRange = std::make_pair(colLower, colUpper);
                std::cout << "Диапазон для строк: " << rowRange.first << " - " << rowRange.second << std::endl;
                std::cout << "Диапазон для столбцов: " << colRange.first << " - " << colRange.second << std::endl;

                std::cout << "Подматрица, соответствующая диапазонам:\n";
                for (int i = rowRange.first; i <= rowRange.second; i++) {
                    for (int j = colRange.first; j <= colRange.second; j++) {
                        std::cout << result_volumes[i][j] << "\t";
                    }
                    std::cout << std::endl;
                }
                std::cout << "Введите 'exit', чтобы выйти в главное меню, или продолжите вводить диапазоны." << std::endl;
            }
            break;
        }
        case 2: {
            while (true) {
                cout << "Введите идентификатор потребителя (или -1 для выхода)" << endl;
                int consumer_id;
                cin >> consumer_id;
                if (consumer_id == -1) {
                    break;
                }
                cout << "id   volume" << endl;
                for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
                    if (result_volumes[i][consumer_id] > 0) {
                        cout << i << "   " << result_volumes[i][consumer_id] << endl;
                    }
                }
            }
            break;
        }
        default: {
            cout << "Неверный режим. Пожалуйста, выберите 1 или 2." << endl;
            break;
        }
        }
    }
    return 0;
}

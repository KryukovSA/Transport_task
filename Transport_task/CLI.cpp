#include <SFML/Graphics.hpp>
#include <sstream> 
#include <fstream>
#include <iomanip>
#include <vector>
#include "Method_potentials.h" // ���������� ��� ������������ ���� Method_potentials.h
#include"math.h"


int main() {
    int num_procs = omp_get_num_procs();
    //std::cout << "Number of available processors: " << num_procs << std::endl;
    omp_set_num_threads(4);


    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));
    Method_potentials method_potencials_standart;
    Method_potentials method_potencials_electric;
    int electric_count = 5;//���������� �� ����� �������� ����� ������������ ��������
    int flag = 1;// 1 - �� ���� ������     0 - �������������
    vector<vector<Cell>> main_matrix;
    method_potencials_standart.method_potentials_init(flag, 8, main_matrix);


    cout << "������� ������������ ������ � 1 ����� ����������" << endl;
    method_potencials_standart.printLine();
    cout << "�������� ������: " << endl;
    method_potencials_standart.showTarifs();
    method_potencials_standart.solve1();












    float econom_parametr = 0.6;
    while (true) {
        cout << "� ����� ���������� ������������� ������ �� ������ ������� ����������� ����? (1 ���� ��� ����)" << endl;
        cin >> econom_parametr;
        if (cin.fail() || econom_parametr < 0.4 || econom_parametr > 1.0) {
            cin.clear(); // ����� ��������� ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ������ �����
            cout << "����������, ������� ����� � ��������� �� 0.4 �� 1.0." << endl;
        }
        else {
            break; // ���������� �������� �������, ����� �� �����
        }
    }

    //vector<vector<int>> result_volumes(method_potencials.get_countSuppliers(), std::vector<int>(method_potencials.get_countConsumers()));
    //vector<vector<Cell>> main_matrix;
    //if (flag == 0)
    //    main_matrix = method_potencials.generate_transport_task();


    //method_potencials.method_potentials_init(flag, 8, main_matrix);
    //method_potencials.solve1();
    //if (econom_parametr - 0.001 < 1 && econom_parametr + 0.001 > 1) {//��� ��� ������� ���������� �� ��������
    //    for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
    //        for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
    //            result_volumes[i][j] = method_potencials.get_value(i, j);
    //        }
    //    }
    //}
    ////method_potencials.method_potentials_init(flag, 7, main_matrix);
    ////method_potencials.solve_electric_sequence(electric_count);

    //method_potencials.method_potentials_init(flag, 8, main_matrix);
    //method_potencials.solve_parallel(electric_count, 0.9);
    //if (econom_parametr - 0.001 < 0.9 && econom_parametr + 0.001 > 0.9) {
    //    for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
    //        for (int j = 0; j < method_potencials.get_countConsumers(); j++) {
    //            result_volumes[i][j] = method_potencials.get_value(i, j);
    //        }
    //    }
    //}



    ////-------------------------------------���������---------------------------------------
    ////method_potencials.method_potentials_init();
    //std::cin.ignore();
    //cout << "���������� ��������� " << endl << endl << endl;
    ////method_potencials.solve();
    //while (true) {
    //    cout << "�������� ����� ������" << endl << endl;
    //    cout << "1 � ������� �������" << endl;
    //    cout << "2 �������� ����������� �����������" << endl;
    //    cout << "������� '0', ����� �����." << endl;
    //    int regim = 0;
    //    cin >> regim;

    //    if (regim == 0) {
    //        cout << "��������� ���������." << endl;
    //        break;
    //    }

    //    switch (regim)
    //    {
    //    case 1: {
    //        while (true) {
    //            cout << "������� ��������� �������� ��� ����������� ����������� ������� �������� (100 - 110) (10 - 30) " << endl;//����� �� ������ �������� (100 - 110) (10 - 30)
    //            std::string input;

    //            //std::cin.ignore();//(40 - 49) (40 - 49)
    //            std::getline(std::cin >> std::ws, input);

    //            if (input == "exit") {
    //                //std::cout << "��������� ���������." << std::endl;
    //                break; // ����� �� �����
    //            }
    //            std::pair<int, int> rowRange, colRange;
    //            //std::stringstream ss(input);

    //            std::istringstream ss(input);
    //            char delimiter;
    //            int rowLower, rowUpper, colLower, colUpper;

    //            ss >> delimiter >> rowLower >> delimiter >> rowUpper >> delimiter;
    //            rowRange = std::make_pair(rowLower, rowUpper);

    //            // ������ ��������� ��������
    //            ss >> delimiter >> colLower >> delimiter >> colUpper >> delimiter;
    //            colRange = std::make_pair(colLower, colUpper);
    //            std::cout << "�������� ��� �����: " << rowRange.first << " - " << rowRange.second << std::endl;
    //            std::cout << "�������� ��� ��������: " << colRange.first << " - " << colRange.second << std::endl;

    //            std::cout << "����������, ��������������� ����������:\n";
    //            for (int i = rowRange.first; i <= rowRange.second; i++) {
    //                for (int j = colRange.first; j <= colRange.second; j++) {
    //                    std::cout << result_volumes[i][j] << "\t";
    //                }
    //                std::cout << std::endl;
    //            }
    //            std::cout << "������� 'exit', ����� ����� � ������� ����, ��� ���������� ������� ���������." << std::endl;
    //        }
    //        break;
    //    }
    //    case 2: {
    //        while (true) {
    //            cout << "������� ������������� ����������� (��� -1 ��� ������)" << endl;
    //            int consumer_id;
    //            cin >> consumer_id;
    //            if (consumer_id == -1) {
    //                break;
    //            }
    //            cout << "id   volume" << endl;
    //            for (int i = 0; i < method_potencials.get_countSuppliers(); i++) {
    //                if (result_volumes[i][consumer_id] > 0) {
    //                    cout << i << "   " << result_volumes[i][consumer_id] << endl;
    //                }
    //            }
    //        }
    //        break;
    //    }
    //    default: {
    //        cout << "�������� �����. ����������, �������� 1 ��� 2." << endl;
    //        break;
    //    }
    //    }
    //}
    return 0;
}

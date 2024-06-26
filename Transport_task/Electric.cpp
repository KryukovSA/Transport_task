#include"Method_potentials.h"
#include <fstream>
#include<numeric>
#include <chrono>
#include <iomanip>

//����� ������������ �� ��� �����������������
//�������  ������� ������� � ��������� ��� � ������� ����� �����������
void Method_potentials::methodMinElem_electric() {
    vector<double> stocks_ = stocks;
    vector<double> needs_ = needs;
    int min = costMat[0][1].get_tarif();
    int minIndexI = 0;
    int minIndexJ = 0;
    int a = 0;
    vector<int> busyStrIndex;
    vector<int> busyColIndex;

    for (int i = 0; i < stocks_.size(); i++) {
        for (int j = 0; j < needs_.size(); j++) {
            if (costMat[i][j].get_electric() == true) {//���������� �������������o���
                if (stocks_[i] > needs_[j] && needs_[j] != 0) {
                    costMat[i][j].set_cargoVolueme(needs_[j]);
                    stocks_[i] -= needs_[j];//�������� ����� ��������� �������
                    needs_[j] = 0;
                    costMat[i][j].set_status(basic);
                    busyColIndex.push_back(j);
                }
                else if (stocks_[i] == needs_[j] && needs_[j] != 0) {
                    costMat[i][j].set_cargoVolueme(needs_[j]);
                    stocks_[i] -= needs_[j];
                    needs_[j] = 0;
                    costMat[i][j].set_status(basic);
                    busyColIndex.push_back(j);//������������� �����������
                    busyStrIndex.push_back(i);// ������ ��������� � ������ ����� �� �������������
                }
                else {
                    costMat[i][j].set_cargoVolueme(stocks_[i]);
                    needs_[j] -= stocks_[i];
                    stocks_[i] = 0;
                    costMat[i][j].set_status(basic);
                    busyStrIndex.push_back(i);
                }
            }
        }
    }






    while (true) {
        a++;
        for (int i = 0; i < stocks_.size(); i++) {//���� � ����� � ����� ��������� ������� � �������
            if (!count(busyStrIndex.begin(), busyStrIndex.end(), i))//���� ������ �� ������������ ��� (false ����� count 0 ������)
                for (int j = 0; j < needs_.size(); j++) {
                    if (costMat[i][j].get_tarif() <= min && costMat[i][j].get_status() == free_ && !count(busyColIndex.begin(), busyColIndex.end(), j)) {
                        minIndexI = i;//������� ���  ����� ������� �� �����������
                        minIndexJ = j;
                        min = costMat[i][j].get_tarif();
                    }
                }
        }
        if (stocks_[minIndexI] > needs_[minIndexJ] && needs_[minIndexJ] != 0) {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs_[minIndexJ]);
            stocks_[minIndexI] -= needs_[minIndexJ];//�������� ����� ��������� �������
            needs_[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);
        }
        else if (stocks_[minIndexI] == needs_[minIndexJ] && needs_[minIndexJ] != 0) {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs_[minIndexJ]);
            stocks_[minIndexI] -= needs_[minIndexJ];
            needs_[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);//������������� �����������
            busyStrIndex.push_back(minIndexI);// ������ ��������� � ������ ����� �� �������������
        }
        else {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(stocks_[minIndexI]);
            needs_[minIndexJ] -= stocks_[minIndexI];
            stocks_[minIndexI] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyStrIndex.push_back(minIndexI);
        }
        //showPostavki(costMat, stocks.size(), needs.size());
        //cout << endl;
        min = findMax();
        if (busyStrIndex.size() == stocks_.size() && busyColIndex.size() == needs_.size()) {
            if (closeTypeTask == false) {//� ��������� ������ ������� ������(���� ������ ���� ������ ������)
                for (int k = 0; k < stocks_.size(); k++) {
                    costMat[k][needs_.size() - 1].set_tarif(0);
                }
            }
            return;
        }
    }

}





//����� ��� ������������
//������ ����������� ���������
void Method_potentials::solve_parallel(int electric_count, double  economic_koef) {
    if (!checkCloseTypeTask()) {//���� �� ��������
        addDataForClosingTask();
        closeTypeTask = false;
    }

    //std::cout << std::fixed << std::setprecision(5);
     static int num = 0;
    //add_electric(electric_count, economic_koef); //��� ��������� ��������� �����
     this->correct_electric_tarifs(economic_koef);
     printLine();
     cout << "�������� ������: " << endl;
     this->showTarifs();
    
     methodMinElem_electric();
    if (num == 0) {
        printLine();
        cout << "������� ���� " << endl;
        this->showPostavki();//������ ������� ����
    }
    
    cout << "\nC�������� ��������� � ������� ����� ����������: " << fixed << static_cast<long long>(calculating�osts()) << endl;


    num++;  
    //{
    //    std::cout << std::fixed << std::setprecision(1);
    //    std::cout << "��������� � ������������ �����������: " << economic_koef << std::endl;
    //}
    //showPostavki();//������ ������� ����

    //cout << "result cost after minimal elem method: " << calculating�osts() << endl;
    while (checkDegeneratePlan()) {//������������� �������
        addNullTransportation();
    }
    //calculatePotencials();
    calculatePotencials_parallel();
    auto start_time = std::chrono::high_resolution_clock::now();
    //redistributionSupplies_parallel(); //old
    redistributionSuppliesElectric();
    showEstimation();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    printLine();
    cout << "����������� ����:";
    showPostavki();
    cout << "\n������������ " <<this->electric_count<< " �����������������" << endl;
    std::cout << std::fixed << std::setprecision(1);
    cout << "��������� ��������� � ������������ �����������: " << economic_koef <<" ���������� "<< fixed << static_cast<long long>(calculating�osts()) << endl;
    //std::cout << "execution time: " << duration.count() << " second." << std::endl;
    //showPostavki();
    cout << endl;
}

void Method_potentials::solve_parallel_for_bigsize(int electric_count, double  economic_koef) {
    if (!checkCloseTypeTask()) {//���� �� ��������
        addDataForClosingTask();
        closeTypeTask = false;
    }


    //cout << "��������� � ������������� �������������: " << economic_koef << endl;
    //std::cout << std::fixed << std::setprecision(5);
    add_electric(electric_count, economic_koef); //��� ��������� ��������� �����
    methodMinElem_electric();

    //{
    //    std::cout << std::fixed << std::setprecision(1);
    //    std::cout << "��������� � ������������ �����������: " << economic_koef << std::endl;
    //}
    //showPostavki();//������ ������� ����

    //cout << "result cost after minimal elem method: " << calculating�osts() << endl;
    while (checkDegeneratePlan()) {//������������� �������
        addNullTransportation();
    }
    calculatePotencials_parallel();
    auto start_time = std::chrono::high_resolution_clock::now();
    redistributionSuppliesElectric();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    std::cout << std::fixed << std::setprecision(1);
    cout << "��������� ��������� � ������������ �����������: " << economic_koef << " ���������� " << fixed << static_cast<long long>(calculating�osts()) << endl;
    std::cout << std::fixed << std::setprecision(5);
    //std::cout << "execution time: " << duration.count() << " second." << std::endl;
    //showPostavki();
    cout << endl;
}


//���������� ���������������� � ������
//� ��������� ������� ���� � �����
void Method_potentials::add_electric(int count, double economic_koef) {
    if (count > countSuppliers) {
        cout << "\n �������� ���������� ����������������� \n";
        return;
    }
    for (int i = 0; i < count; i++) {
        costMat[i][i*2].set_electric();
        costMat[i][i*2].set_tarif(costMat[i][i].get_tarif()* economic_koef);
    }
    
}

void Method_potentials::correct_electric_tarifs(double economic_koef) {
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            if (costMat[i][j].get_electric() == true)
                costMat[i][j].set_tarif(std::ceil(costMat[i][j].get_tarif()* economic_koef));
        }
    }

}

void Method_potentials::calculatePotencials_parallel() {
    double identefikator = -100.1786;

    // ���������� suppliersPotincials � �onsumerPotincials �����������
#pragma omp parallel for default(none)
    for (int i = 0; i < suppliersPotincials.size(); i++) {
        suppliersPotincials[i] = identefikator;
    }
#pragma omp parallel for default(none)
    for (int i = 0; i < �onsumerPotincials.size(); i++) {
        �onsumerPotincials[i] = identefikator;
    }

    int max_count_basic = 0;
    int index = 0;

    // ���������� ������� ������ � ���������� ����������� ������� �����
#pragma omp parallel for reduction(max:max_count_basic) shared(index) default(none)
    for (int m = 0; m < suppliersPotincials.size(); m++) {
        int max_count_tmp = 0;
        for (int n = 0; n < �onsumerPotincials.size(); n++) {
            if (costMat[m][n].get_status() == basic)
                max_count_tmp++;
        }
#pragma omp critical
        {
            if (max_count_tmp > max_count_basic) {
                max_count_basic = max_count_tmp;
                index = m;
            }
        }
    }

    int counter = 0;
    suppliersPotincials[index] = 0;

    // ���������� ��������� �������
    while (counter < suppliersPotincials.size() - 1) {
#pragma omp parallel for default(none) shared(identefikator)
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            if (suppliersPotincials[i] != identefikator) {
                for (int j = 0; j < �onsumerPotincials.size(); j++) {
                    if (costMat[i][j].get_status() == basic) {
                        double temp = costMat[i][j].get_tarif() - suppliersPotincials[i];
#pragma omp atomic write
                        �onsumerPotincials[j] = temp;
                    }
                }
            }

            if (suppliersPotincials[i] == identefikator) {
                for (int j = 0; j < �onsumerPotincials.size(); j++) {
                    if (costMat[i][j].get_status() == basic && �onsumerPotincials[j] != identefikator) {
                        double temp = costMat[i][j].get_tarif() - �onsumerPotincials[j];
#pragma omp atomic write
                        suppliersPotincials[i] = temp;
                        break;
                    }
                }
            }
        }
        counter++;
    }
}

void Method_potentials::redistributionSuppliesElectric() {



    //calculatePotencials(costMat, suppliersPotincials, �onsumerPotincials);
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;
    int electric_make_zero = 0;

    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//������������� �������, ��� ����������? 
            addNullTransportation();
        }

        vector<vector<Cell>> tmpCostMat = costMat;

        old_cost = calculating�osts();
        Cell min = tmpCostMat[0][0];
        int minIndexI = 0;
        int minIndexJ = 0;

        //������� ������ � ���� ��������� c-u-v ��� ������ ����� ���������
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            for (int j = 0; j < �onsumerPotincials.size(); j++) {
                if (tmpCostMat[i][j].get_status() == free_)
                    if (tmpCostMat[i][j].get_defferncTarifAndPotincials() < min.get_defferncTarifAndPotincials()) {
                        min = tmpCostMat[i][j];
                        minIndexI = i;
                        minIndexJ = j;
                    }
            }
        }
        //build halfchain
        //���� ��� �� ������ +- ��������, ��� ������ � ����������� ��������� ������ ��������
        tmpCostMat[minIndexI][minIndexJ].set_signInHalfChain(positive);
        tmpCostMat[minIndexI][minIndexJ].set_status(basic);

        vector<int> indexIinChain; // ����� ������ ��� ������ �����
        vector<int> indexJinChain; // ����� �������
        unordered_set<int> forbiddenCells; // ����� ����������� �����
        indexIinChain.push_back(minIndexI);//���������� ������� ������ �����
        indexJinChain.push_back(minIndexJ);
        int currentIndexI = minIndexI;//�� ��� �� �� ������� ����
        int currentIndexJ = minIndexJ;
        int i = 0;
        int j = 0;
        int counter = 0;
        int direction_changing = 1;
        Direction direction = RIGHT;
        //������ ���������� ����� ���������
        while (true) {     
            if (currentIndexI == minIndexI && currentIndexJ == minIndexJ && indexIinChain.size() <= 4) {
                counter++;
            }
            int flag = 0;

            // ���������� ��������� ������ � ����������� �� �����������
            switch (direction) {
            case RIGHT:
                for (int j = currentIndexJ + 1; j < countConsumers && flag == 0; ++j) {
                    if (costMat[currentIndexI][j].get_status() == basic && !containIndexes(currentIndexI, j, indexIinChain, indexJinChain) && !isForbidden(currentIndexI, j, forbiddenCells, countConsumers)) {
                        currentIndexJ = j;
                        flag = 1;
                        direction = DOWN;
                        break;
                    }
                }
                break;
            case DOWN:
                for (int i = currentIndexI + 1; i < countSuppliers && flag == 0; ++i) {
                    if (costMat[i][currentIndexJ].get_status() == basic && !containIndexes(i, currentIndexJ, indexIinChain, indexJinChain) && !isForbidden(i, currentIndexJ, forbiddenCells, countConsumers)) {
                        currentIndexI = i;
                        flag = 1;
                        direction = LEFT;
                        break;
                    }
                }
                break;
            case LEFT:
                for (int j = currentIndexJ - 1; j >= 0 && flag == 0; --j) {
                    if (costMat[currentIndexI][j].get_status() == basic && !containIndexes(currentIndexI, j, indexIinChain, indexJinChain) && !isForbidden(currentIndexI, j, forbiddenCells, countConsumers)) {
                        currentIndexJ = j;
                        flag = 1;
                        direction = UP;
                        break;
                    }
                }
                break;
            case UP:
                for (int i = currentIndexI - 1; i >= 0&& flag == 0; --i) {
                    if (costMat[i][currentIndexJ].get_status() == basic && !containIndexes(i, currentIndexJ, indexIinChain, indexJinChain) && !isForbidden(i, currentIndexJ, forbiddenCells, countConsumers)) {
                        currentIndexI = i;
                        flag = 1;
                        direction = RIGHT;
                        break;
                    }
                }
                break;
            }

            if (flag) {
                direction_changing = 1;
                if (tmpCostMat[indexIinChain.back()][indexJinChain.back()].get_signInHalfChain() == positive)
                    tmpCostMat[currentIndexI][currentIndexJ].set_signInHalfChain(negative);
                else
                    tmpCostMat[currentIndexI][currentIndexJ].set_signInHalfChain(positive);
                indexIinChain.push_back(currentIndexI);
                indexJinChain.push_back(currentIndexJ);
                if (indexIinChain.front() == indexIinChain.back() && indexJinChain.front() != indexJinChain.back() && indexIinChain.size() >= 4 )//&& indexIinChain.size() % 2 == 0
                    break;
                if (indexJinChain.front() == indexJinChain.back() && indexIinChain.front() != indexIinChain.back() && indexIinChain.size() >= 4 )//&& indexIinChain.size() % 2 == 0
                    break;
            }
            else {
                if (direction_changing == 2) {//������ ����������� � ��� ����� �� ����� ������ ������
                    direction_changing = 0;
                    forbiddenCells.insert(currentIndexI * countConsumers + currentIndexJ); // ��������� � �����������
                    if (indexIinChain.size() == 1 && indexJinChain.size() == 1 && counter >= 5) {
                        break;
                    }
                    indexIinChain.pop_back();
                    indexJinChain.pop_back();
                    direction = static_cast<Direction>((direction + 1) % 4);//����� �������� �������� �������������� ����� �� ������������ � ��������
                }
                else {
                    direction = static_cast<Direction>((direction + 2) % 4); // ������ ����������� �� ���������������
                }
                
                if (!indexIinChain.empty() && !indexJinChain.empty()) {
                    currentIndexI = indexIinChain.back();
                    currentIndexJ = indexJinChain.back();
                }
                //direction = static_cast<Direction>((direction + 2) % 4); // ������ ����������� �� ���������������
                direction_changing++;
            }
        }
        //if (countCycles >=1000* suppliersPotincials.size() * �onsumerPotincials.size()) {//|| counter >= 5
        //    cout << "Cycle not found" << "\n";
        //    break;
        //}
        int minI = indexIinChain[1];
        int minJ = indexJinChain[1];

        //���� ������� �������� � ���������� negative �������
        double minCargoVolume = tmpCostMat[indexIinChain[1]][indexJinChain[1]].get_cargoVolueme();
        for (int k = 0; k < indexIinChain.size(); k++) {
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() < minCargoVolume) {//����� �������� ���� ��� �� �������� � ���
                minCargoVolume = tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme();
                minI = indexIinChain[k];
                minJ = indexJinChain[k];
            }

        }

        //---------------------------------------------------
        for (int k = 0; k < indexIinChain.size(); k++) {
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_electric() == true &&
                tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() == tmpCostMat[minI][minJ].get_cargoVolueme()) {

                electric_make_zero++;
                //break;
            }
        }

        if (costMat[minI][minJ].get_electric() == true || electric_make_zero > 0) {
            //cout << "������������������� �����������, ����������� ��������" << endl;
            break;
        }  
        //------------------------------------------------------------------------------------


        //���������������� �������� � �������������� �������
        if (minCargoVolume != 0) {
            for (int k = 0; k < indexIinChain.size(); k++) {
                if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative) {//����� �������� ���� ��� �� �������� � ���
                    costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() - minCargoVolume);
                }
                else
                {
                    costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() + minCargoVolume);
                }
            }
        }
        //else {
        //    //showPostavki();
        //    std::cout << "calculating cost: " << static_cast<long long>(calculating�osts()) << endl;
        //    std::cout << "redistribution volume = 0" << endl;//���� � ���� ��������� ������� ���������, ���� ��� ����������������� ��� ����������, �� �� ��� � ������� ���� ���� ������ �� �������
        //    break;
        //}


        //showPostavki();//----------------------------------------------

         //������ ������� ���������� ������
        updateStatuses(indexIinChain, indexJinChain);

        //std::cout << "calculating cost: " << static_cast<long long>(calculating�osts()) << endl;
        new_cost = calculating�osts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 4) {
            //std::cout << "cost not change because break" << endl; //�������� �� ����������� ��� ��� � �� �������� ������� �����������������
            break;
        }

        /*  for (int i = 0; i < suppliersPotincials.size(); i++) {
              cout << suppliersPotincials[i] << " ";
          }
          cout << endl;

          for (int i = 0; i < �onsumerPotincials.size(); i++)
              cout << �onsumerPotincials[i] << " ";
          cout << endl;*/
        auto start_time = std::chrono::high_resolution_clock::now();
        calculatePotencials_parallel();
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
        clean_time += duration.count();
    }
}
#include"Method_potentials.h"
#include <fstream>
#include<numeric>
#include <chrono>
#include "Parallel.h"
// ����� � ������������� �� ��������� ����� ���� ����� ���������� ��� ����������  � ������ � ������������������ �������� ��������������� ��������� ���� ������


//����� ������������ �� ��� ������������������
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


void Method_potentials::redistributionSupplies_elctric() {//���������������� ��� �������� ������ �������
    //calculatePotencials(costMat, suppliersPotincials, �onsumerPotincials);
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;

    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//������������� ������� �� � ���� ����� �� ����
            addNullTransportation();
            //calculatePotencials();//? ���� ��
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

        vector<int> indexIinChain;//����� ������ ��� ������ �����
        vector<int> indexJinChain;//����� �������
        indexIinChain.push_back(minIndexI);//���������� ������� ������ �����
        indexJinChain.push_back(minIndexJ);
        int currentIndexI = minIndexI;//�� ��� �� �� ������� ����
        int currentIndexJ = minIndexJ;
        int countCycles = 0;
        int i = 0;
        int j = 0;
        int counter = 0;
        while ((((currentIndexI != minIndexI || currentIndexJ != minIndexJ) || indexIinChain.size() <= 4)) && countCycles < suppliersPotincials.size() * �onsumerPotincials.size()) {// && counter < stocks.size()
            if (currentIndexI == minIndexI && currentIndexJ == minIndexJ && indexIinChain.size() <= 4) {//���������� ��������� � ��������� �����.
                counter++;
            }
            countCycles++;
            int flag = 0;
            //�� ��������� � ��������� ������� ���� ������ ������
            for (i; i < suppliersPotincials.size(); i++) {
                if (tmpCostMat[i][currentIndexJ].get_status() == basic && i != currentIndexI && (!containIndexes(i, currentIndexJ, indexIinChain.size(), indexIinChain, indexJinChain) || (i == minIndexI && currentIndexJ == minIndexJ))) {

                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == positive)
                        tmpCostMat[i][currentIndexJ].set_signInHalfChain(negative);//����� ������ ��� ����
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);

                    indexIinChain.push_back(i);//���������� ������� ������ (����� ������)
                    indexJinChain.push_back(currentIndexJ);
                    currentIndexI = i;
                    flag = 1;
                    j = 0;
                    break;
                }
            }
            if (flag == 0)       //���� � ������� �� ����� ������ ��� ����������� �����, 
            {     // �� � ������� ������� �������, � ������� ������
                j = indexJinChain.back() + 1;//�������� ������� �� ������� ���� ������
                indexJinChain.erase(indexJinChain.end() - 1);
                indexIinChain.erase(indexIinChain.end() - 1);//?
                currentIndexJ = indexJinChain.back();

            }
            flag = 0;
            //�� ����������� � ��������� ������ ���� ������ �������
            for (j; j < �onsumerPotincials.size(); j++) {

                if (tmpCostMat[currentIndexI][j].get_status() == basic && j != currentIndexJ && (!containIndexes(currentIndexI, j, indexIinChain.size(), indexIinChain, indexJinChain) || (currentIndexI == minIndexI && j == minIndexJ))) {
                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == negative)
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);//����� ������ ��� ����
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(negative);

                    indexIinChain.push_back(currentIndexI);//���������� ������� ������ 
                    indexJinChain.push_back(j);//���������� ����� �������
                    currentIndexJ = j;
                    flag = 1;
                    i = 0;
                    break;
                }
            }
            if (flag == 0)       //���� � ������ �� ����� ������ ��� ����������� �����, 
            {     // �� � ������� ������ ������, � ������� ������
                i = indexIinChain.back() + 1;//�������� ������� �� ������� ���� ������
                indexIinChain.erase(indexIinChain.end() - 1);
                indexJinChain.erase(indexJinChain.end() - 1);//?
                currentIndexI = indexIinChain.back();

            }
        }
        if (countCycles >= suppliersPotincials.size() * �onsumerPotincials.size()) {//|| counter >= 5
            cout << "Cycle not found" << "\n";
            break;
        }
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
        //-------------------------------�� ���� �������� ����������������--------------------------------------------
        for (int k = 0; k < indexIinChain.size(); k++) { //���� ��������� ��������� � ���� �� ��� � �����������������
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_electric() == true &&
                tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() == tmpCostMat[minI][minJ].get_cargoVolueme()) {
                std::cout << "������ � ����������������� ���������" << endl;
                break;
            }
        }
        if (costMat[minI][minJ].get_electric() == true) {
            std::cout << "calculating cost: " << calculating�osts() << endl;
            std::cout << "��������� ������ � ����������������� �����������" << endl;
            break;
        }
        //------------------------------------------------------------------------------------
        //���������������� �������� � �������������� �������
        if (minCargoVolume != 0) {
            for (int k = 0; k < indexIinChain.size() - 1; k++) {// -1 �� ������ ������ ����� ������ �����������
                if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative) {//����� �������� ���� ��� �� �������� � ���
                    costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() - minCargoVolume);
                }
                else
                {
                    costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() + minCargoVolume);
                }
            }
        }
        else {
            //showPostavki();
            std::cout << "calculating cost: " << static_cast<long long>(calculating�osts()) << endl;
            std::cout << "redistribution volume = 0" << endl;//���� � ���� ��������� ������� ���������, ���� ��� ����������������� ��� ����������, �� �� ��� � ������� ���� ���� ������ �� �������
            break;
        }

        //showPostavki();//----------------------------------------------
        //������ ������� ���������� ������
        updateStatuses(indexIinChain, indexJinChain);

        std::cout << "calculating cost: " << static_cast<long long>(calculating�osts()) << endl;
        new_cost = calculating�osts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 3) { //3 ��� ������
            std::cout << "cost not change because break" << endl; //�������� �� ����������� ��� ��� � �� �������� ������� �����������������
            break;
        }
        calculatePotencials();
    }
}

//������������ ����� �����������������
void Method_potentials::redistributionSupplies_parallel() {
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;


    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//������������� ������� �� � ���� ����� �� ����
            addNullTransportation();
        //calculatePotencials_parallel();//
        }
        vector<vector<Cell>> tmpCostMat = costMat;

        old_cost = calculating�osts();
        Cell min = tmpCostMat[0][0];
        int minIndexI = 0;
        int minIndexJ = 0;

        //������� ������ � ���� ��������� c-u-v ��� ������ ����� ���������
//#pragma omp parallel for collapse(2) shared(tmpCostMat, min, minIndexI, minIndexJ)
//#pragma omp parallel for collapse(2) schedule(static)
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            for (int j = 0; j < �onsumerPotincials.size(); j++) {
                if (tmpCostMat[i][j].get_status() == free_)
//#pragma omp critical
              // {
                    if (tmpCostMat[i][j].get_defferncTarifAndPotincials() < min.get_defferncTarifAndPotincials()) {
                        min = tmpCostMat[i][j];
                        minIndexI = i;
                        minIndexJ = j;
                    }
               // }
            }
        }
        //build halfchain
        //���� ��� �� ������ +- ��������, ��� ������ � ����������� ��������� ������ ��������
        tmpCostMat[minIndexI][minIndexJ].set_signInHalfChain(positive);
        tmpCostMat[minIndexI][minIndexJ].set_status(basic);

        vector<int> indexIinChain;//����� ������ ��� ������ �����
        vector<int> indexJinChain;//����� �������
        indexIinChain.push_back(minIndexI);//���������� ������� ������ �����
        indexJinChain.push_back(minIndexJ);
        int currentIndexI = minIndexI;//�� ��� �� �� ������� ����
        int currentIndexJ = minIndexJ;
        int countCycles = 0;
        int i = 0;
        int j = 0;
        int counter = 0;

        while ((((currentIndexI != minIndexI || currentIndexJ != minIndexJ) || indexIinChain.size() <= 4)) && 
            countCycles < suppliersPotincials.size() * �onsumerPotincials.size()) {// && counter < stocks.size()
            if (currentIndexI == minIndexI && currentIndexJ == minIndexJ && indexIinChain.size() <= 4) {//���������� ��������� � ��������� �����.
                counter++;
            }
            countCycles++;
            int flag = 0;
            //�� ��������� � ��������� ������� ���� ������ ������
            for (i; i < suppliersPotincials.size(); i++) {
                if (tmpCostMat[i][currentIndexJ].get_status() == basic && i != currentIndexI && 
                    (!containIndexes(i, currentIndexJ, indexIinChain.size(), indexIinChain, indexJinChain) ||
                        (i == minIndexI && currentIndexJ == minIndexJ))) {

                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == positive)
                        tmpCostMat[i][currentIndexJ].set_signInHalfChain(negative);//����� ������ ��� ����
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);

                        indexIinChain.push_back(i);//���������� ������� ������ (����� ������)
                        indexJinChain.push_back(currentIndexJ);
                    
                    currentIndexI = i;
                    flag = 1;
                    j = 0;
                    break;
                }
            }
            if (flag == 0)       //���� � ������� �� ����� ������ ��� ����������� �����, 
            {     // �� � ������� ������� �������, � ������� ������
                j = indexJinChain.back() + 1;//�������� ������� �� ������� ���� ������

                    indexJinChain.erase(indexJinChain.end() - 1);
                    indexIinChain.erase(indexIinChain.end() - 1);//?
                
                currentIndexJ = indexJinChain.back();

            }
            flag = 0;
            //�� ����������� � ��������� ������ ���� ������ �������
            for (j; j < �onsumerPotincials.size(); j++) {

                if (tmpCostMat[currentIndexI][j].get_status() == basic && j != currentIndexJ && (!containIndexes(currentIndexI, j, indexIinChain.size(), indexIinChain, indexJinChain) || (currentIndexI == minIndexI && j == minIndexJ))) {
                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == negative)
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);//����� ������ ��� ����
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(negative);
                        indexIinChain.push_back(currentIndexI);//���������� ������� ������ 
                        indexJinChain.push_back(j);//���������� ����� �������
                    
                    currentIndexJ = j;
                    flag = 1;
                    i = 0;
                    break;
                }
            }
            if (flag == 0)       //���� � ������ �� ����� ������ ��� ����������� �����, 
            {     // �� � ������� ������ ������, � ������� ������
                i = indexIinChain.back() + 1;//�������� ������� �� ������� ���� ������
                    indexIinChain.erase(indexIinChain.end() - 1);
                    indexJinChain.erase(indexJinChain.end() - 1);//?
                currentIndexI = indexIinChain.back();

            }
        }
        if (countCycles >= suppliersPotincials.size() * �onsumerPotincials.size()) {//|| counter >= 5
            cout << "Cycle not found" << "\n";
            break;
        }
        int minI = indexIinChain[1];
        int minJ = indexJinChain[1];

        //���� ������� �������� � ���������� negative �������
        double minCargoVolume = tmpCostMat[indexIinChain[1]][indexJinChain[1]].get_cargoVolueme();
//#pragma omp parallel for
        for (int k = 0; k < indexIinChain.size(); k++) {
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() < minCargoVolume) {//����� �������� ���� ��� �� �������� � ���
//#pragma omp critical
  //              {
                    minCargoVolume = tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme();
                    minI = indexIinChain[k];
                    minJ = indexJinChain[k];
              //  }
            }

        }

        //-------------------------------�� ���� �������� ����������������--------------------------------------------
        for (int k = 0; k < indexIinChain.size(); k++) { //���� ��������� ��������� � ���� �� ��� � �����������������
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_electric() == true &&
                tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() == tmpCostMat[minI][minJ].get_cargoVolueme()) {
                std::cout << "������ � ����������������� ���������" << endl;
                break;
            }
        }

        if (costMat[minI][minJ].get_electric() == true) {
            std::cout << "calculating cost: " << calculating�osts() << endl;
            std::cout << "��������� ������ � ����������������� �����������" << endl;
            break;
        }

        //------------------------------------------------------------------------------------
        //���������������� �������� � �������������� �������
        if (minCargoVolume != 0) {
            for (int k = 0; k < indexIinChain.size() - 1; k++) {// -1 �� ������ ������ ����� ������ �����������
                if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative) {//����� �������� ���� ��� �� �������� � ���
                    costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() - minCargoVolume);
                }
                else
                {
                    costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() + minCargoVolume);
                }
            }
        }
        else {
            //showPostavki();
            std::cout << "calculating cost: " << calculating�osts() << endl;
            std::cout << "redistribution volume = 0" << endl;//���� � ���� ��������� ������� ���������, ���� ��� ����������������� ��� ����������, �� �� ��� � ������� ���� ���� ������ �� �������
            break;
        }
        //showPostavki();//----------------------------------------------
        //������ ������� ���������� ������
        updateStatuses(indexIinChain, indexJinChain);

        std::cout << "calculating cost: " << static_cast<long long>(calculating�osts()) << endl;
        new_cost = calculating�osts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 3) { //3 ��� ������
            std::cout << "cost not change because break" << endl; //�������� �� ����������� ��� ��� � �� �������� ������� �����������������
            break;
        }
        calculatePotencials_parallel();
    }

}


//����� ��� ������������
//������ ����������� ���������
void Method_potentials::solve_parallel(int electric_count) {
    if (!checkCloseTypeTask()) {//���� �� ��������
        addDataForClosingTask();
        closeTypeTask = false;
    }
    add_electric(electric_count);
    methodMinElem_electric();
    //showPostavki();//������ ������� ����

    cout << "result cost after minimal elem method: " << calculating�osts() << endl;
    while (checkDegeneratePlan()) {//������������� �������
        addNullTransportation();
    }
    //calculatePotencials();
    calculatePotencials_parallel();
    auto start_time = std::chrono::high_resolution_clock::now();
    redistributionSupplies_parallel();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    cout << "result minimal cost: " << fixed << static_cast<long long>(calculating�osts()) << endl;
    std::cout << "execution time: " << duration.count() << " second." << std::endl;
    showPostavki();
    cout << endl;
}

void Method_potentials::solve_electric_sequence(int electric_count) {
    if (!checkCloseTypeTask()) {//���� �� ��������
        addDataForClosingTask();
        closeTypeTask = false;
    }
    add_electric(electric_count);
    methodMinElem_electric();
    //showPostavki();//������ ������� ����

    cout << "result cost after minimal elem method: " << calculating�osts() << endl;
    while (checkDegeneratePlan()) {//������������� �������
        addNullTransportation();
    }
    calculatePotencials();
    auto start_time = std::chrono::high_resolution_clock::now();
    redistributionSupplies_elctric();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    cout << "result minimal cost: " << fixed << static_cast<long long>(calculating�osts()) << endl;
    std::cout << "execution time: " << duration.count() << " second." << std::endl;
    showPostavki();
    cout << endl;
}

//���������� ���������������� � ������
//� ��������� ������� ���� � �����
void Method_potentials::add_electric(int count) {
    if (count > countSuppliers)
        cout << "\n �������� ���������� ����������������� \n";
    for (int i = 0; i < count; i++) {
        costMat[i][i*2].set_electric();
        costMat[i][i*2].set_tarif(costMat[i][i].get_tarif()/2);//����� �� 2 ����� �� ������� ��������
    }
    
}

void Method_potentials::calculatePotencials_parallel() {
    double identefikator = -100.1786;

    // ���������� suppliersPotincials � �onsumerPotincials �����������
#pragma omp parallel for
    for (int i = 0; i < suppliersPotincials.size(); i++) {
        suppliersPotincials[i] = identefikator;
    }
#pragma omp parallel for
    for (int i = 0; i < �onsumerPotincials.size(); i++) {
        �onsumerPotincials[i] = identefikator;
    }

    int max_count_basic = 0;
    int index = 0;

    // ���������� ������� ������ � ���������� ����������� ������� �����
#pragma omp parallel for reduction(max:max_count_basic) shared(index)
    for (int m = 0; m < suppliersPotincials.size(); m++) {
        int max_count_tmp = 0;
        for (int n = 0; n < �onsumerPotincials.size(); n++) {
            if (costMat[m][n].get_status() == basic)
                max_count_tmp++;
        }
        if (max_count_tmp > max_count_basic) {
            max_count_basic = max_count_tmp;
            index = m;
        }
    }

    int counter = 0;
    suppliersPotincials[index] = 0;

    // ���������� ��������� �������
    while (counter < suppliersPotincials.size() - 1) {
#pragma omp parallel for
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            if (suppliersPotincials[i] != identefikator) {
                for (int j = 0; j < �onsumerPotincials.size(); j++) {
                    if (costMat[i][j].get_status() == basic)
                        �onsumerPotincials[j] = costMat[i][j].get_tarif() - suppliersPotincials[i];
                }
            }
            if (suppliersPotincials[i] == identefikator) {
                for (int j = 0; j < �onsumerPotincials.size(); j++) {
                    if (costMat[i][j].get_status() == basic && �onsumerPotincials[j] != identefikator)
                        suppliersPotincials[i] = costMat[i][j].get_tarif() - �onsumerPotincials[j];
                }
            }
        }
        counter++;
    }
}
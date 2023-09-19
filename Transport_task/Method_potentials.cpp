#include"Method_potentials.h"
#include <fstream>

using namespace libxl;

void Method_potentials::solve() {
    
    showTable();
    if (!checkCloseTypeTask()) {//���� �� ��������
        addDataForClosingTask();
        closeTypeTask = false;
    }
    methodMinElem();
    showPostavki();//������ ������� ����

    cout << "result cost after minimal elem method: " << calculating�osts() << endl;
    while (checkDegeneratePlan()) {//������������� �������
        addNullTransportation();
    }
    calculatePotencials();

    redistributionSupplies();//�� ��� ��� ���� �� ����� �����������, ���������� ����������� ������ ������

    cout << "result minimal cost: " << calculating�osts() << endl;
}
int Method_potentials::get_value(int i, int j) {
    return costMat[i][j].get_cargoVolueme();

}


void Method_potentials::Method_potentials_init() {
    Book* book = xlCreateXMLBook(); // �������� ������� �����
    if (book) {
        if (book->load(L"data.xlsx")) { // �������� ����� Excel
            Sheet* sheet = book->getSheet(0); // ��������� ������� �����
            if (sheet) {

                int numRows = sheet->lastRow();
                for (int row = sheet->firstRow()+2; row < numRows; ++row) {
                    stocks.push_back(sheet->readNum(row, 0));
                }
                countSuppliers = stocks.size();
                cout << "����������� �������  " << countSuppliers << endl;
                // ������ � ��� ���� ������ � ������� �� Excel
                for (const auto& data : stocks) {
                    cout << " Value: " << data << endl;
                }
            }
        }
        book->save(L"example.xlsx");
        book->release(); // ������������ ������, ������� �������� �����
    }


}








Method_potentials::Method_potentials(int flag, int listnum) {

    if (flag == 0) {
        Book* book = xlCreateXMLBook(); // �������� ������� �����
        if (book) {
            if (book->load(L"data.xlsx")) { // �������� ����� Excel
                Sheet* sheet = book->getSheet(listnum); // ��������� ������� �����
                if (sheet) {

                    int numRows = sheet->lastRow();
                    for (int row = sheet->firstRow() + 2; row < numRows; ++row) {
                        stocks.push_back(sheet->readNum(row, 0));
                    }
                    countSuppliers = stocks.size();

                    //���������� �������
                    int numCols = 0;
                    for (int col = sheet->firstCol() + 1; col < sheet->lastCol(); ++col) {
                        libxl::CellType cellType = sheet->cellType(1, col); // get cell type
                        if (cellType == libxl::CELLTYPE_NUMBER) {
                            numCols++;
                        }
                        else {
                            break;
                        }
                    }




                    for (int col = sheet->firstCol() + 1; col <= sheet->firstCol() + numCols; ++col) {
                        if (!std::isnan(sheet->readNum(1, col))) {
                            needs.push_back(sheet->readNum(1, col));
                        }
                        else { break; }
                    }

                    countConsumers = needs.size();

                    costMat.resize(countSuppliers, vector<Cell>(countConsumers));
                    suppliersPotincials.resize(countSuppliers);
                    �onsumerPotincials.resize(countConsumers);

                    
                    // ������� � ������������� �������
                    
                    for (int i = 0; i < countSuppliers; i++) {
                        for (int j = 0; j < countConsumers; j++) {
                            costMat[i][j].set_tarif(static_cast<int>(getRandomNumber(2, 15)));
                        }
                    }

                    cout << "����������� �������  " << countSuppliers << endl;
                    // ������ � ��� ���� ������ � ������� �� Excel
                    for (const auto& data : stocks) {
                        cout << " Value: " << data << endl;
                    }
                    cout << "����������� ������������  " << countConsumers << endl;
                    // ������ � ��� ���� ������ � ������� �� Excel
                    for (const auto& data : needs) {
                        cout << " Value: " << data << endl;
                    }
                }
            }
            book->save(L"example.xlsx");
            book->release(); // ������������ ������, ������� �������� �����
        }



    }
    else {
        Book* book = xlCreateXMLBook(); // �������� ������� �����
        if (book) {
            if (book->load(L"data.xlsx")) { // �������� ����� Excel
                Sheet* sheet = book->getSheet(listnum); // ��������� ������� �����
                if (sheet) {

                    int numRows = sheet->lastRow();
                    for (int row = sheet->firstRow() + 2; row < numRows; ++row) {
                        stocks.push_back(sheet->readNum(row, 0));
                    }
                    countSuppliers = stocks.size();

                    //���������� �������
                    int numCols = 0;
                    for (int col = sheet->firstCol() + 1; col < sheet->lastCol(); ++col) {
                        libxl::CellType cellType = sheet->cellType(1, col); // get cell type
                        if (cellType == libxl::CELLTYPE_NUMBER) {
                            numCols++;
                        }
                        else {
                            break;
                        }
                    }
                   



                    for (int col = sheet->firstCol() + 1; col <= sheet->firstCol() + numCols; ++col) {
                        if (!std::isnan(sheet->readNum(1, col))) {
                            needs.push_back(sheet->readNum(1, col));
                        }
                        else { break; }
                    } 
                   
                    countConsumers = needs.size();
                    
                    costMat.resize(countSuppliers, vector<Cell>(countConsumers));
                    suppliersPotincials.resize(countSuppliers);
                    �onsumerPotincials.resize(countConsumers);
                    int i = 0;
                    
                    for (int row = sheet->firstRow() + 2; row < numRows; ++row) {//??????
                        int j = 0;
                        for (int col = sheet->firstCol() + 1; col <= sheet->firstCol() + numCols; ++col) {
                            
                            costMat[i][j].set_tarif(sheet->readNum(row, col));
                            j++;
                        }
                        i++;
                    }



                    cout << "����������� �������  " << countSuppliers << endl;
                    // ������ � ��� ���� ������ � ������� �� Excel
                    for (const auto& data : stocks) {
                        cout << " Value: " << data << endl;
                    }
                    cout << "����������� ������������  " << countConsumers << endl;
                    // ������ � ��� ���� ������ � ������� �� Excel
                    for (const auto& data : needs) {
                        cout << " Value: " << data << endl;
                    }
                }
            }
            book->save(L"example.xlsx");
            book->release(); // ������������ ������, ������� �������� �����
        }




    }
    //costMat.resize(countSuppliers, vector<Cell>(countConsumers));
    //suppliersPotincials.resize(countSuppliers);
    //�onsumerPotincials.resize(countConsumers);

  /*  while (!out.eof()) {
        out >> k;
        stocks.push_back(k);
        cout << " zapas from file " << k << endl;
    }*/
  /*  while (!need.eof()) {
        need >> k;
        needs.push_back(k);
        cout << " potrebnosti from file " << k << endl;
    }*/

    //costMat[0][0].set_tarif(7);
    //costMat[0][1].set_tarif(8);
    //costMat[0][2].set_tarif(1);
    //costMat[0][3].set_tarif(2);
    //costMat[1][0].set_tarif(4);
    //costMat[1][1].set_tarif(5); 
    //costMat[1][2].set_tarif(9);
    //costMat[1][3].set_tarif(8);
    //costMat[2][0].set_tarif(9);
    //costMat[2][1].set_tarif(2); 
    //costMat[2][2].set_tarif(3);
    //costMat[2][3].set_tarif(6); 

}

double Method_potentials::getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<double>(rand() * fraction * (max - min + 1) + min);
}

void Method_potentials::showTable() {
    cout << "\n";
    cout << "tarifs" << endl;
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            std::cout << costMat[i][j].get_tarif() << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void Method_potentials::methodMinElem() {
    int min = costMat[0][0].get_tarif();
    int minIndexI = 0;
    int minIndexJ = 0;
    int a = 0;
    vector<int> busyStrIndex;
    vector<int> busyColIndex;
    while (true) {
        a++;
        for (int i = 0; i < stocks.size(); i++) {
            if (!count(busyStrIndex.begin(), busyStrIndex.end(), i))
                for (int j = 0; j < needs.size(); j++) {
                    if (costMat[i][j].get_tarif() <= min && costMat[i][j].get_status() == free_ && !count(busyColIndex.begin(), busyColIndex.end(), j)) {
                        minIndexI = i;//������� ���  ����� ������� �� �����������
                        minIndexJ = j;
                        min = costMat[i][j].get_tarif();
                    }
                }
        }
        if (stocks[minIndexI] > needs[minIndexJ] && needs[minIndexJ] != 0) {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs[minIndexJ]);
            stocks[minIndexI] -= needs[minIndexJ];
            needs[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);
        }
        else if (stocks[minIndexI] == needs[minIndexJ] && needs[minIndexJ] != 0) {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs[minIndexJ]);
            stocks[minIndexI] -= needs[minIndexJ];
            needs[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);
            busyStrIndex.push_back(minIndexI);
        }
        else {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(stocks[minIndexI]);
            needs[minIndexJ] -= stocks[minIndexI];
            stocks[minIndexI] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyStrIndex.push_back(minIndexI);
        }
        //showPostavki(costMat, stocks.size(), needs.size());
        cout << endl;
        min = findMax();
        if (busyStrIndex.size() == stocks.size() && busyColIndex.size() == needs.size()) {
            if (closeTypeTask == false) {//� ��������� ������ ������� ������(���� ������ ���� ������ ������)
                for (int k = 0; k < stocks.size(); k++) {
                    costMat[k][needs.size() - 1].set_tarif(0);
                }
            }
            return;
        }
    }
}


void Method_potentials::showPostavki() {
    cout << endl;
    for (int i = 0; i < countSuppliers; i++)
    {
        for (int j = 0; j < countConsumers; j++)
            std::cout << costMat[i][j].get_cargoVolueme() << "\t";
        cout << endl;
    }
}

double Method_potentials::findMax() {
    double max = costMat[0][0].get_tarif();
    for (int i = 0; i < countSuppliers; i++)
    {
        for (int j = 0; j < countConsumers; j++) {
            if (costMat[i][j].get_tarif() >= max)
                max = costMat[i][j].get_tarif();
        }
    }
    return max;
}

bool Method_potentials::checkDegeneratePlan() {
    int countBasisCell = 0;
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            if (costMat[i][j].get_status() == basic)
                countBasisCell++;
        }
    }
    if (countBasisCell < countConsumers + countSuppliers - 1) {
        return true;
    }
    return false;
}

void Method_potentials::calculatePotencials() {
    double identefikator = -100.1;

    for (int i = 0; i < suppliersPotincials.size(); i++) {
        suppliersPotincials[i] = identefikator;
    }
    for (int i = 0; i < �onsumerPotincials.size(); i++) {
        �onsumerPotincials[i] = identefikator;
    }
    int counter = 0;
    suppliersPotincials[0] = 0;
    while (counter < suppliersPotincials.size() - 1) {
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            if (suppliersPotincials[i] != identefikator) {
                for (int j = 0; j < �onsumerPotincials.size(); j++)
                {
                    if (costMat[i][j].get_status() == basic)
                        �onsumerPotincials[j] = costMat[i][j].get_tarif() - suppliersPotincials[i];
                }
            }
            if (suppliersPotincials[i] == identefikator) {
                for (int j = 0; j < �onsumerPotincials.size(); j++)
                {
                    if (costMat[i][j].get_status() == basic && �onsumerPotincials[j] != identefikator)
                        suppliersPotincials[i] = costMat[i][j].get_tarif() - �onsumerPotincials[j];
                }
            }
        }
        counter++;
    }
}


bool Method_potentials::checkOptimal() {

    bool optimal = true;
    //������� ������ ��������� ������
    for (int i = 0; i < suppliersPotincials.size(); i++) {
        for (int j = 0; j < �onsumerPotincials.size(); j++) {
            if (costMat[i][j].get_status() == free_)
                costMat[i][j].set_defferncTarifAndPotincials(costMat[i][j].get_tarif() - suppliersPotincials[i] - �onsumerPotincials[j]);
        }
    }

    for (int i = 0; i < suppliersPotincials.size(); i++) {
        for (int j = 0; j < �onsumerPotincials.size(); j++) {
            if (costMat[i][j].get_status() == free_) {
                if (costMat[i][j].get_defferncTarifAndPotincials() >= 0)
                    continue;
                else
                    optimal = false;
            }

        }
    }
    return optimal;

}

void Method_potentials::updateStatuses(vector<int>& indexIinChain, vector<int>& indexJinChain) {
    for (int i = 0; i < indexIinChain.size() - 1; i++) { // -1 �� � ������� ��������� ������ �������� ��� � � ����� // � ������ ����� �������� ���� ������ ������ �� ����������� ���������?(������ �� ��������� ������� � �����)
        if (costMat[indexIinChain[i]][indexJinChain[i]].get_cargoVolueme() > 0) {
            costMat[indexIinChain[i]][indexJinChain[i]].set_status(basic);
            costMat[indexIinChain[i]][indexJinChain[i]].set_signInHalfChain(neutral);
        }
    }  
    for (int i = 0; i < indexIinChain.size() - 1; i++) {
        if (costMat[indexIinChain[i]][indexJinChain[i]].get_cargoVolueme() == 0) {
            costMat[indexIinChain[i]][indexJinChain[i]].set_signInHalfChain(neutral);
        }
    }

    for (int i = 0; i < indexIinChain.size() - 1; i++) {
        if (costMat[indexIinChain[i]][indexJinChain[i]].get_cargoVolueme() == 0) {
          costMat[indexIinChain[i]][indexJinChain[i]].set_status(free_);//� ���� ��������� ������������ ������ ������ ���� ������????
          break;
        }
    }
}






void Method_potentials::redistributionSupplies() {
    //calculatePotencials(costMat, suppliersPotincials, �onsumerPotincials);
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;
    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//������������� ������� �� � ���� ����� �� ����
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
        while ((((currentIndexI != minIndexI || currentIndexJ != minIndexJ) || indexIinChain.size() <= 4) && counter < 5) && countCycles < suppliersPotincials.size() * �onsumerPotincials.size()) {
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
                    if(tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == negative)
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
        if (countCycles >= suppliersPotincials.size() * �onsumerPotincials.size() || counter >= 5) {
            cout << "Cycle not found" << "\n";
            break;
        }


        //���� ������� �������� � ���������� negative �������
        double minCargoVolume = tmpCostMat[indexIinChain[1]][indexJinChain[1]].get_cargoVolueme();
        for (int k = 0; k < indexIinChain.size(); k++) {
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() < minCargoVolume) {//����� �������� ���� ��� �� �������� � ���
                minCargoVolume = tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme();
            }

        }

        //���������������� �������� � �������������� �������
        for (int k = 0; k < indexIinChain.size() - 1; k++) {// -1 �� ������ ������ ����� ������ �����������
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative) {//����� �������� ���� ��� �� �������� � ���
                costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() - minCargoVolume);
            }
            else
            {
                costMat[indexIinChain[k]][indexJinChain[k]].set_cargoVolueme(costMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() + minCargoVolume);
            }
        }


        showPostavki();

        //������ ������� ���������� ������
        updateStatuses(indexIinChain, indexJinChain);
 
        std::cout << "calculating cost: " << calculating�osts() << endl;
        new_cost = calculating�osts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 5) { //5 ��� ������
            std::cout << "cost not change because break" << endl;
            break;
        }
        calculatePotencials();
       /* for (int i = 0; i < suppliersPotincials.size(); i++) {
            cout << suppliersPotincials[i] << " ";
        }
        cout << endl;

        for (int i = 0; i < �onsumerPotincials.size(); i++)
            cout << �onsumerPotincials[i] << " ";
        cout << endl;*/
    }
}

double Method_potentials::calculating�osts() {
    double sumCosts = 0;
    for (int i = 0; i < countSuppliers; i++)
    {
        for (int j = 0; j < countConsumers; j++) {
            if (costMat[i][j].get_cargoVolueme() > 0)
                sumCosts += costMat[i][j].get_cargoVolueme() * costMat[i][j].get_tarif();

        }
    }
    return sumCosts;

}

bool Method_potentials::ignoreCell(int indI, int indJ, vector<int> ignoreIndexI, vector<int> ignoreIndexJ) {
    for (int i = 0; i < ignoreIndexI.size(); i++) {
        if (ignoreIndexI[i] == indI && ignoreIndexJ[i] == indJ)
            return true;//������ ������ ��������������
    }
    return false;
}

void Method_potentials::addNullTransportation() {
    vector<int> ignoreIndexI;//������ ������� ������� ���������� ���� �� �� �������������
    vector<int> ignoreIndexJ;


    while (checkDegeneratePlan()) {
        int cellIndexI = 0;//������� ������ ������� ����� �������� � ����� � �������� ����������.
        int cellIndexJ = 0;
        for (int k = 0; k < countSuppliers; k++) {//���� ������ ���������� ������ � 0 ���������� 
            bool signal = 0;
            for (int m = 0; m < countConsumers; m++) {
                if (costMat[k][m].get_cargoVolueme() == 0 && costMat[k][m].get_status() == free_ && !ignoreCell(k, m, ignoreIndexI, ignoreIndexJ)) {
                    cellIndexI = k;
                    cellIndexJ = m;//������� �� ��� ���������
                    signal = 1;
                    break;
                }
            }
            if (signal == 1)
                break;

        }


        vector<int> indexIinChain;//����� ������ ��� ������ �����
        vector<int> indexJinChain;//����� �������
        indexIinChain.push_back(cellIndexI);//���������� ������� ������ ��� ������ �����
        indexJinChain.push_back(cellIndexJ);
        int currentIndexI = cellIndexI;//
        int currentIndexJ = cellIndexJ;
        int countCycles = 0;
        int i = 0;
        int j = 0;
        int counter = 0; //|| indexIinChain.size() <= 4) && counter < 5 )
        //� ������������ ��� ������ ����� �������
        while (((currentIndexI != cellIndexI || currentIndexJ != cellIndexJ) || counter < 5) && countCycles < countSuppliers * countConsumers) { //����� �� ������� ���� ��� ���� �� ������. ������� || ���� ������ &&

            countCycles++;
            int flag = 0;
            if (currentIndexI == cellIndexI && currentIndexJ == cellIndexJ && indexIinChain.size() <= 4) {//���������� ��������� � ��������� �����.
                counter++;
            }
            //�� ��������� � ��������� ������� ���� ������ ������
            for (i; i < countSuppliers; i++) {
                if (costMat[i][currentIndexJ].get_status() == basic && i != currentIndexI && (!containIndexes(i, currentIndexJ, indexIinChain.size(), indexIinChain, indexJinChain) || (i == cellIndexI && currentIndexJ == cellIndexJ))) {//

                    if (costMat[currentIndexI][currentIndexJ].get_signInHalfChain() == positive)
                        costMat[i][currentIndexJ].set_signInHalfChain(negative);//����� ������ ��� ����
                    else
                        costMat[currentIndexI][j].set_signInHalfChain(positive);
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
                if (indexJinChain.size() == 1 || indexIinChain.size() == 1) {
                    countCycles = countSuppliers * countConsumers;
                    break;
                }
                j = indexJinChain.back() + 1;//�������� ������� �� ������� ���� ������
                indexJinChain.erase(indexJinChain.end() - 1);
                indexIinChain.erase(indexIinChain.end() - 1);//?
                currentIndexJ = indexJinChain.back();
            }
            flag = 0;
            //�� ����������� � ��������� ������ ���� ������ �������
            for (j; j < countConsumers; j++) {
                if (costMat[currentIndexI][j].get_status() == basic && j != currentIndexJ && (!containIndexes(currentIndexI, j, indexIinChain.size(), indexIinChain, indexJinChain) || (currentIndexI == cellIndexI && j == cellIndexJ))) {//
                    if (costMat[currentIndexI][currentIndexJ].get_signInHalfChain() == negative)
                        costMat[currentIndexI][j].set_signInHalfChain(positive);//����� ������ ��� ����
                    else
                        costMat[currentIndexI][j].set_signInHalfChain(negative);
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
                if (indexIinChain.size() == 1 || indexJinChain.size() == 1) {//indexIinChain.empty()
                    countCycles = countSuppliers * countConsumers;
                    break;
                }
                i = indexIinChain.back() + 1;//�������� ������� �� ������� ���� ������
                indexIinChain.erase(indexIinChain.end() - 1);
                indexJinChain.erase(indexJinChain.end() - 1);//?
                currentIndexI = indexIinChain.back();

            }
        }
        //countCycles == countSuppliers * countConsumers ||
        if (countCycles > countSuppliers * countConsumers || ((currentIndexI == cellIndexI && currentIndexJ == cellIndexJ) && indexIinChain.size() <= 4)) { //���� �� ������/ ������ � �������� ������ ��� ������ ������� ������ 5
            costMat[cellIndexI][cellIndexJ].set_status(basic);//��������� ������� ���������                                                      ������ ��� ���� �� ��� ���� >5 �� ��� ������ ��� ������ ���� ���������
        }
        else {
            ignoreIndexI.push_back(cellIndexI);
            ignoreIndexJ.push_back(cellIndexJ);
        }
    }
}

bool Method_potentials::checkCloseTypeTask() {
    bool close = true;
    int sumStocks = 0;
    int sumNeeds = 0;
    for (int i = 0; i < stocks.size(); i++) {
        sumStocks += stocks[i];
    }
    for (int i = 0; i < needs.size(); i++) {
        sumNeeds += needs[i];
    }
    if (sumStocks != sumNeeds) {
        close = false;
    }
    return close;
}

void Method_potentials::addDataForClosingTask() {
    int sumStocks = 0;
    int sumNeeds = 0;
    for (int i = 0; i < stocks.size(); i++) {
        sumStocks += stocks[i];
    }
    for (int i = 0; i < needs.size(); i++) {
        sumNeeds += needs[i];
    }
    Cell cell;
    cell.set_tarif(findMax() + 1);//��������� ������� �����, ���� � ������ ������������ �������� ��������� ������ ���������� �����������(����� ��� ������� 0)
    if (sumStocks > sumNeeds) {//������� ���������� �����������
        countConsumers++;
        for (int i = 0; i < stocks.size(); i++) {
            costMat[i].push_back(cell);//������ ������� ��������� �������������
        }
        needs.push_back(sumStocks - sumNeeds);
    }
    //if (sumStocks < sumNeeds) {//������� ���������� ��c�������
    //    countSuppliers++;
    //    Cell cell;
    //    for (int i = 0; i < needs.size(); i++) {
    //        costMat[][i].push_back(cell);//������ ������� ��������� �������������
    //    }
    //    stocks.push_back(sumNeeds - sumStocks);
    //}

}


bool containIndexes(int i, int j, int size, const vector<int>& indexIinChain, const vector<int>& indexJinChain) {
    if (size == 0)
        return false;
    for (int m = 0; m < size; m++) {
        if (indexIinChain[m] == i && indexJinChain[m] == j) //���� ������ � �������� ��������� ��� ���� � ������� ��� �����
            return true;
    }
    return false;
}
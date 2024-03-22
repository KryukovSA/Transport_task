#include"Method_potentials.h"
#include <fstream>
#include<numeric>
#include <chrono>
#include "Parallel.h"
// может с наследованием че придкмать чтобы типо новый конструкор где количество  и пункты с электрогрузовиками задается переопредлелить указанные ниже методы


//метод минимального эл для электротранспортаЪ
//сначала  смотрим статусы и заполняем что с электро потом стандартный
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
            if (costMat[i][j].get_electric() == true) {//заполнение электропереовoзок
                if (stocks_[i] > needs_[j] && needs_[j] != 0) {
                    costMat[i][j].set_cargoVolueme(needs_[j]);
                    stocks_[i] -= needs_[j];//уменьшим обьем доступных запасов
                    needs_[j] = 0;
                    costMat[i][j].set_status(basic);
                    busyColIndex.push_back(j);
                }
                else if (stocks_[i] == needs_[j] && needs_[j] != 0) {
                    costMat[i][j].set_cargoVolueme(needs_[j]);
                    stocks_[i] -= needs_[j];
                    needs_[j] = 0;
                    costMat[i][j].set_status(basic);
                    busyColIndex.push_back(j);//удовлетворены потребности
                    busyStrIndex.push_back(i);// запасы кончились и строку более не рассматриваем
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
        for (int i = 0; i < stocks_.size(); i++) {//ищем я чейку с самым маленьким тарифом в матрице
            if (!count(busyStrIndex.begin(), busyStrIndex.end(), i))//если строка не распределена еще (false когда count 0 вернул)
                for (int j = 0; j < needs_.size(); j++) {
                    if (costMat[i][j].get_tarif() <= min && costMat[i][j].get_status() == free_ && !count(busyColIndex.begin(), busyColIndex.end(), j)) {
                        minIndexI = i;//минимум тут  после первого не обновляется
                        minIndexJ = j;
                        min = costMat[i][j].get_tarif();
                    }
                }
        }
        if (stocks_[minIndexI] > needs_[minIndexJ] && needs_[minIndexJ] != 0) {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs_[minIndexJ]);
            stocks_[minIndexI] -= needs_[minIndexJ];//уменьшим обьем доступных запасов
            needs_[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);
        }
        else if (stocks_[minIndexI] == needs_[minIndexJ] && needs_[minIndexJ] != 0) {
            costMat[minIndexI][minIndexJ].set_cargoVolueme(needs_[minIndexJ]);
            stocks_[minIndexI] -= needs_[minIndexJ];
            needs_[minIndexJ] = 0;
            costMat[minIndexI][minIndexJ].set_status(basic);
            busyColIndex.push_back(minIndexJ);//удовлетворены потребности
            busyStrIndex.push_back(minIndexI);// запасы кончились и строку более не рассматриваем
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
            if (closeTypeTask == false) {//в фиктивные клетки нулевые тарифы(пока тольок если запасы больше)
                for (int k = 0; k < stocks_.size(); k++) {
                    costMat[k][needs_.size() - 1].set_tarif(0);
                }
            }
            return;
        }
    }

}


void Method_potentials::redistributionSupplies_elctric() {//последовательная для проверки ответа электро
    //calculatePotencials(costMat, suppliersPotincials, сonsumerPotincials);
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;

    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//вырожденность убирает мб в этом месте не надо
            addNullTransportation();
            //calculatePotencials();//? надо ли
        }
        
        vector<vector<Cell>> tmpCostMat = costMat;
        old_cost = calculatingСosts();
        Cell min = tmpCostMat[0][0];
        int minIndexI = 0;
        int minIndexJ = 0;
        //находим клетку с наим разностью c-u-v для начала цикла пересчета
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            for (int j = 0; j < сonsumerPotincials.size(); j++) {
                if (tmpCostMat[i][j].get_status() == free_)
                    if (tmpCostMat[i][j].get_defferncTarifAndPotincials() < min.get_defferncTarifAndPotincials()) {
                        min = tmpCostMat[i][j];
                        minIndexI = i;
                        minIndexJ = j;
                    }

            }
        }
        //build halfchain
        //цикл как то строим +- выделяем, все клетки с минимальной поставкой станут нулевыми
        tmpCostMat[minIndexI][minIndexJ].set_signInHalfChain(positive);
        tmpCostMat[minIndexI][minIndexJ].set_status(basic);

        vector<int> indexIinChain;//номер строки для ячейки цикла
        vector<int> indexJinChain;//номер столбца
        indexIinChain.push_back(minIndexI);//запоминаем индексы начала цикла
        indexJinChain.push_back(minIndexJ);
        int currentIndexI = minIndexI;//то где мы на текущем шаге
        int currentIndexJ = minIndexJ;
        int countCycles = 0;
        int i = 0;
        int j = 0;
        int counter = 0;
        while ((((currentIndexI != minIndexI || currentIndexJ != minIndexJ) || indexIinChain.size() <= 4)) && countCycles < suppliersPotincials.size() * сonsumerPotincials.size()) {// && counter < stocks.size()
            if (currentIndexI == minIndexI && currentIndexJ == minIndexJ && indexIinChain.size() <= 4) {//количество попаданий в начальную точку.
                counter++;
            }
            countCycles++;
            int flag = 0;
            //по вертикали в выбранном столбце ищем нужную строку
            for (i; i < suppliersPotincials.size(); i++) {
                if (tmpCostMat[i][currentIndexJ].get_status() == basic && i != currentIndexI && (!containIndexes(i, currentIndexJ, indexIinChain.size(), indexIinChain, indexJinChain) || (i == minIndexI && currentIndexJ == minIndexJ))) {

                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == positive)
                        tmpCostMat[i][currentIndexJ].set_signInHalfChain(negative);//ранее просто это было
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);

                    indexIinChain.push_back(i);//запоминаем индексы клетки (номер строки)
                    indexJinChain.push_back(currentIndexJ);
                    currentIndexI = i;
                    flag = 1;
                    j = 0;
                    break;
                }
            }
            if (flag == 0)       //если в столбце не нашли клетку для продолжения цикла, 
            {     // то в поисках другого столбца, а текущий удалим
                j = indexJinChain.back() + 1;//пропусти элемент на котором цикл сбился
                indexJinChain.erase(indexJinChain.end() - 1);
                indexIinChain.erase(indexIinChain.end() - 1);//?
                currentIndexJ = indexJinChain.back();

            }
            flag = 0;
            //по Горизонтали в выбрангой строке ищем нужный столбец
            for (j; j < сonsumerPotincials.size(); j++) {

                if (tmpCostMat[currentIndexI][j].get_status() == basic && j != currentIndexJ && (!containIndexes(currentIndexI, j, indexIinChain.size(), indexIinChain, indexJinChain) || (currentIndexI == minIndexI && j == minIndexJ))) {
                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == negative)
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);//ранее просто это было
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(negative);

                    indexIinChain.push_back(currentIndexI);//запоминаем индексы клетки 
                    indexJinChain.push_back(j);//запоминаем номер столбца
                    currentIndexJ = j;
                    flag = 1;
                    i = 0;
                    break;
                }
            }
            if (flag == 0)       //если в строке не нашли клетку для продолжения цикла, 
            {     // то в поисках другой строки, а текущий удалим
                i = indexIinChain.back() + 1;//пропусти элемент на котором цикл сбился
                indexIinChain.erase(indexIinChain.end() - 1);
                indexJinChain.erase(indexJinChain.end() - 1);//?
                currentIndexI = indexIinChain.back();

            }
        }
        if (countCycles >= suppliersPotincials.size() * сonsumerPotincials.size()) {//|| counter >= 5
            cout << "Cycle not found" << "\n";
            break;
        }
        int minI = indexIinChain[1];
        int minJ = indexJinChain[1];

        //ищем минимум поставок в помеченных negative клетках
        double minCargoVolume = tmpCostMat[indexIinChain[1]][indexJinChain[1]].get_cargoVolueme();
        for (int k = 0; k < indexIinChain.size(); k++) {
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() < minCargoVolume) {//можно улучшить чтоб шел по нечетным и все
                minCargoVolume = tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme();
                minI = indexIinChain[k];
                minJ = indexJinChain[k];
            }

        }
        //-------------------------------не дать занулить электроперевозку--------------------------------------------
        for (int k = 0; k < indexIinChain.size(); k++) { //если занулятся несколько и одна из них с электроперевозкой
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_electric() == true &&
                tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() == tmpCostMat[minI][minJ].get_cargoVolueme()) {
                std::cout << "клетка с электроперевозкой занулится" << endl;
                break;
            }
        }
        if (costMat[minI][minJ].get_electric() == true) {
            std::cout << "calculating cost: " << calculatingСosts() << endl;
            std::cout << "зануление клетки с электроперевозкой недопустимо" << endl;
            break;
        }
        //------------------------------------------------------------------------------------
        //перераспределяем поставки в результирующую матрицу
        if (minCargoVolume != 0) {
            for (int k = 0; k < indexIinChain.size() - 1; k++) {// -1 тк клетка начала цикла дважды повторяется
                if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative) {//можно улучшить чтоб шел по нечетным и все
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
            std::cout << "calculating cost: " << static_cast<long long>(calculatingСosts()) << endl;
            std::cout << "redistribution volume = 0" << endl;//либо в цикл добавляли нулевую перевозку, либо при перераспределении две занулились, но мы как и следует лишь одну убрали из базовых
            break;
        }

        //showPostavki();//----------------------------------------------
        //меняем статусы обнуленных клеток
        updateStatuses(indexIinChain, indexJinChain);

        std::cout << "calculating cost: " << static_cast<long long>(calculatingСosts()) << endl;
        new_cost = calculatingСosts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 3) { //3 это наугад
            std::cout << "cost not change because break" << endl; //вероятно не понадобится так как я не допускаю нулевое перераспределение
            break;
        }
        calculatePotencials();
    }
}

//параллельный метод перераспределения
void Method_potentials::redistributionSupplies_parallel() {
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;


    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//вырожденность убирает мб в этом месте не надо
            addNullTransportation();
        //calculatePotencials_parallel();//
        }
        vector<vector<Cell>> tmpCostMat = costMat;

        old_cost = calculatingСosts();
        Cell min = tmpCostMat[0][0];
        int minIndexI = 0;
        int minIndexJ = 0;

        //находим клетку с наим разностью c-u-v для начала цикла пересчета
//#pragma omp parallel for collapse(2) shared(tmpCostMat, min, minIndexI, minIndexJ)
//#pragma omp parallel for collapse(2) schedule(static)
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            for (int j = 0; j < сonsumerPotincials.size(); j++) {
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
        //цикл как то строим +- выделяем, все клетки с минимальной поставкой станут нулевыми
        tmpCostMat[minIndexI][minIndexJ].set_signInHalfChain(positive);
        tmpCostMat[minIndexI][minIndexJ].set_status(basic);

        vector<int> indexIinChain;//номер строки для ячейки цикла
        vector<int> indexJinChain;//номер столбца
        indexIinChain.push_back(minIndexI);//запоминаем индексы начала цикла
        indexJinChain.push_back(minIndexJ);
        int currentIndexI = minIndexI;//то где мы на текущем шаге
        int currentIndexJ = minIndexJ;
        int countCycles = 0;
        int i = 0;
        int j = 0;
        int counter = 0;

        while ((((currentIndexI != minIndexI || currentIndexJ != minIndexJ) || indexIinChain.size() <= 4)) && 
            countCycles < suppliersPotincials.size() * сonsumerPotincials.size()) {// && counter < stocks.size()
            if (currentIndexI == minIndexI && currentIndexJ == minIndexJ && indexIinChain.size() <= 4) {//количество попаданий в начальную точку.
                counter++;
            }
            countCycles++;
            int flag = 0;
            //по вертикали в выбранном столбце ищем нужную строку
            for (i; i < suppliersPotincials.size(); i++) {
                if (tmpCostMat[i][currentIndexJ].get_status() == basic && i != currentIndexI && 
                    (!containIndexes(i, currentIndexJ, indexIinChain.size(), indexIinChain, indexJinChain) ||
                        (i == minIndexI && currentIndexJ == minIndexJ))) {

                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == positive)
                        tmpCostMat[i][currentIndexJ].set_signInHalfChain(negative);//ранее просто это было
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);

                        indexIinChain.push_back(i);//запоминаем индексы клетки (номер строки)
                        indexJinChain.push_back(currentIndexJ);
                    
                    currentIndexI = i;
                    flag = 1;
                    j = 0;
                    break;
                }
            }
            if (flag == 0)       //если в столбце не нашли клетку для продолжения цикла, 
            {     // то в поисках другого столбца, а текущий удалим
                j = indexJinChain.back() + 1;//пропусти элемент на котором цикл сбился

                    indexJinChain.erase(indexJinChain.end() - 1);
                    indexIinChain.erase(indexIinChain.end() - 1);//?
                
                currentIndexJ = indexJinChain.back();

            }
            flag = 0;
            //по Горизонтали в выбрангой строке ищем нужный столбец
            for (j; j < сonsumerPotincials.size(); j++) {

                if (tmpCostMat[currentIndexI][j].get_status() == basic && j != currentIndexJ && (!containIndexes(currentIndexI, j, indexIinChain.size(), indexIinChain, indexJinChain) || (currentIndexI == minIndexI && j == minIndexJ))) {
                    if (tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == negative)
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(positive);//ранее просто это было
                    else
                        tmpCostMat[currentIndexI][j].set_signInHalfChain(negative);
                        indexIinChain.push_back(currentIndexI);//запоминаем индексы клетки 
                        indexJinChain.push_back(j);//запоминаем номер столбца
                    
                    currentIndexJ = j;
                    flag = 1;
                    i = 0;
                    break;
                }
            }
            if (flag == 0)       //если в строке не нашли клетку для продолжения цикла, 
            {     // то в поисках другой строки, а текущий удалим
                i = indexIinChain.back() + 1;//пропусти элемент на котором цикл сбился
                    indexIinChain.erase(indexIinChain.end() - 1);
                    indexJinChain.erase(indexJinChain.end() - 1);//?
                currentIndexI = indexIinChain.back();

            }
        }
        if (countCycles >= suppliersPotincials.size() * сonsumerPotincials.size()) {//|| counter >= 5
            cout << "Cycle not found" << "\n";
            break;
        }
        int minI = indexIinChain[1];
        int minJ = indexJinChain[1];

        //ищем минимум поставок в помеченных negative клетках
        double minCargoVolume = tmpCostMat[indexIinChain[1]][indexJinChain[1]].get_cargoVolueme();
//#pragma omp parallel for
        for (int k = 0; k < indexIinChain.size(); k++) {
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() < minCargoVolume) {//можно улучшить чтоб шел по нечетным и все
//#pragma omp critical
  //              {
                    minCargoVolume = tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme();
                    minI = indexIinChain[k];
                    minJ = indexJinChain[k];
              //  }
            }

        }

        //-------------------------------не дать занулить электроперевозку--------------------------------------------
        for (int k = 0; k < indexIinChain.size(); k++) { //если занулятся несколько и одна из них с электроперевозкой
            if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative && tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_electric() == true &&
                tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_cargoVolueme() == tmpCostMat[minI][minJ].get_cargoVolueme()) {
                std::cout << "клетка с электроперевозкой занулится" << endl;
                break;
            }
        }

        if (costMat[minI][minJ].get_electric() == true) {
            std::cout << "calculating cost: " << calculatingСosts() << endl;
            std::cout << "зануление клетки с электроперевозкой недопустимо" << endl;
            break;
        }

        //------------------------------------------------------------------------------------
        //перераспределяем поставки в результирующую матрицу
        if (minCargoVolume != 0) {
            for (int k = 0; k < indexIinChain.size() - 1; k++) {// -1 тк клетка начала цикла дважды повторяется
                if (tmpCostMat[indexIinChain[k]][indexJinChain[k]].get_signInHalfChain() == negative) {//можно улучшить чтоб шел по нечетным и все
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
            std::cout << "calculating cost: " << calculatingСosts() << endl;
            std::cout << "redistribution volume = 0" << endl;//либо в цикл добавляли нулевую перевозку, либо при перераспределении две занулились, но мы как и следует лишь одну убрали из базовых
            break;
        }
        //showPostavki();//----------------------------------------------
        //меняем статусы обнуленных клеток
        updateStatuses(indexIinChain, indexJinChain);

        std::cout << "calculating cost: " << static_cast<long long>(calculatingСosts()) << endl;
        new_cost = calculatingСosts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 3) { //3 это наугад
            std::cout << "cost not change because break" << endl; //вероятно не понадобится так как я не допускаю нулевое перераспределение
            break;
        }
        calculatePotencials_parallel();
    }

}


//солве для параллельной
//дрцгой минимальный применяем
void Method_potentials::solve_parallel(int electric_count) {
    if (!checkCloseTypeTask()) {//если не закрытая
        addDataForClosingTask();
        closeTypeTask = false;
    }
    add_electric(electric_count);
    methodMinElem_electric();
    //showPostavki();//первый опорный план

    cout << "result cost after minimal elem method: " << calculatingСosts() << endl;
    while (checkDegeneratePlan()) {//вырожденность убирает
        addNullTransportation();
    }
    //calculatePotencials();
    calculatePotencials_parallel();
    auto start_time = std::chrono::high_resolution_clock::now();
    redistributionSupplies_parallel();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    cout << "result minimal cost: " << fixed << static_cast<long long>(calculatingСosts()) << endl;
    std::cout << "execution time: " << duration.count() << " second." << std::endl;
    showPostavki();
    cout << endl;
}

void Method_potentials::solve_electric_sequence(int electric_count) {
    if (!checkCloseTypeTask()) {//если не закрытая
        addDataForClosingTask();
        closeTypeTask = false;
    }
    add_electric(electric_count);
    methodMinElem_electric();
    //showPostavki();//первый опорный план

    cout << "result cost after minimal elem method: " << calculatingСosts() << endl;
    while (checkDegeneratePlan()) {//вырожденность убирает
        addNullTransportation();
    }
    calculatePotencials();
    auto start_time = std::chrono::high_resolution_clock::now();
    redistributionSupplies_elctric();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    cout << "result minimal cost: " << fixed << static_cast<long long>(calculatingСosts()) << endl;
    std::cout << "execution time: " << duration.count() << " second." << std::endl;
    showPostavki();
    cout << endl;
}

//добавление электроперевозок в задачу
//в рандомных ячейках флаг и тариф
void Method_potentials::add_electric(int count) {
    if (count > countSuppliers)
        cout << "\n неверное количество электрогрузовиков \n";
    for (int i = 0; i < count; i++) {
        costMat[i][i*2].set_electric();
        costMat[i][i*2].set_tarif(costMat[i][i].get_tarif()/2);//тариф на 2 делим по условию задумано
    }
    
}

void Method_potentials::calculatePotencials_parallel() {
    double identefikator = -100.1786;

    // Заполнение suppliersPotincials и сonsumerPotincials параллельно
#pragma omp parallel for
    for (int i = 0; i < suppliersPotincials.size(); i++) {
        suppliersPotincials[i] = identefikator;
    }
#pragma omp parallel for
    for (int i = 0; i < сonsumerPotincials.size(); i++) {
        сonsumerPotincials[i] = identefikator;
    }

    int max_count_basic = 0;
    int index = 0;

    // Нахождение индекса строки с наибольшим количеством базовых ячеек
#pragma omp parallel for reduction(max:max_count_basic) shared(index)
    for (int m = 0; m < suppliersPotincials.size(); m++) {
        int max_count_tmp = 0;
        for (int n = 0; n < сonsumerPotincials.size(); n++) {
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

    // Выполнение основного расчета
    while (counter < suppliersPotincials.size() - 1) {
#pragma omp parallel for
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            if (suppliersPotincials[i] != identefikator) {
                for (int j = 0; j < сonsumerPotincials.size(); j++) {
                    if (costMat[i][j].get_status() == basic)
                        сonsumerPotincials[j] = costMat[i][j].get_tarif() - suppliersPotincials[i];
                }
            }
            if (suppliersPotincials[i] == identefikator) {
                for (int j = 0; j < сonsumerPotincials.size(); j++) {
                    if (costMat[i][j].get_status() == basic && сonsumerPotincials[j] != identefikator)
                        suppliersPotincials[i] = costMat[i][j].get_tarif() - сonsumerPotincials[j];
                }
            }
        }
        counter++;
    }
}
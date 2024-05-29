#include"Method_potentials.h"
#include <fstream>
#include<numeric>
#include <chrono>
#include <unordered_set>

using namespace libxl;

void Method_potentials::solve() {
    
    //showTable();
    if (!checkCloseTypeTask()) {//если не закрытая
        addDataForClosingTask();
        closeTypeTask = false;
    }
    methodMinElem();
    //showPostavki();//первый опорный план

    //cout << "result cost after minimal elem method: " << static_cast<long long>(calculatingСosts()) << endl;
    while (checkDegeneratePlan()) {//вырожденность убирает
        addNullTransportation();
    }
    calculatePotencials();
    auto start_time = std::chrono::high_resolution_clock::now();
    //redistributionSupplies();
    redistributionSuppliesNewShema();//до тех пор пока не будет оптимальным, вычисления потенциалов внутри метода
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
    cout << "стоимость перевозок без использования электрогрузовиков" << endl;
    cout << "result minimal cost: " << static_cast<long long>(calculatingСosts()) << endl;
    //std::cout << "execution time: " << duration.count() << " second." << std::endl;
    //showPostavki();
    cout << endl;
}
    bool isForbidden(int i, int j, const unordered_set<int>&forbiddenCells, int countConsumers) {
        return forbiddenCells.find(i * countConsumers + j) != forbiddenCells.end();
    };
    enum Direction {
        RIGHT, DOWN, LEFT, UP
    };
    bool containIndexes1(int i, int j, const vector<int>& indexIinChain, const vector<int>& indexJinChain) {
        for (size_t k = 0; k < indexIinChain.size(); ++k) {
            if (indexIinChain[k] == i && indexJinChain[k] == j) {
                return true;
            }
        }
        return false;
    }
void Method_potentials::redistributionSuppliesNewShema() {



    //calculatePotencials(costMat, suppliersPotincials, сonsumerPotincials);
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;

    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//вырожденность убирает, тут необходимо? 
            addNullTransportation();
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

        vector<int> indexIinChain; // номер строки для ячейки цикла
        vector<int> indexJinChain; // номер столбца
        unordered_set<int> forbiddenCells; // набор запрещенных ячеек
        indexIinChain.push_back(minIndexI);//запоминаем индексы начала цикла
        indexJinChain.push_back(minIndexJ);
        int currentIndexI = minIndexI;//то где мы на текущем шаге
        int currentIndexJ = minIndexJ;
        int countCycles = 0;
        int i = 0;
        int j = 0;
        int counter = 0;
        int signal = 1;
        Direction direction = RIGHT;
        Direction prev_direction;
        while ((((currentIndexI != minIndexI || currentIndexJ != minIndexJ) || indexIinChain.size() <= 4)) && countCycles < 100*suppliersPotincials.size() * сonsumerPotincials.size()) {// && counter < stocks.size()
            if (currentIndexI == minIndexI && currentIndexJ == minIndexJ && indexIinChain.size() <= 4) {//количество попаданий в начальную точку.
                counter++;
            }
            countCycles++;
            int flag = 0;
            if (indexIinChain.front() == indexIinChain.back() && indexJinChain.front() != indexJinChain.back() && indexIinChain.size() >= 4)
                break;
            if (indexJinChain.front() == indexJinChain.back() && indexIinChain.front() != indexIinChain.back() && indexIinChain.size() >= 4)
                break;
            // Определяем следующую ячейку в зависимости от направления
            switch (direction) {
            case RIGHT:
                for (int j = currentIndexJ + 1; j < countConsumers; ++j) {
                    if (costMat[currentIndexI][j].get_status() == basic && !containIndexes1(currentIndexI, j, indexIinChain, indexJinChain) && !isForbidden(currentIndexI, j, forbiddenCells, countConsumers)) {
                        currentIndexJ = j;
                        flag = 1;
                        prev_direction = direction;
                        direction = DOWN;
                        break;
                    }
                }
                break;
            case DOWN:
                for (int i = currentIndexI + 1; i < countSuppliers; ++i) {
                    if (costMat[i][currentIndexJ].get_status() == basic && !containIndexes1(i, currentIndexJ, indexIinChain, indexJinChain) && !isForbidden(i, currentIndexJ, forbiddenCells, countConsumers)) {
                        currentIndexI = i;
                        flag = 1;
                        prev_direction = direction;
                        direction = LEFT;
                        break;
                    }
                }
                break;
            case LEFT:
                for (int j = currentIndexJ - 1; j >= 0; --j) {
                    if (costMat[currentIndexI][j].get_status() == basic && !containIndexes1(currentIndexI, j, indexIinChain, indexJinChain) && !isForbidden(currentIndexI, j, forbiddenCells, countConsumers)) {
                        currentIndexJ = j;
                        flag = 1;
                        prev_direction = direction;
                        direction = UP;
                        break;
                    }
                }
                break;
            case UP:
                for (int i = currentIndexI - 1; i >= 0; --i) {
                    if (costMat[i][currentIndexJ].get_status() == basic && !containIndexes1(i, currentIndexJ, indexIinChain, indexJinChain) && !isForbidden(i, currentIndexJ, forbiddenCells, countConsumers)) {
                        currentIndexI = i;
                        flag = 1;
                        prev_direction = direction;
                        direction = RIGHT;
                        break;
                    }
                }
                break;
            }

            if (flag ) { 
                signal == 1;
                if (tmpCostMat[indexIinChain.back()][indexJinChain.back()].get_signInHalfChain() == positive)
                    tmpCostMat[currentIndexI][currentIndexJ].set_signInHalfChain(negative);//ранее просто это было
                else
                    tmpCostMat[currentIndexI][currentIndexJ].set_signInHalfChain(positive);
                indexIinChain.push_back(currentIndexI);
                indexJinChain.push_back(currentIndexJ);
               
            }
            else { // если не нашли подходящую ячейку, откатываемся
                if (signal == 2) {
                    signal == 0;
                     forbiddenCells.insert(currentIndexI * countConsumers + currentIndexJ); // добавляем в запрещенные
                    if (indexIinChain.size() == 1 && indexJinChain.size() == 1 && counter >= 5) {
                        countCycles = countSuppliers * countConsumers;
                        break;
                    }
                    indexIinChain.pop_back();
                    indexJinChain.pop_back();
                }
                if (!indexIinChain.empty() && !indexJinChain.empty()) {
                    currentIndexI = indexIinChain.back();
                    currentIndexJ = indexJinChain.back();
                }
                direction = static_cast<Direction>((direction + 2) % 4); // меняем направление на противоположное
                signal++;
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
        //    std::cout << "calculating cost: " << static_cast<long long>(calculatingСosts()) << endl;
        //    std::cout << "redistribution volume = 0" << endl;//либо в цикл добавляли нулевую перевозку, либо при перераспределении две занулились, но мы как и следует лишь одну убрали из базовых
            break;
        }


        //showPostavki();//----------------------------------------------

        //меняем статусы обнуленных клеток
        updateStatuses(indexIinChain, indexJinChain);

        //std::cout << "calculating cost: " << static_cast<long long>(calculatingСosts()) << endl;
        new_cost = calculatingСosts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 3) { //3 это наугад
            std::cout << "cost not change because break" << endl; //вероятно не понадобится так как я не допускаю нулевое перераспределение
            break;
        }

        /*  for (int i = 0; i < suppliersPotincials.size(); i++) {
              cout << suppliersPotincials[i] << " ";
          }
          cout << endl;

          for (int i = 0; i < сonsumerPotincials.size(); i++)
              cout << сonsumerPotincials[i] << " ";
          cout << endl;*/
        calculatePotencials();
    }
}






int Method_potentials::get_value(int i, int j) {
    return costMat[i][j].get_cargoVolueme();

}


//void Method_potentials::Method_potentials_init() {
//    Book* book = xlCreateXMLBook(); // Создание объекта книги
//    if (book) {
//        if (book->load(L"data.xlsx")) { // Загрузка файла Excel
//            Sheet* sheet = book->getSheet(0); // Получение первого листа
//            if (sheet) {
//
//                int numRows = sheet->lastRow();
//                for (int row = sheet->firstRow()+2; row < numRows; ++row) {
//                    stocks.push_back(sheet->readNum(row, 0));
//                }
//                countSuppliers = stocks.size();
//                cout << "колическтва запасов  " << countSuppliers << endl;
//                // Теперь у вас есть вектор с данными из Excel
//                for (const auto& data : stocks) {
//                    cout << " Value: " << data << endl;
//                }
//            }
//        }
//        book->save(L"example.xlsx");
//        book->release(); // Освобождение памяти, занятой объектом книги
//    }
//
//
//}

void Method_potentials::save_example() {
    // Создаем новую книгу Excel
    Book* book = xlCreateBook();

    if (book) {
        // Добавляем новый лист
        Sheet* sheet = book->addSheet(L"Sheet1");



        // Запись первого вектора в столбец A, начиная с третьей строки
        for (int i = 0; i < stocks.size(); ++i) {
            sheet->writeNum(i + 2, 0, stocks[i]);
        }

        // Запись второго вектора в строку 2, начиная с ячейки B
        for (int i = 0; i < needs.size(); ++i) {
            sheet->writeNum(1, i + 1, needs[i]);
        }

        for (int i = 0; i < stocks.size(); ++i) {
            for (int j = 0; j < needs.size(); ++j) {
                sheet->writeNum(i + 2, j + 1, costMat[i][j].get_tarif());
            }
        }

        // Сохраняем книгу
        book->save(L"output.xls");

        // Закрываем книгу
        book->release();
    }
}







void Method_potentials::method_potentials_init(int flag, int listnum, vector<vector<Cell>>& costMat_) {

    if (flag == 0) {
        Book* book = xlCreateXMLBook(); // Создание объекта книги
        if (book) {
            if (book->load(L"data.xlsx")) { // Загрузка файла Excel
                Sheet* sheet = book->getSheet(listnum); // Получение первого листа
                if (sheet) {


                    costMat = costMat_;
                    //set_countSuppliers(500);
                    //set_countConsumers(500);

                    //generate_transport_task();

                }
            }
            //book->save(L"example.xlsx");
            book->release(); // Освобождение памяти, занятой объектом книги
        }



    }
    else {
        Book* book = xlCreateXMLBook(); // Создание объекта книги
        if (book) {
            if (book->load(L"data.xlsx")) { // Загрузка файла Excel
                Sheet* sheet = book->getSheet(listnum); // Получение первого листа
                if (sheet) {
                    set_countSuppliers(200);
                    set_countConsumers(200);
                    int numRows = sheet->lastRow();
                    for (int row = sheet->firstRow() + 2; row < numRows; ++row) {
                        stocks.push_back(sheet->readNum(row, 0));
                    }
                    countSuppliers = stocks.size();

                    
                    //количество колонок
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
                        //if (!std::isnan(sheet->readNum(1, col))) {
                            needs.push_back(sheet->readNum(1, col));
                        //}
                       // else { break; }
                    } 
                   
                    countConsumers = needs.size();
                    
                    costMat.resize(countSuppliers, vector<Cell>(countConsumers));
                    suppliersPotincials.resize(countSuppliers);
                    сonsumerPotincials.resize(countConsumers);
                    int i = 0;
                    
                    for (int row = sheet->firstRow() + 2; row < numRows; ++row) {//??????
                        int j = 0;
                        for (int col = sheet->firstCol() + 1; col <= sheet->firstCol() + numCols; ++col) {
                            
                            costMat[row-2][col-1].set_tarif(sheet->readNum(row, col));
                            j++;
                        }
                        i++;
                    }
          


                    //cout << "колическтва запасов  " << countSuppliers << endl;
                    //// Теперь у вас есть вектор с данными из Excel
                    //for (const auto& data : stocks) {
                    //    cout << " Value: " << data << endl;
                    //}
                    //cout << "колическтво потребностей  " << countConsumers << endl;
                    //// Теперь у вас есть вектор с данными из Excel
                    //for (const auto& data : needs) {
                    //    cout << " Value: " << data << endl;
                    //}
                }
            }
            book->save(L"example.xlsx");
            book->release(); // Освобождение памяти, занятой объектом книги
        }




    }

}

vector<vector<Cell>> Method_potentials::generate_transport_task() {

   
    stocks.clear();
    needs.clear();

    costMat.resize(countSuppliers, vector<Cell>(countConsumers));
    suppliersPotincials.resize(countSuppliers);
    сonsumerPotincials.resize(countConsumers);

    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < countSuppliers; i++) {
        stocks.push_back(rand() % 2500 + 1);
    }
    for (int i = 0; i < countConsumers; i++) {
        needs.push_back(rand() % 2500 + 100);
    }
    int totalStocks = accumulate(stocks.begin(), stocks.end(), 0);
    int totalNeeds = accumulate(needs.begin(), needs.end(), 0);
    if (totalNeeds > totalStocks) {
        stocks[0] += (totalNeeds - totalStocks);
    }
    else if (totalNeeds < totalStocks) {
        needs[0] += (totalStocks - totalNeeds);
    }
    //генерация тарифов
    for (int i = 0; i < countSuppliers; i++) {
        for (int j = 0; j < countConsumers; j++) {
            costMat[i][j].set_tarif(static_cast<int>(getRandomNumber(1000, 2000)));
        }
    }
    return costMat;
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
    vector<double> stocks_ = stocks;
    vector<double> needs_ = needs;
    int min = costMat[0][0].get_tarif();
    int minIndexI = 0;
    int minIndexJ = 0;
    int a = 0;
    vector<int> busyStrIndex;
    vector<int> busyColIndex;
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


void Method_potentials::showPostavki() {
    cout << endl;
    if (countConsumers < 30) {
        for (int i = 0; i < countSuppliers; i++)
        {
            for (int j = 0; j < countConsumers; j++)
                std::cout << costMat[i][j].get_cargoVolueme() << "\t";
            cout << endl;
        }
    }
    else {
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 15; j++)
                std::cout << costMat[i][j].get_cargoVolueme() << "\t";
            cout << endl;
        }
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
    const double identefikator = -100.1786;

    for (int i = 0; i < suppliersPotincials.size(); i++) {
        suppliersPotincials[i] = identefikator;
    }
    for (int i = 0; i < сonsumerPotincials.size(); i++) {
        сonsumerPotincials[i] = identefikator;
    }

    int max_count_basic = 0;
    int index = 0;
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
    while (counter < suppliersPotincials.size() - 1) {
        for (int i = 0; i < suppliersPotincials.size(); i++) {
            if (suppliersPotincials[i] != identefikator) {
                for (int j = 0; j < сonsumerPotincials.size(); j++)
                {
                    if (costMat[i][j].get_status() == basic)
                        сonsumerPotincials[j] = costMat[i][j].get_tarif() - suppliersPotincials[i];
                }
            }
            if (suppliersPotincials[i] == identefikator) {
                for (int j = 0; j < сonsumerPotincials.size(); j++)
                {
                    if (costMat[i][j].get_status() == basic && сonsumerPotincials[j] != identefikator) {
                        suppliersPotincials[i] = costMat[i][j].get_tarif() - сonsumerPotincials[j];
                        break;
                    }
                }
            }
        }
        counter++;
    }
}



bool Method_potentials::checkOptimal() {

    bool optimal = true;
    //считакм оценки свободных клеток
    for (int i = 0; i < suppliersPotincials.size(); i++) {
        for (int j = 0; j < сonsumerPotincials.size(); j++) {
            if (costMat[i][j].get_status() == free_)
                costMat[i][j].set_defferncTarifAndPotincials(costMat[i][j].get_tarif() - suppliersPotincials[i] - сonsumerPotincials[j]);
        }
    }

    for (int i = 0; i < suppliersPotincials.size(); i++) {
        for (int j = 0; j < сonsumerPotincials.size(); j++) {
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
    for (int i = 0; i < indexIinChain.size() - 1; i++) { // -1 тк в цепочке начальная клетка записана еще и в конце // с одного может начинать чтоб первая клетка не становилась свободной?(ошибка на маленькой таблице в итоге)
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
          costMat[indexIinChain[i]][indexJinChain[i]].set_status(free_);//в ходе пересчета освободиться должна только одна клетка????
          break;
        }
    }
}






void Method_potentials::redistributionSupplies() {
    //calculatePotencials(costMat, suppliersPotincials, сonsumerPotincials);
    int old_cost = 0;
    int new_cost = 1;
    int colision = 0;

    while (checkOptimal() != true) {//

        if (checkDegeneratePlan()) {//вырожденность убирает, тут необходимо? 
            addNullTransportation();
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
                    if(tmpCostMat[currentIndexI][currentIndexJ].get_signInHalfChain() == negative)
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
        if (countCycles >= suppliersPotincials.size() * сonsumerPotincials.size() ) {//|| counter >= 5
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
        //    std::cout << "calculating cost: " << static_cast<long long>(calculatingСosts()) << endl;
        //    std::cout << "redistribution volume = 0" << endl;//либо в цикл добавляли нулевую перевозку, либо при перераспределении две занулились, но мы как и следует лишь одну убрали из базовых
            break;
        }


        //showPostavki();//----------------------------------------------

        //меняем статусы обнуленных клеток
        updateStatuses(indexIinChain, indexJinChain);
 
        //std::cout << "calculating cost: " << static_cast<long long>(calculatingСosts()) << endl;
        new_cost = calculatingСosts();
        if (new_cost == old_cost) {
            colision++;
        }
        if (colision > 3) { //3 это наугад
            std::cout << "cost not change because break" << endl; //вероятно не понадобится так как я не допускаю нулевое перераспределение
            break;
        }
        
      /*  for (int i = 0; i < suppliersPotincials.size(); i++) {
            cout << suppliersPotincials[i] << " ";
        }
        cout << endl;

        for (int i = 0; i < сonsumerPotincials.size(); i++)
            cout << сonsumerPotincials[i] << " ";
        cout << endl;*/
        calculatePotencials();
    }
}

double Method_potentials::calculatingСosts() {
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
            return true;//клетка должна игнорироваться
    }
    return false;
}

void Method_potentials::addNullTransportation() {
    vector<int> ignoreIndexI;//клетки которые однажды образовали цикл мы не рассматриваем
    vector<int> ignoreIndexJ;


    while (checkDegeneratePlan()) {
        int cellIndexI = 0;//индексы клетки которую хотим добавить в базис с нулеволй перевозкой.
        int cellIndexJ = 0;
        for (int k = 0; k < countSuppliers; k++) {//ищем первую попавшуюся клетку с 0 перевозкой 
            bool signal = 0;
            for (int m = 0; m < countConsumers; m++) {
                if (costMat[k][m].get_cargoVolueme() == 0 && costMat[k][m].get_status() == free_ && !ignoreCell(k, m, ignoreIndexI, ignoreIndexJ)) {
                    cellIndexI = k;
                    cellIndexJ = m;//выделим ее как кандидата
                    costMat[cellIndexI][cellIndexJ].set_status(basic);//чтоб поиск не сбивался допускаем ее базовой(потом если что отменим)
                    signal = 1;
                    break;
                }
            }
            if (signal == 1)
                break;

        }


        vector<int> indexIinChain;//номер строки для ячейки цикла
        vector<int> indexJinChain;//номер столбца
        indexIinChain.push_back(cellIndexI);//запоминаем индексы начала для поиска цикла
        indexJinChain.push_back(cellIndexJ);
        int currentIndexI = cellIndexI;
        int currentIndexJ = cellIndexJ;
        int countCycles = 0;
        int i = 0;
        int j = 0;
        int counter = 0;
        while ((((currentIndexI != cellIndexI && currentIndexJ != cellIndexJ) || indexIinChain.size()<2) && counter < 5) && countCycles < countSuppliers * countConsumers) {
            countCycles++;
            int flag = 0;
            if (currentIndexI == cellIndexI && currentIndexJ == cellIndexJ && indexIinChain.size() <= 4) {//количество попаданий в начальную точку.
                counter++;
            }
            //по вертикали в выбранном столбце ищем нужную строку
            for (i; i < countSuppliers; i++) {
                if (costMat[i][currentIndexJ].get_status() == basic && i != currentIndexI && (!containIndexes(i, currentIndexJ, indexIinChain.size(), indexIinChain, indexJinChain) || (i == cellIndexI && currentIndexJ == cellIndexJ))) {//
                    indexIinChain.push_back(i); //запоминаем индексы клетки (номер строки)
                    indexJinChain.push_back(currentIndexJ);
                    currentIndexI = i;
                    flag = 1;
                    j = 0;
                    break;
                }
            }
            if (flag == 0)       //если в столбце не нашли клетку для продолжения цикла, 
            {     // то в поисках другого столбца, а текущий удалим
                if (indexJinChain.size() == 1 || indexIinChain.size() == 1) {//чтобы из пустого вектора не удаляли
                    countCycles = countSuppliers * countConsumers;
                    break;
                }
                j = indexJinChain.back() + 1;//пропустим элемент на котором цикл сбился
                indexJinChain.erase(indexJinChain.end() - 1);
                indexIinChain.erase(indexIinChain.end() - 1);
                currentIndexJ = indexJinChain.back();
            }
            flag = 0;
            //по горизонтали в выбрангой строке ищем нужный столбец
            for (j; j < countConsumers; j++) {
                if (costMat[currentIndexI][j].get_status() == basic && j != currentIndexJ && (!containIndexes(currentIndexI, j, indexIinChain.size(), indexIinChain, indexJinChain) || (currentIndexI == cellIndexI && j == cellIndexJ))) {//
                    indexIinChain.push_back(currentIndexI);//запоминаем индексы клетки 
                    indexJinChain.push_back(j);//запоминаем номер столбца
                    currentIndexJ = j;
                    flag = 1;
                    i = 0;
                    break;
                }
            }
            if (flag == 0)       
            {     
                if (indexIinChain.size() == 1 || indexJinChain.size() == 1) {
                    countCycles = countSuppliers * countConsumers;
                    break;
                }
                i = indexIinChain.back() + 1;
                indexIinChain.erase(indexIinChain.end() - 1);
                indexJinChain.erase(indexJinChain.end() - 1);
                currentIndexI = indexIinChain.back();

            }
        }
        //countCycles == countSuppliers * countConsumers ||
        if (countCycles >= countSuppliers * countConsumers || ((currentIndexI == cellIndexI && currentIndexJ == cellIndexJ) && indexIinChain.size() <= 4)) { //цикл не найден/ пришли в исходную клетку при длинне цепочки меньше 5   //countSuppliers * countConsumers
            costMat[cellIndexI][cellIndexJ].set_status(basic);//добавляем нулевую перевозку                                                      потому что если бы она была >5 то это значит что просто цикл замкнулся
        }
        else {
            costMat[cellIndexI][cellIndexJ].set_status(free_);
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
    cell.set_tarif(findMax() + 1);//установим большой тариф, чтоб в методе минимального элемента фиктивные клетки последними заполнялись(потом его сделаем 0)
    if (sumStocks > sumNeeds) {//добавим фмктивного потребителя
        countConsumers++;
        for (int i = 0; i < stocks.size(); i++) {
            costMat[i].push_back(cell);//клетки имеющие начальную инициализацию
        }
        needs.push_back(sumStocks - sumNeeds);
    }
    //if (sumStocks < sumNeeds) {//добавим фмктивного поcтавщика
    //    countSuppliers++;
    //    Cell cell;
    //    for (int i = 0; i < needs.size(); i++) {
    //        costMat[][i].push_back(cell);//клетка имеющая начальную инициализацию
    //    }
    //    stocks.push_back(sumNeeds - sumStocks);
    //}

}


bool containIndexes(int i, int j, int size, const vector<int>& indexIinChain, const vector<int>& indexJinChain) {
    if (size == 0)
        return false;
    for (int m = 0; m < size; m++) {
        if (indexIinChain[m] == i && indexJinChain[m] == j) //если ячейка с дангными индексами уже есть в цепочке для цикла
            return true;
    }
    return false;
}
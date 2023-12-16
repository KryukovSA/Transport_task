#include "pch.h"
#include "../Transport_task/Method_potentials.h"
#include "../Transport_task/Method_potentials.cpp"


TEST(MethodPotentialsTest, GenerateTransportTask) {
    Method_potentials method(0,0);
    method.set_countSuppliers(450);
    method.set_countConsumers(460);

    // Вызываем метод генерации задачи
    method.generate_transport_task();

    // Проверяем, что размеры матрицы и векторов установлены правильно
    EXPECT_EQ(method.get_countConsumers(), 460);
    EXPECT_EQ(method.get_countSuppliers(), 450);


    // Проверяем, что тарифы установлены в правильном диапазоне
    for (int i = 0; i < 450; i++) {
        for (int j = 0; j < 460; j++) {
            int tarif = method.getCostMat()[i][j].get_tarif();
            EXPECT_GE(tarif, 30);  // Минимальный тариф
            EXPECT_LE(tarif, 2000);  // Максимальный тариф
        }
    }
}

TEST(MethodPotentialsTest, FindMax) {
    // Создаем объект класса Method_potentials
    Method_potentials method(0,0);
    method.set_countSuppliers(450);
    method.set_countConsumers(460);
    // Вызываем метод generate_transport_task(), чтобы иметь исходные данные
    method.generate_transport_task();

    // Получаем текущее состояние costMat
    auto currentCostMat = method.getCostMat();

    // Предположим, что первый элемент costMat - это максимальное значение
    double expectedMax = currentCostMat[0][0].get_tarif();

    // Ищем максимальное значение в текущем состоянии costMat
    for (size_t i = 0; i < currentCostMat.size(); ++i) {
        for (size_t j = 0; j < currentCostMat[i].size(); ++j) {
            double currentTarif = currentCostMat[i][j].get_tarif();
            if (currentTarif >= expectedMax) {
                expectedMax = currentTarif;
            }
        }
    }
    // Вызываем метод findMax()
    double actualMax = method.findMax();
    // Проверяем, что возвращенное значение совпадает с ожидаемым максимальным значением
    EXPECT_DOUBLE_EQ(expectedMax, actualMax);
}

TEST(MethodPotentialsTest, CheckDegeneratePlan) {
    Method_potentials method(1, 0);
    
    method.methodMinElem();
    

    // Вызываем метод checkDegeneratePlan()
    bool isDegenerate = method.checkDegeneratePlan();

    // Проверяем, что метод возвращает правильное значение для не вырожденного плана
    EXPECT_FALSE(isDegenerate);
}

TEST(MethodPotentialsTest, CalculatePotencials) {
    Method_potentials method(1, 0);

    method.methodMinElem();
    method.calculatePotencials();

    vector<double> suppliersPotincials = method.get_suppliersPotincials();
    vector<double> consumersPotincials = method.get_сonsumerPotincials();
    vector<double> a = { 0, 6, 4 };
    vector<double> b = { -2, -2, 1, 2 };
    // Проверяем, что метод возвращает правильное значение для не вырожденного плана
      // Проверяем, что тарифы установлены в правильном диапазоне
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(a[i], suppliersPotincials[i]);  // Минимальный тариф
    }
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(b[i], consumersPotincials[i]);  // Минимальный тариф
    }
}

TEST(MethodPotentialsTest, CheckOptimal) {
    Method_potentials method(1, 0);
    method.methodMinElem();
    method.calculatePotencials();
    EXPECT_FALSE(method.checkOptimal());
}

TEST(MethodPotentialsTest, RedistributionSupplies) {
    Method_potentials method(1, 0);
    method.methodMinElem();
    method.calculatePotencials();
    double cost1 = method.calculatingСosts();
    method.redistributionSupplies();
    double cost2 = method.calculatingСosts();
    EXPECT_TRUE(cost2<cost1);
}

TEST(MethodPotentialsTest, CheckCloseTypeTask) {
    Method_potentials method(1, 0);
    bool close_task = method.checkCloseTypeTask();
    EXPECT_TRUE(close_task);
}

TEST(MethodPotentialsTest, AddDataForClosingTask) {
    Method_potentials method(1, 11);
    bool close_task = method.checkCloseTypeTask();
    EXPECT_FALSE(close_task);
    method.addDataForClosingTask();
    close_task = method.checkCloseTypeTask();
    EXPECT_TRUE(close_task);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
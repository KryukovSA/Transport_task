#include "pch.h"
#include "../Transport_task/Method_potentials.h"
#include "../Transport_task/Method_potentials.cpp"


TEST(MethodPotentialsTest, GenerateTransportTask) {
    Method_potentials method(0,0);
    method.set_countSuppliers(450);
    method.set_countConsumers(460);

    // �������� ����� ��������� ������
    method.generate_transport_task();

    // ���������, ��� ������� ������� � �������� ����������� ���������
    EXPECT_EQ(method.get_countConsumers(), 460);
    EXPECT_EQ(method.get_countSuppliers(), 450);


    // ���������, ��� ������ ����������� � ���������� ���������
    for (int i = 0; i < 450; i++) {
        for (int j = 0; j < 460; j++) {
            int tarif = method.getCostMat()[i][j].get_tarif();
            EXPECT_GE(tarif, 30);  // ����������� �����
            EXPECT_LE(tarif, 2000);  // ������������ �����
        }
    }
}

TEST(MethodPotentialsTest, FindMax) {
    // ������� ������ ������ Method_potentials
    Method_potentials method(0,0);
    method.set_countSuppliers(450);
    method.set_countConsumers(460);
    // �������� ����� generate_transport_task(), ����� ����� �������� ������
    method.generate_transport_task();

    // �������� ������� ��������� costMat
    auto currentCostMat = method.getCostMat();

    // �����������, ��� ������ ������� costMat - ��� ������������ ��������
    double expectedMax = currentCostMat[0][0].get_tarif();

    // ���� ������������ �������� � ������� ��������� costMat
    for (size_t i = 0; i < currentCostMat.size(); ++i) {
        for (size_t j = 0; j < currentCostMat[i].size(); ++j) {
            double currentTarif = currentCostMat[i][j].get_tarif();
            if (currentTarif >= expectedMax) {
                expectedMax = currentTarif;
            }
        }
    }
    // �������� ����� findMax()
    double actualMax = method.findMax();
    // ���������, ��� ������������ �������� ��������� � ��������� ������������ ���������
    EXPECT_DOUBLE_EQ(expectedMax, actualMax);
}

TEST(MethodPotentialsTest, CheckDegeneratePlan) {
    Method_potentials method(1, 0);
    
    method.methodMinElem();
    

    // �������� ����� checkDegeneratePlan()
    bool isDegenerate = method.checkDegeneratePlan();

    // ���������, ��� ����� ���������� ���������� �������� ��� �� ������������ �����
    EXPECT_FALSE(isDegenerate);
}

TEST(MethodPotentialsTest, CalculatePotencials) {
    Method_potentials method(1, 0);

    method.methodMinElem();
    method.calculatePotencials();

    vector<double> suppliersPotincials = method.get_suppliersPotincials();
    vector<double> consumersPotincials = method.get_�onsumerPotincials();
    vector<double> a = { 0, 6, 4 };
    vector<double> b = { -2, -2, 1, 2 };
    // ���������, ��� ����� ���������� ���������� �������� ��� �� ������������ �����
      // ���������, ��� ������ ����������� � ���������� ���������
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(a[i], suppliersPotincials[i]);  // ����������� �����
    }
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(b[i], consumersPotincials[i]);  // ����������� �����
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
    double cost1 = method.calculating�osts();
    method.redistributionSupplies();
    double cost2 = method.calculating�osts();
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
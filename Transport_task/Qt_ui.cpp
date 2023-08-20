//
//#include <QApplication>
//#include <QWidget>
//#include <QLabel>
//#include <QPushButton>
//#include <QLineEdit>
//#include <QTableWidget>
//#include <QHBoxLayout>
//#include <QVBoxLayout>
//#include <fstream>
//#include "Method_potentials.h"
//#include "Cell.h"
//
//int main(int argc, char* argv[]) {
//    Method_potentials method_potencials;
//    method_potencials.solve();
//
//    QApplication app(argc, argv);
//
//    // �������� ������ � ������ ������
//    std::ifstream out("zapas.txt");
//    std::ifstream need("needs.txt");
//    // ... ������ ������ �� ������ � �������� �������� stocks � needs
//
//    // �������� �������� ����
//    QWidget window;
//    window.setWindowTitle("Transport Task Solver");
//
//    // �������� �������� ��� ����� ���������� ����������� � ������������
//    QLabel countSuppliersLabel("Input count Suppliers:");
//    QLabel countConsumersLabel("Input count Consumers:");
//    QLineEdit countSuppliersEdit;
//    QLineEdit countConsumersEdit;
//
//    // �������� ������ ��� ������� �������
//    QPushButton calculateButton("Calculate");
//
//    // �������� ������� ��� ����������� �����������
//    QTableWidget resultTable;
//
//    // ����������� ���������� ��������
//    QVBoxLayout layout;
//    layout.addWidget(&countSuppliersLabel);
//    layout.addWidget(&countSuppliersEdit);
//    layout.addWidget(&countConsumersLabel);
//    layout.addWidget(&countConsumersEdit);
//    layout.addWidget(&calculateButton);
//    layout.addWidget(&resultTable);
//    window.setLayout(&layout);
//
//    // ��������� ������� �� ������
//    QObject::connect(&calculateButton, &QPushButton::clicked, [&]() {
//    int countSuppliers = countSuppliersEdit.text().toInt();
//    int countConsumers = countConsumersEdit.text().toInt();
//
//    // ... �������� � ���������� �������� stocks � needs
//
//    // �������� � ���������� costMat � ������ ����������� ������
//
//    // ���������� ��������
//
//    // ����������� ����������� � �������
//    resultTable.setRowCount(countSuppliers);
//    resultTable.setColumnCount(countConsumers);
//    for (int i = 0; i < countSuppliers; ++i) {
//        for (int j = 0; j < countConsumers; ++j) {
//            QTableWidgetItem* item = new QTableWidgetItem(QString::number(method_potencials.get_value(i, j)));
//            resultTable.setItem(i, j, item);
//        }
//    }
//        });
//
//    // ����������� �������� ����
//    window.show();
//
//    return app.exec();
//}
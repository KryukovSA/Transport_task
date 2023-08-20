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
//    // Открытие файлов и чтение данных
//    std::ifstream out("zapas.txt");
//    std::ifstream need("needs.txt");
//    // ... Чтение данных из файлов и создание векторов stocks и needs
//
//    // Создание главного окна
//    QWidget window;
//    window.setWindowTitle("Transport Task Solver");
//
//    // Создание виджетов для ввода количества поставщиков и потребителей
//    QLabel countSuppliersLabel("Input count Suppliers:");
//    QLabel countConsumersLabel("Input count Consumers:");
//    QLineEdit countSuppliersEdit;
//    QLineEdit countConsumersEdit;
//
//    // Создание кнопки для запуска расчета
//    QPushButton calculateButton("Calculate");
//
//    // Создание таблицы для отображения результатов
//    QTableWidget resultTable;
//
//    // Организация компоновки виджетов
//    QVBoxLayout layout;
//    layout.addWidget(&countSuppliersLabel);
//    layout.addWidget(&countSuppliersEdit);
//    layout.addWidget(&countConsumersLabel);
//    layout.addWidget(&countConsumersEdit);
//    layout.addWidget(&calculateButton);
//    layout.addWidget(&resultTable);
//    window.setLayout(&layout);
//
//    // Обработка нажатия на кнопку
//    QObject::connect(&calculateButton, &QPushButton::clicked, [&]() {
//    int countSuppliers = countSuppliersEdit.text().toInt();
//    int countConsumers = countConsumersEdit.text().toInt();
//
//    // ... Создание и заполнение векторов stocks и needs
//
//    // Создание и заполнение costMat и других необходимых данных
//
//    // Выполнение расчетов
//
//    // Отображение результатов в таблице
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
//    // Отображение главного окна
//    window.show();
//
//    return app.exec();
//}
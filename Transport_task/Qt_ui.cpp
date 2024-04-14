//#include <QtWidgets>
//
//class MatrixProcessor : public QWidget {
//    Q_OBJECT
//
//public:
//    MatrixProcessor(QWidget* parent = nullptr) : QWidget(parent) {
//        // Создание виджетов
//        inputLabel1 = new QLabel("Начальный индекс i:");
//        inputLabel2 = new QLabel("Начальный индекс j:");
//        outputLabel = new QLabel("Подматрица:");
//
//        inputIndex1 = new QLineEdit;
//        inputIndex2 = new QLineEdit;
//        outputMatrix = new QTextEdit;
//
//        computeButton = new QPushButton("Вычислить");
//
//        // Размещение виджетов на форме
//        QVBoxLayout* layout = new QVBoxLayout;
//        layout->addWidget(inputLabel1);
//        layout->addWidget(inputIndex1);
//        layout->addWidget(inputLabel2);
//        layout->addWidget(inputIndex2);
//        layout->addWidget(computeButton);
//        layout->addWidget(outputLabel);
//        layout->addWidget(outputMatrix);
//
//        setLayout(layout);
//
//        // Подключение сигналов и слотов
//        connect(computeButton, &QPushButton::clicked, this, &MatrixProcessor::compute);
//    }
//
//public slots:
//    void compute() {
//        // Здесь вставьте ваш код для обработки матрицы
//        // Например:
//        int startIndexI = inputIndex1->text().toInt();
//        int startIndexJ = inputIndex2->text().toInt();
//
//        // Показываем подматрицу размером 20x20 начиная с указанных индексов
//        QString submatrix;
//        for (int i = startIndexI; i < startIndexI + 20; ++i) {
//            for (int j = startIndexJ; j < startIndexJ + 20; ++j) {
//                // Здесь должен быть ваш код для получения элементов матрицы
//                // и формирования подматрицы
//                // Например:
//                submatrix += QString::number(i + j) + "\t";
//            }
//            submatrix += "\n";
//        }
//        outputMatrix->setText(submatrix);
//    }
//
//private:
//    QLabel* inputLabel1;
//    QLabel* inputLabel2;
//    QLabel* outputLabel;
//    QLineEdit* inputIndex1;
//    QLineEdit* inputIndex2;
//    QTextEdit* outputMatrix;
//    QPushButton* computeButton;
//};
//
//int main(int argc, char* argv[]) {
//    QApplication app(argc, argv);
//    MatrixProcessor window;
//    window.setWindowTitle("Matrix Processor");
//    window.show();
//    return app.exec();
//}
//
////#include "main.moc"
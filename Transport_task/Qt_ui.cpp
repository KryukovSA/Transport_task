//#include <QtWidgets>
//
//class MatrixProcessor : public QWidget {
//    Q_OBJECT
//
//public:
//    MatrixProcessor(QWidget* parent = nullptr) : QWidget(parent) {
//        // �������� ��������
//        inputLabel1 = new QLabel("��������� ������ i:");
//        inputLabel2 = new QLabel("��������� ������ j:");
//        outputLabel = new QLabel("����������:");
//
//        inputIndex1 = new QLineEdit;
//        inputIndex2 = new QLineEdit;
//        outputMatrix = new QTextEdit;
//
//        computeButton = new QPushButton("���������");
//
//        // ���������� �������� �� �����
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
//        // ����������� �������� � ������
//        connect(computeButton, &QPushButton::clicked, this, &MatrixProcessor::compute);
//    }
//
//public slots:
//    void compute() {
//        // ����� �������� ��� ��� ��� ��������� �������
//        // ��������:
//        int startIndexI = inputIndex1->text().toInt();
//        int startIndexJ = inputIndex2->text().toInt();
//
//        // ���������� ���������� �������� 20x20 ������� � ��������� ��������
//        QString submatrix;
//        for (int i = startIndexI; i < startIndexI + 20; ++i) {
//            for (int j = startIndexJ; j < startIndexJ + 20; ++j) {
//                // ����� ������ ���� ��� ��� ��� ��������� ��������� �������
//                // � ������������ ����������
//                // ��������:
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
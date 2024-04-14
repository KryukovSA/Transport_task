//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <iostream>
//
//// ������� ��� ���������� ����� ��������� �������
//int calculate(const std::vector<std::vector<int>>& matrix) {
//    int sum = 0;
//    for (const auto& row : matrix) {
//        for (int element : row) {
//            sum += element;
//        }
//    }
//    return sum;
//}
//
//// ������� ��� ������ ���������� �������� 20x20, ������� � ��������� ��������
//void showSubmatrix(const std::vector<std::vector<int>>& matrix, int startRow, int startColumn) {
//    std::cout << "���������� 20x20, ������� � �������� (" << startRow << ", " << startColumn << "):\n";
//    for (int i = startRow; i < startRow + 20 && i < matrix.size(); ++i) {
//        for (int j = startColumn; j < startColumn + 20 && j < matrix[i].size(); ++j) {
//            std::cout << matrix[i][j] << " ";
//        }
//        std::cout << "\n";
//    }
//}
//
//int main() {
//    // �������� � ���������� ������� (������ ��������)
//    std::vector<std::vector<int>> matrix = {
//        {1, 2, 3, 4, 5},
//        {6, 7, 8, 9, 10},
//        {11, 12, 13, 14, 15},
//        // ��������� ���� �������
//    };
//
//    // �������� ����
//    sf::RenderWindow window(sf::VideoMode(800, 600), "��������� ���������");
//
//    // ������ ��� ������� ����������
//    sf::RectangleShape calculateButton(sf::Vector2f(100, 50));
//    calculateButton.setFillColor(sf::Color::Green);
//    calculateButton.setPosition(50, 50);
//
//    // �������� ������� ������ "���������"
//    bool calculationsCompleted = false;
//    while (window.isOpen() && !calculationsCompleted) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            else if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
//                    if (calculateButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
//                        // ������ ����������
//                        calculate(matrix);
//                        calculationsCompleted = true;
//                    }
//                }
//            }
//        }
//
//        window.clear();
//        window.draw(calculateButton);
//        window.display();
//    }
//
//    // ���� ��� ����� �������� ������ � �������
//    int startRow, startColumn;
//    std::cout << "������� ��������� ������: ";
//    std::cin >> startRow;
//    std::cout << "������� ��������� �������: ";
//    std::cin >> startColumn;
//
//    // ������ "Show" ��� ������ ������� ������ ����������
//    sf::RectangleShape showButton(sf::Vector2f(100, 50));
//    showButton.setFillColor(sf::Color::Blue);
//    showButton.setPosition(50, 150);
//
//    bool showButtonClicked = false;
//    while (window.isOpen() && !showButtonClicked) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            else if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
//                    if (showButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
//                        // ����� ����������
//                        showSubmatrix(matrix, startRow, startColumn);
//                        showButtonClicked = true;
//                    }
//                }
//            }
//        }
//
//        window.clear();
//        window.draw(showButton);
//        window.display();
//    }
//
//    return 0;
//}

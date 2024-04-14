//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <iostream>
//
//// Функция для вычисления суммы элементов матрицы
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
//// Функция для вывода подматрицы размером 20x20, начиная с указанных индексов
//void showSubmatrix(const std::vector<std::vector<int>>& matrix, int startRow, int startColumn) {
//    std::cout << "Подматрица 20x20, начиная с элемента (" << startRow << ", " << startColumn << "):\n";
//    for (int i = startRow; i < startRow + 20 && i < matrix.size(); ++i) {
//        for (int j = startColumn; j < startColumn + 20 && j < matrix[i].size(); ++j) {
//            std::cout << matrix[i][j] << " ";
//        }
//        std::cout << "\n";
//    }
//}
//
//int main() {
//    // Создание и заполнение матрицы (вектор векторов)
//    std::vector<std::vector<int>> matrix = {
//        {1, 2, 3, 4, 5},
//        {6, 7, 8, 9, 10},
//        {11, 12, 13, 14, 15},
//        // Дополните вашу матрицу
//    };
//
//    // Создание окна
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Матричный интерфейс");
//
//    // Кнопка для запуска вычислений
//    sf::RectangleShape calculateButton(sf::Vector2f(100, 50));
//    calculateButton.setFillColor(sf::Color::Green);
//    calculateButton.setPosition(50, 50);
//
//    // Ожидание нажатия кнопки "Вычислить"
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
//                        // Запуск вычислений
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
//    // Поля для ввода индексов строки и столбца
//    int startRow, startColumn;
//    std::cout << "Введите начальную строку: ";
//    std::cin >> startRow;
//    std::cout << "Введите начальный столбец: ";
//    std::cin >> startColumn;
//
//    // Кнопка "Show" для вызова функции вывода подматрицы
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
//                        // Показ подматрицы
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

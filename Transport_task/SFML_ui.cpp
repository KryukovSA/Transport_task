#include <SFML/Graphics.hpp>
#include <sstream> 
#include <fstream>
#include <iomanip>
#include <vector>
#include "Method_potentials.h" // Подключите ваш заголовочный файл Method_potentials.h


int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));
    Method_potentials method_potencials;
    int flag = 0;// 1 - из табл тарифы     0 - автогенерация
    vector<vector<Cell>> main_matrix;
    if(flag == 0)
        main_matrix = method_potencials.generate_transport_task();


    method_potencials.method_potentials_init(flag, 0, main_matrix); 





    //генератор возвращает саму матрицу
    //и после отработки  последовательного решателя, матрицу инициализируем заново и паралельный решатель применяем
    method_potencials.solve();
    method_potencials.method_potentials_init(flag, 0, main_matrix);
    method_potencials.solve_parallel();
  
    //method_potencials.Method_potentials_init();

    //method_potencials.solve();

    //sf::RenderWindow window(sf::VideoMode(2500, 2000), "Transport Task Solver");

    //sf::Font font;
    //if (!font.loadFromFile("Arial.ttf")) {
    //    return EXIT_FAILURE;
    //}

    //int margin = 50;
    ////sf::Text countSuppliersLabel("Input count Suppliers:", font, 70);
    ////sf::Text countConsumersLabel("Input count Consumers:", font, 70);
    //sf::Text calculateButton("Calculate", font, 60);
    ////позиции текста
    ////countSuppliersLabel.setPosition(10, 100);
    ////countConsumersLabel.setPosition(10, countSuppliersLabel.getPosition().y + countSuppliersLabel.getLocalBounds().height + margin);

    //sf::Text resultText;
    //resultText.setFont(font);
    //resultText.setCharacterSize(70);
    //resultText.setPosition(10, 500);

    ////sf::RectangleShape resultTable;
    ////resultTable.setFillColor(sf::Color::White);
    ////resultTable.setSize(sf::Vector2f(800, 1000));
    ////resultTable.setPosition(800, resultText.getPosition().y + resultText.getLocalBounds().height); // Позиционируем ниже текста


    ////sf::RectangleShape countSuppliersEdit;
    ////countSuppliersEdit.setFillColor(sf::Color::White);
    ////countSuppliersEdit.setSize(sf::Vector2f(100, 30));
    ////countSuppliersEdit.setPosition(countSuppliersLabel.getPosition().x + countSuppliersLabel.getLocalBounds().width + margin + 60, countSuppliersLabel.getPosition().y + margin);

    ////sf::RectangleShape countConsumersEdit;
    ////countConsumersEdit.setFillColor(sf::Color::White);
    ////countConsumersEdit.setSize(sf::Vector2f(100, 30));
    ////countConsumersEdit.setPosition(countConsumersLabel.getPosition().x + countSuppliersLabel.getLocalBounds().width + margin+60, countConsumersLabel.getPosition().y + margin);

    //while (window.isOpen()) {
    //    sf::Event event;
    //    while (window.pollEvent(event)) {
    //        if (event.type == sf::Event::Closed)
    //            window.close();
    //        if (event.type == sf::Event::TextEntered) {
    //            // Обработка ввода текста для countSuppliersEdit и countConsumersEdit
    //        }
    //        if (event.type == sf::Event::MouseButtonPressed) {
    //            // Обработка нажатия кнопки Calculate
    //            if (event.mouseButton.button == sf::Mouse::Left &&
    //                event.mouseButton.x >= calculateButton.getPosition().x &&
    //                event.mouseButton.x <= calculateButton.getPosition().x + calculateButton.getGlobalBounds().width &&
    //                event.mouseButton.y >= calculateButton.getPosition().y &&
    //                event.mouseButton.y <= calculateButton.getPosition().y + calculateButton.getGlobalBounds().height) {

    //                // Выполнение расчетов
    //                method_potencials.solve();


    //                // Отображение результатов в текстовом формате
    //                std::ostringstream resultStream; // Создание потока для форматированного вывода

    //                for (int i = 0; i < method_potencials.get_countSuppliers(); ++i) {
    //                    for (int j = 0; j < method_potencials.get_countConsumers(); ++j) {
    //                        resultStream << std::setw(8) << method_potencials.get_value(i, j); // Форматированный вывод
    //                    }
    //                    resultStream << "\n"; // Переход на новую строку
    //                }

    //                resultText.setString("Results:\n" + resultStream.str()); // Установка форматированного текста в объект resultText

    //                //resultText.setString("Results:\n");
    //                //for (int i = 0; i < method_potencials.get_countSuppliers(); ++i) {
    //                //    for (int j = 0; j < method_potencials.get_countConsumers(); ++j) {
    //                //        resultText.setString(resultText.getString() + std::to_string(method_potencials.get_value(i, j)) + "\t");
    //                //    }
    //                //    resultText.setString(resultText.getString() + "\n");
    //                //}

    //            }
    //        }
    //    }

    //    window.clear();

    ///*    window.draw(countSuppliersLabel);
    //    window.draw(countConsumersLabel);
    //    window.draw(countSuppliersEdit);
    //    window.draw(countConsumersEdit);*/
    //    window.draw(calculateButton);
    //    //window.draw(resultTable);
    //    window.draw(resultText);

    //    window.display();
    //}

    return 0;
}

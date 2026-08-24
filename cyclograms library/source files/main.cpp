#include "Cyclogram.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <sstream>
#include <limits>

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    setlocale(LC_ALL, "Russian");
    Cyclogram myCg;
    myCg.loadFromFile("data.txt"); 
    int choice;

    while (true) {
        cout << "\n1. Список" << endl;
        cout << "2. Добавить" << endl;
        cout << "3. Удалить" << endl;
        cout << "4. Изменить" << endl;
        cout << "5. Сохранить" << endl;
        cout << "6. Выход" << endl;
        cout << "Выбор: ";
        
        if (!(cin >> choice)) {
            cout << "Ошибка ввода" << endl;
            clearInput();
            continue;
        }

        if (choice == 6) break;

        switch (choice) {
            case 1:
                myCg.print();
                break;

            case 2: {
                Point p;
                string timeStr;
                cout << "Время (например, 0-5): ";
                cin >> timeStr;
                if (!parseInterval(timeStr, p.t_start, p.t_end)) {
                    cout << "Ошибка времени" << endl;
                    clearInput();
                    break;
                }
                
                cout << "X Y Z: ";
                if (!(cin >> p.x >> p.y >> p.z)) {
                    cout << "Нужны числа" << endl;
                    clearInput();
                    break;
                }
                
                if (!isValidCoordinate(p.x) or !isValidCoordinate(p.y) or !isValidCoordinate(p.z)) {
                    cout << "Выход за лимиты" << endl;
                    break;
                }

                myCg.addPoint(p);
                break;
            }

            case 3: {
                int idx;
                cout << "Номер: ";
                if (!(cin >> idx)) {
                    cout << "Нужно число" << endl;
                    clearInput();
                    break;
                }
                myCg.removePoint(idx);
                break;
            }

            case 4: {
                int idx;
                cout << "Номер: ";
                if (!(cin >> idx) or idx < 0 or idx >= myCg.getCount()) {
                    cout << "Ошибка номера" << endl;
                    clearInput();
                    break;
                }
                Point p;
                string timeStr;
                cout << "Новое время: ";
                cin >> timeStr;
                if (!parseInterval(timeStr, p.t_start, p.t_end)) {
                    cout << "Ошибка времени" << endl;
                    clearInput();
                    break;
                }
                cout << "Новые X Y Z: ";
                if (!(cin >> p.x >> p.y >> p.z)) {
                    cout << "Нужны числа" << endl;
                    clearInput();
                    break;
                }
                if (!isValidCoordinate(p.x) or !isValidCoordinate(p.y) or !isValidCoordinate(p.z)) {
                    cout << "Выход за лимиты" << endl;
                    break;
                }
                myCg.editPoint(idx, p);
                break;
            }

            case 5:
                myCg.saveToFile("data.txt");
                break;
                
            default:
                cout << "Неизвестная команда" << endl;
        }
    }
    return 0;
}

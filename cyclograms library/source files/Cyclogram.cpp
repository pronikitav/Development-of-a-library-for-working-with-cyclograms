#include "Cyclogram.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;


bool safeStringToDouble(const string& str, double& outVal) {
    stringstream ss(str);
    ss >> outVal;
    if (ss.fail()) return false;
    
    string rest;
    ss >> rest;
    if (!rest.empty()) return false;
    
    return true;
}

bool parseInterval(const string& str, double& start, double& end) {
    size_t dashPos = str.find('-');
    if (dashPos == string::npos or dashPos == 0 or dashPos == str.length() - 1) return false;
    
    if (!safeStringToDouble(str.substr(0, dashPos), start)) return false;
    if (!safeStringToDouble(str.substr(dashPos + 1), end)) return false;
    
    if (start < 0 or end < 0 or start > 1e7 or end > 1e7) return false;
    
    if (start == end) return false; 
    
    if (start > end) swap(start, end); 
    
    return true;
}

bool isValidCoordinate(double val) {
    return (val >= -100000.0 and val <= 100000.0);
}

void Cyclogram::addPoint(const Point& newP) {
    vector<Point> newPoints;
    for (const auto& p : points) {
        if (p.t_end <= newP.t_start or p.t_start >= newP.t_end) {
            newPoints.push_back(p);
        }
        else if (p.t_start >= newP.t_start and p.t_end <= newP.t_end) {
            continue; 
        }
        else if (p.t_start < newP.t_start and p.t_end > newP.t_end) {
            Point p1 = p; p1.t_end = newP.t_start;
            Point p2 = p; p2.t_start = newP.t_end;
            newPoints.push_back(p1);
            newPoints.push_back(p2);
        }
        else if (p.t_start < newP.t_start and p.t_end > newP.t_start and p.t_end <= newP.t_end) {
            Point p1 = p; p1.t_end = newP.t_start;
            newPoints.push_back(p1);
        }
        else if (p.t_start >= newP.t_start and p.t_start < newP.t_end and p.t_end > newP.t_end) {
            Point p1 = p; p1.t_start = newP.t_end;
            newPoints.push_back(p1);
        }
    }
    newPoints.push_back(newP);
    sort(newPoints.begin(), newPoints.end(), [](const Point& a, const Point& b) {
        return a.t_start < b.t_start;
    });
    points = newPoints;
    cout << "Готово" << endl;
}

void Cyclogram::removePoint(int index) {
    if (index >= 0 and index < (int)points.size()) {
        points.erase(points.begin() + index);
        cout << "Удалено" << endl;
    } else {
        cout << "Ошибка номера" << endl;
    }
}

void Cyclogram::editPoint(int index, const Point& newPoint) {
    if (index >= 0 and index < (int)points.size()) {
        points.erase(points.begin() + index);
        addPoint(newPoint);
    } else {
        cout << "Ошибка номера" << endl;
    }
}

void Cyclogram::saveToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& p : points) {
            file << p.t_start << "-" << p.t_end << " " << p.x << " " << p.y << " " << p.z << "\n";
        }
        file.close();
        if (file.fail()) {
            cout << "Ошибка записи" << endl;
        } else {
            cout << "Сохранено: " << filename << endl;
        }
    } else {
        cout << "Ошибка доступа" << endl;
    }
}

void Cyclogram::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        if (file.peek() == std::ifstream::traits_type::eof()) {
            cout << "Файл пуст" << endl;
            return;
        }

        points.clear();
        string line;
        int lineNum = 0;
        
        while (getline(file, line)) {
            lineNum++;
            if (line.empty()) continue; 

            stringstream ss(line);
            string intervalStr;
            Point p;
            
            if (ss >> intervalStr >> p.x >> p.y >> p.z) {
                if (isValidCoordinate(p.x) and isValidCoordinate(p.y) and isValidCoordinate(p.z)) {
                    if (parseInterval(intervalStr, p.t_start, p.t_end)) {
                        points.push_back(p);
                    } else {
                        cout << "Ошибка времени, строка " << lineNum << endl;
                    }
                } else {
                    cout << "Ошибка лимита, строка " << lineNum << endl;
                }
            } else {
                cout << "Ошибка данных, строка " << lineNum << endl;
            }
        }
        file.close();
        sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.t_start < b.t_start;
        });
        cout << "Загружено: " << filename << endl;
    } else {
        cout << "Файл не найден" << endl;
    }
}

void Cyclogram::print() {
    if (points.empty()) {
        cout << "Пусто" << endl;
        return;
    }
    cout << "\nСписок точек" << endl;
    for (size_t i = 0; i < points.size(); ++i) {
        cout << "[" << i << "] Время: " << points[i].t_start << "-" << points[i].t_end
             << "  Коорд: (" << points[i].x << ", " 
             << points[i].y << ", " << points[i].z << ")" << endl;
    }
    cout << "" << endl;
}

int Cyclogram::getCount() { return (int)points.size(); }

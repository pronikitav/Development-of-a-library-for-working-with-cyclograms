#include <vector>
#include <string>

struct Point {
    double t_start, t_end;
    double x, y, z;
};


bool safeStringToDouble(const std::string& str, double& outVal);
bool parseInterval(const std::string& str, double& start, double& end);
bool isValidCoordinate(double val);

class Cyclogram {
private:
    std::vector<Point> points;

public:
    void addPoint(const Point& newP);
    void removePoint(int index);
    void editPoint(int index, const Point& newPoint);
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void print();
    int getCount();
};



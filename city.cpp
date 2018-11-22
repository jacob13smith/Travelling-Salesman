#include "city.hpp"
#include <random>
#include <iomanip>

using namespace std;

constexpr int MIN_COORD = 0;
constexpr  int MAX_COORD = 1000;

int city::id = 0;

city::city() :  x{random_coord()}, y{random_coord()}, name{to_string(city::id++)}{}

int city::random_coord() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(MIN_COORD, MAX_COORD);
    return uni(rng);
}

ostream &operator<<(ostream &os, const city &c) {
    os << "ID: " << c.name << ", Longitude: " << setfill('0') << setw(4) << c.x << ", Latitude: " << setfill('0') << setw(4) << c.y << endl;
    return os;
}

double city::distance(const city& other) {
    double x_distance = abs(x - other.x);
    double y_distance = abs(y - other.y);
    return sqrt(pow(x_distance, 2) + pow(y_distance, 2));
}

bool operator==(const city &first, const city &second) {
    return first.name == second.name;
}

bool operator!=(const city &first, const city &second) {
    return !operator==(first, second);
}

#pragma once

#include <string>

using namespace std;

class city {
public:
    static int id;
    string name;
    city();
    int random_coord();
    int get_x();
    int get_y();
    friend ostream& operator<<(ostream& os, const city& c);
    friend bool operator==(const city &first, const city &second);
    friend bool operator!=(const city &first, const city &second);
    double distance(const city& other);
private:
    int x;
    int y;
};

#pragma once

#include <vector>
#include "city.hpp"

using namespace std;

class tour {
public:
    tour(vector<city> new_cities);
    tour();
    double get_fitness() const;
    friend ostream& operator<<(ostream& os, const tour& t);
    void add_city(city new_city);
    int number_cities();
    bool in_tour(city test_city);
    city get_city(int index);
private:
    void calculate_fitness();
    vector<city> cities;
    double fitness;
    double distance;
};

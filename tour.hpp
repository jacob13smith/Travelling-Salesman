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
    friend bool operator==(const tour &first, const tour &second);
    void add_city(city new_city);
    int number_cities() const;
    bool in_tour(city test_city);
    city get_city(int index) const;
    vector<city> get_cities();
    double get_distance();
    void calculate_fitness();
    void city_swap(int current, int other);
private:
    vector<city> cities;
    double fitness;
    double distance;
};

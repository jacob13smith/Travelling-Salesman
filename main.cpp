#include <random>
#include <iostream>
#include <vector>
#include "city.hpp"
#include "tour.hpp"
#include "population.hpp"
#include <algorithm>
#include <fstream>

constexpr int CITIES_IN_TOUR = 32;
constexpr int POPULATION_SIZE = 32;
constexpr int ITERATIONS = 1000;

using namespace std;

int main() {
    vector<city> cities;
    for (int i = 0; i < CITIES_IN_TOUR; i++){
        city temp = city();
        cities.push_back(temp);
    }
    population tours;
    for (int i = 0; i < POPULATION_SIZE; i++){
        tour temp = tour(cities);
        tours.add(temp);
    }
    for (int i = 0; i < ITERATIONS; i++){
        tours.iterate(i);
    }
    move_best_to_front(tours.list_of_tours);

    ofstream coords("../coordinates.json");
    coords << "{\"coordinates\":[\n";

    cities = tours.list_of_tours[0].cities;

    for (int i = 0; i < cities.size(); i++){
        coords << "{ \"x\" : " << cities[i].get_x() << ", \"y\" : " << cities[i].get_y() << " }";
        if (i != cities.size() - 1){
            coords << ",";
        }
        coords << "\n";
    }
    coords << "]}\n";

}

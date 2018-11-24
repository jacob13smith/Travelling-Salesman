#include <random>
#include <utility>
#include <algorithm>
#include "tour.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

int constexpr SCALAR_MULT = 10000;
constexpr int SHUFFLES = 6;

tour::tour(vector<city> new_cities) {
    cities = std::move(new_cities);
    for (int i = 0; i < SHUFFLES; i++){
        shuffle(cities.begin(), cities.end(), std::mt19937(std::random_device()()));
    }
    calculate_fitness();
}

tour::tour() : fitness{0}, distance{0} {}

void tour::calculate_fitness() {
    double total = 0;
    for (int i = 0; i < cities.size() - 1; i ++){
        total += cities[i].distance(cities[i + 1]);
    }
    distance = total;
    fitness = SCALAR_MULT/distance;
}

double tour::get_fitness() const {
    return fitness;
}

ostream &operator<<(ostream &os, const tour &t) {
    os << "Distance: " << setw(6) << t.distance << " Fitness: " << setw(6) << t.fitness << endl;
    return os;
}

void tour::add_city(city new_city) {
    cities.push_back(new_city);
}

int tour::number_cities() {
    return static_cast<int>(cities.size());
}

bool tour::in_tour(city test_city) {
    for (const city &temp : cities){
        if (test_city.name == temp.name){
            return true;
        }
    }
    return false;
}

city tour::get_city(int index) {
    return cities[index];
}

double tour::get_distance() {
    return distance;
}



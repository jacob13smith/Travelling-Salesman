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
constexpr int ITERATIONS = 10000;

using namespace std;

int main() {
    // Create cities and store in vector
    vector<city> cities;
    for (int i = 0; i < CITIES_IN_TOUR; i++){
        city temp = city();
        cities.push_back(temp);
    }

    // Create a population of tours
    population tours;
    for (int i = 0; i < POPULATION_SIZE; i++){
        tour temp = tour(cities);
        tours.add(temp);
    }

    // Iterate the predetermined number of times
    // Might be changed to improvement factor
    for (int i = 0; i < ITERATIONS; i++){
        tours.iterate(i);
    }

    // Move the best tour to front after all the iterations
    move_best_to_front(tours.list_of_tours);

    // Open file to write best list of coordinates
    ofstream coords("../coordinates.csv");

    // If the file was found, write coordinates then launch Java drawing program I wrote :)
    if (coords) {
        cities = tours.list_of_tours[0].cities;

        for (auto &city : cities) {
            coords << city.get_x() << "," << city.get_y() << "\n";
        }
        coords.close();
        system("cd ../\njava Main");
    }
}

#include <random>
#include <iostream>
#include <vector>
#include "city.hpp"
#include "tour.hpp"
#include "population.hpp"
#include <algorithm>
#include <fstream>
#include <thread>

constexpr int CITIES_IN_TOUR = 32;
constexpr int POPULATION_SIZE = 32;
constexpr int ITERATIONS = 1000;

using namespace std;

void write_to_file(vector<city> cities, const string title){
    // Open file to write best list of coordinates
    ofstream coords("../coordinates.csv");

    // If the file was found, write coordinates then launch Java drawing program I wrote :)
    if (coords) {

        for (auto &city : cities) {
            coords << city.get_x() << "," << city.get_y() << "\n";
        }
        coords.close();
        string string_command = "cd ../\njava Main " + title + " &";
        const char* command = string_command.c_str();
        system(command);
    }
}

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

    // Create thread for first Java instance
    thread java_before(write_to_file, tours.get_list_of_tours()[0].get_cities(), "Generation-1");

    // Iterate the predetermined number of times
    // Might be changed to improvement factor
    tours.move_best_to_front();
    for (int i = 0; i < ITERATIONS; i++){
        tours.iterate();
    }

    // Create thread for second Java instance
    thread java_after(write_to_file, tours.get_list_of_tours()[0].get_cities(), "Generation-" + to_string(tours.get_gen()));

    // Join both threads and end program
    java_after.join();
    java_before.join();
}

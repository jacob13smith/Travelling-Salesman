#include <random>
#include <iostream>
#include <vector>
#include "city.hpp"
#include "tour.hpp"
#include "population.hpp"
#include <algorithm>
#include <fstream>
#include <thread>
#include <iomanip>
constexpr int PRINT_WIDTH = 30;

constexpr int DEFAULT_CITIES_IN_TOUR = 32;
constexpr int DEFAULT_POPULATION_SIZE = 32;
constexpr int DEFAULT_ITERATIONS = 1000;

int CITIES_IN_TOUR = DEFAULT_CITIES_IN_TOUR;
int POPULATION_SIZE = DEFAULT_POPULATION_SIZE;
int ITERATIONS = DEFAULT_ITERATIONS;

using namespace std;

void write_to_file(vector<city> cities, const string &title){
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

void print_constants(){
    cout << "STARTING ALGORITHM USING:\n";
    cout << setw(PRINT_WIDTH) << left << "Cities per tour: " << CITIES_IN_TOUR << "\n";
    cout << setw(PRINT_WIDTH) << left << "Tours per population: " << POPULATION_SIZE << "\n";
    population::print_constants();
    cout << setw(PRINT_WIDTH) << left << "Iterations: " << ITERATIONS << "\n\n";
}

void ask_user(){
    char answer;
    cout << "Use default settings?[Y/n]\n";
    cin >> answer;
    if (answer == 'n'){
        cout << "Number of cities in a tour:\n";
        cin >> CITIES_IN_TOUR;
        cout << "Number of tours in a population:\n";
        cin >> POPULATION_SIZE;
        cout << "Number of iterations:\n";
        cin >> ITERATIONS;
        population::ask_user();
    }

}

int main() {
    // Ask user for values
    ask_user();
    print_constants();
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
    tours.move_best_to_front();

    // Create thread for first Java instance
    int distance_before = (int) tours.get_list_of_tours()[0].get_distance();
    thread java_before(write_to_file, tours.get_list_of_tours()[0].get_cities(), "\"Gen: 1 - Distance: " + to_string(distance_before) + "\"");

    // Iterate the predetermined number of times
    // Might be changed to improvement factor
    for (int i = 0; i < ITERATIONS; i++){
        tours.iterate();
    }

    // Create thread for second Java instance
    int distance_after = (int) tours.get_list_of_tours()[0].get_distance();
    thread java_after(write_to_file, tours.get_list_of_tours()[0].get_cities(), "\"Gen: "
    + to_string(tours.get_gen()) + " - Distance: " + to_string(distance_after) + "\"");

    double improvement = 1.0 - ((double) distance_after / (double) distance_before);
    cout << setw(PRINT_WIDTH) << left << "\n\nBest beginning tour distance: " << distance_before << "\n";
    cout << setw(PRINT_WIDTH) << left << "Best ending tour distance: " << distance_after << "\n";
    cout << "Improvement: " << setw(3) << improvement * 100<< "%\n";

    // Join both threads and end program
    java_after.join();
    java_before.join();
}

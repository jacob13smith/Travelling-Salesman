#include <random>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "population.hpp"

constexpr int PARENT_POOL_SIZE = 5;
constexpr int NUMBER_OF_PARENTS = 3;
constexpr int MUTATION_RATE = 15;

void population::add(tour new_tour) {
    list_of_tours.push_back(new_tour);
}

void move_best_to_front(vector<tour> &list) {
    int index = 0;
    for(int i = 1; i < list.size(); i++){
        if (list[i].get_fitness() > list[index].get_fitness())
            index = i;
    }
    tour temp = list[index];
    swap(list[0], list[index]);
}

int random_index(int end){
    random_device random_device;
    mt19937 engine{random_device()};
    uniform_int_distribution<int> dist(0, static_cast<int>(end - 1));
    return dist(engine);
}

void population::iterate(int gen) {

    vector<tour> new_list;

    // Selection
    move_best_to_front(list_of_tours);
    new_list.push_back(list_of_tours[0]);

    // Crossover
    for (int i = 1; i < list_of_tours.size(); i++){
        // List of parent tours to be created
        vector<tour> parents;

        // Loop through NUMBER_OF_PARENTS
        for (int j = 0; j < NUMBER_OF_PARENTS; j++){
            // Create parent pool for selection
            vector<tour> parent_pool;
            for (int k = 0; k < PARENT_POOL_SIZE; k++){

                // Choose random tour from the list and make it a parent
                tour random_tour = list_of_tours[random_index(static_cast<int>(list_of_tours.size()))];
                parent_pool.push_back(random_tour);
            }
            // Choose the best parent from the list
            move_best_to_front(parent_pool);
            parents.push_back(parent_pool[0]);
        }

        // Create random list of indices for parents
        vector<int> indices;
        for (int j = 0; j < NUMBER_OF_PARENTS - 1; j++){
            indices.push_back(random_index(list_of_tours[0].number_cities()));
        }
        sort(indices.begin(), indices.end());
        indices.push_back(list_of_tours[0].number_cities());

        // New tour to be birthed
        tour new_tour;

        // Parent count is the index of the current parent being used to copy
        int parent_count = 0;

        // j is the number of city that is being inserted
        for (int j = 0; j < list_of_tours[0].number_cities(); j++){

            // curr is reset to j so that parents are copied from the proper index
            int curr = j;

            // While the new tour already has the parent's city at curr, increment curr
            while (new_tour.in_tour(parents[parent_count].get_city(curr % list_of_tours[0].number_cities()))) {
                curr++;
            }

            // Add current city to new tour at the back
            new_tour.add_city(parents[parent_count].get_city(curr % list_of_tours[0].number_cities()));

            // If we have hit the end of an index, increment the parent we are using
            if (j == indices[parent_count]){
                parent_count++;
            }
        }

        // Add new tour to the new list
        new_list.push_back(new_tour);
    }

    // Mutate
    for (int j = 1; j < new_list.size(); j++){
        for (int i = 0; i < new_list[0].number_cities(); i++){
            if (random_index(100) + 1 < MUTATION_RATE){
                swap(new_list[j].cities[i], new_list[j].cities[(i + 1) % new_list[0].number_cities()]);
            }
        }
    }

    // Evaluate
    for (tour &temp : new_list){
        temp.calculate_fitness();
    }
    list_of_tours = new_list;

    // Report
    report_fitnesses(gen);
}

void population::report_fitnesses(int gen){
    double total_fitness = 0;
    for (const tour &temp : list_of_tours){
        total_fitness += temp.get_fitness();
    }
    double average = total_fitness / list_of_tours.size();
    cout << flush;
    //cout << "Most fit: " << setw(6) << list_of_tours[0].get_fitness() << endl;
    cout << "Generation: " << gen << ", Least distance: " << setw(6) << list_of_tours[0].get_distance() << "\r";
    //cout << "Average fitness: " << setw(6) << average << "\n\n";
}

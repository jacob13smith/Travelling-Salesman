#include <random>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "population.hpp"

// Number of parents in each selection pool
constexpr int PARENT_POOL_SIZE = 8;

// Number of parents to birth new tours
constexpr int NUMBER_OF_PARENTS = 3;

// Percent chance a tour will mutate (calculated for each city in the tour)
constexpr int MUTATION_RATE = 7;

// Add a tour to the end of a population
// param new_tour : tour to be added
void population::add(tour new_tour) {
    list_of_tours.push_back(new_tour);
}

// Random number generator between 0 and end (exclusive)
// param end : exlusive upperbound of random number
int random_index(int end){
    random_device random_device;
    mt19937 engine{random_device()};
    uniform_int_distribution<int> dist(0, static_cast<int>(end - 1));
    return dist(engine);
}

// Perform an iteration on this population
// param this : population getting iterated
void population::iterate() {

    // Increment generation counter
    gen++;

    // New list of tours being created for this population
    vector<tour> new_list;
    new_list.push_back(list_of_tours[0]);

    // Crossover
    for (int i = 1; i < list_of_tours.size(); i++){
        // List of parent tours to be created
        vector<tour> parents;

        // Loop through NUMBER_OF_PARENTS
        for (int j = 0; j < NUMBER_OF_PARENTS; j++){
            // Create parent pool for selection
            population parent_pool;
            for (int k = 0; k < PARENT_POOL_SIZE; k++){
                tour random_tour = list_of_tours[random_index(static_cast<int>(list_of_tours.size()))];
                while (parent_pool.contains(random_tour)){
                    random_tour = list_of_tours[random_index(static_cast<int>(list_of_tours.size()))];
                }

                parent_pool.add(random_tour);
                // Choose random tour from the list and make it a parent
            }
            // Choose the best parent from the list
            parent_pool.move_best_to_front();
            parents.push_back(parent_pool.get_list_of_tours()[0]);
        }

        // Create random list of indices for parents
        vector<int> indices;
        for (int j = 0; j < NUMBER_OF_PARENTS - 1; j++){
            indices.push_back(random_index(list_of_tours[0].number_cities()));
        }
        // Sort indices in order
        sort(indices.begin(), indices.end());
        // Add the last index to the end of indices
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
                // Not sure the best method for mutating, either swapping with an adjacent, or a
                // completely random city.  Random city has given best results on average, especially
                // with more higher iterations

                int other = random_index(new_list[j].number_cities());
                new_list[j].city_swap(i, other);

                /*
                int beside = 1;
                if (random_index(1))
                    beside = -1;
                new_list[j].city_swap(i, i + beside % new_list[j].number_cities())
                 */
            }
        }
    }

    // Evaluate
    for (tour &temp : new_list){
        temp.calculate_fitness();
    }
    list_of_tours = new_list;


    // Selection  !! I moved this here because it makes more sense to move the elite after each generation
    move_best_to_front();

    // Report
    report_fitnesses();
}

// Report the current fitness level of the population
// Can change to report different things, I prefer just reporting the distance of the elite
void population::report_fitnesses(){
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

// Return list of tours
vector<tour> population::get_list_of_tours() {
    return list_of_tours;
}

int population::get_gen() {
    return gen;
}

void population::move_best_to_front() {

    int index = 0;
    for(int i = 1; i < list_of_tours.size(); i++){
        if (list_of_tours[i].get_fitness() > list_of_tours[index].get_fitness())
            index = i;
    }
    swap(list_of_tours[0], list_of_tours[index]);
}

bool population::contains(tour search) {
    for (const tour &current : list_of_tours){
        if (current == search) return true;
    }
    return false;
}

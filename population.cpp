#include <random>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "population.hpp"

constexpr int PRINT_WIDTH = 30;
constexpr int REPORT_FREQ = 200;

// Number of parents in each selection pool
constexpr int DEFAULT_PARENT_POOL_SIZE = 4;

// Number of parents to birth new tours
constexpr int DEFAULT_NUMBER_OF_PARENTS = 3;

// Percent chance a tour will mutate (calculated for each city in the tour)
constexpr int DEFAULT_MUTATION_RATE = 4;

// Prevents infinite loop looking for unique parents
constexpr int MAX_PARENT_SEARCH = 20;

constexpr int DEFAULT_ELITES = 20;

static int ELITES = DEFAULT_ELITES;
static int PARENT_POOL_SIZE = DEFAULT_PARENT_POOL_SIZE;
static int NUMBER_OF_PARENTS = DEFAULT_NUMBER_OF_PARENTS;
static int MUTATION_RATE = DEFAULT_MUTATION_RATE;

void population::ask_user(){
    cout << "Number of parents per selection pool:\n";
    cin >> PARENT_POOL_SIZE;
    cout << "Number of parents per offspring:\n";
    cin >> NUMBER_OF_PARENTS;
    cout << "Mutation chance of each city in each tour per iteration(%):\n";
    cin >> MUTATION_RATE;
    cout << "Number of elites saved per generation:\n";
    cin >> ELITES;
}

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
    uniform_int_distribution<int> dist(0, (end - 1));
    return dist(engine);
}

// Perform an iteration on this population
// param this : population getting iterated
void population::iterate() {
    if (gen == 1) {
        selection();
        report_fitnesses();
    }

    gen++;
    new_list.clear();
    new_list.push_back(list_of_tours[0]);

    // Crossover
    for (int i = 1; i < list_of_tours.size(); i++){
        new_list.push_back(crossover());
    }

    // Mutate
    mutate();

    // Evaluate
    for (tour &temp : new_list){
        temp.calculate_fitness();
    }
    list_of_tours = new_list;

    // Selection  !! I moved this here because it makes more sense to move the elite after each generation
    selection();

    // Report
    report_fitnesses();
}

void population::selection(){
    for (int i = 0; i < ELITES; i++){
        move_best_to_front(i);
    }
}

// Report the current fitness level of the population
// Can change to report different things, I prefer just reporting the distance of the elite
void population::report_fitnesses(){
    double total_fitness = 0;
    for (const tour &temp : list_of_tours){
        total_fitness += temp.get_fitness();
    }
    // double average = total_fitness / list_of_tours.size();
    cout << flush;
    //cout << "Most fit: " << setw(6) << list_of_tours[0].get_fitness() << endl;
    cout << "Generation: " << gen << ", Least distance: " << setw(6) << list_of_tours[0].get_distance() << "\r";
    //cout << "Average fitness: " << setw(6) << average << "\n\n";
    if (gen % REPORT_FREQ == 0 || gen == 1){
        cout << "\n";
    }
}

// Return list of tours
vector<tour> population::get_list_of_tours() {
    return list_of_tours;
}

int population::get_gen() {
    return gen;
}

void population::move_best_to_front(int index) {

    int best = index;
    for(int i = index + 1; i < list_of_tours.size(); i++){
        if (list_of_tours[i].get_fitness() > list_of_tours[best].get_fitness())
            best = i;
    }
    swap(list_of_tours[index], list_of_tours[best]);
}

bool population::contains(tour search) {
    for (const tour &current : list_of_tours){
        if (current == search) return true;
    }
    return false;
}

void population::print_constants() {
    cout << setw(PRINT_WIDTH) << left << "Parents per pool: " << PARENT_POOL_SIZE << "\n";
    cout << setw(PRINT_WIDTH) << left << "Parents per offspring: " << NUMBER_OF_PARENTS << "\n";
    cout << setw(PRINT_WIDTH) << left << "Mutation rate: " << MUTATION_RATE << "%\n";
    cout << setw(PRINT_WIDTH) << left << "Number of elites: " << ELITES << "\n";
}

// Crossover parents to create a new tour
tour population::crossover(){
    // List of parent tours to be created
    vector<tour> parents;

    // Loop through NUMBER_OF_PARENTS
    for (int j = 0; j < NUMBER_OF_PARENTS; j++){
        // Create parent pool for selection
        population parent_pool;
        for (int k = 0; k < PARENT_POOL_SIZE; k++){
            tour random_tour = list_of_tours[random_index(static_cast<int>(list_of_tours.size()))];
            int counter = 0;
            while (parent_pool.contains(random_tour) && counter < MAX_PARENT_SEARCH){
                random_tour = list_of_tours[random_index(static_cast<int>(list_of_tours.size()))];
                counter++;
            }

            parent_pool.add(random_tour);
            // Choose random tour from the list and make it a parent
        }
        // Choose the best parent from the list
        parent_pool.move_best_to_front(0);
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
    return new_tour;
}

// Mutate the next generation associated with this population
void population::mutate() {
    for (int j = 1; j < new_list.size(); j++){
        for (int i = 0; i < new_list[0].number_cities(); i++){
            if (random_index(100) + 1 < MUTATION_RATE){
                // Not sure the best method for mutating, either swapping with an adjacent, or a
                // completely random city.  Random city has given best results on average, especially
                // with higher iterations.


                int other = random_index(new_list[j].number_cities());
                new_list[j].city_swap(i, other);

                /*
                int beside = 1;
                if (random_index(1))
                    beside = -1;
                new_list[j].city_swap(i, (i + beside) % new_list[0].number_cities());
                */

            }
        }
    }
}

#include <random>
#include <algorithm>
#include "population.hpp"

constexpr int PARENT_POOL_SIZE = 5;
constexpr int NUMBER_OF_PARENTS = 2;

void population::add(tour new_tour) {
    list_of_tours.push_back(new_tour);
}

void move_best_to_front(vector<tour> &list) {
    int index = 0;
    for(int i = 1; i < list.size(); i++){
        if (list[i].get_fitness() > list[i].get_fitness())
            index = i;
    }
    tour temp = list[index];
    list.erase(list.begin() + index);
    list.insert(list.begin(), temp);
}

int random_index(int end){
    random_device random_device;
    mt19937 engine{random_device()};
    uniform_int_distribution<int> dist(0, static_cast<int>(end - 1));
    return dist(engine);
}

void population::iterate() {
    vector<tour> new_list;

    // Selection
    move_best_to_front(list_of_tours);

    // Crossover
    for (int i = 1; i < list_of_tours.size(); i++){
        vector<tour> parents;
        for (int j = 0; j < NUMBER_OF_PARENTS; j++){
            vector<tour> parent_pool;
            for (int k = 0; k < PARENT_POOL_SIZE; k++){
                tour random_tour = list_of_tours[random_index(static_cast<int>(list_of_tours.size()))];
                parent_pool.push_back(random_tour);
            }
            move_best_to_front(parent_pool);
            parents.push_back(parent_pool[0]);
        }
        vector<int> indices;
        for (int j = 0; j < NUMBER_OF_PARENTS; j++){
            indices.push_back(list_of_tours[0].number_cities());
        }
        indices.push_back(list_of_tours[0].number_cities());

        sort(indices.begin(), indices.end());
        tour new_tour;
        int parent_count = 0;
        for (int j = 0; j < list_of_tours[0].number_cities(); j++){
            int curr = j;
            while (new_tour.in_tour(parents[parent_count].get_city(curr % list_of_tours[0].number_cities()))) {
                curr++;
            }
            new_tour.add_city(parents[parent_count].get_city(curr % list_of_tours[0].number_cities()));
            if (j == indices[parent_count]){
                parent_count++;
            }
        }
    }
}

#pragma once

#include <vector>
#include "tour.hpp"

using namespace std;

class population {
public:
    static void ask_user();
    static void print_constants();
    void add(tour new_tour);
    void iterate();
    vector<tour> get_list_of_tours();
    int get_gen();
    void move_best_to_front(int index);
    void selection();
private:
    tour crossover();
    void mutate();
    void report_fitnesses();
    int gen = 1;
    vector<tour> list_of_tours;
    vector<tour> new_list;
    bool contains(tour search);
};

#pragma once

#include <vector>
#include "tour.hpp"

using namespace std;

class population {
public:
    void add(tour new_tour);
    void iterate();
    vector<tour> get_list_of_tours();
private:
    void report_fitnesses();
    int gen = 1;
    vector<tour> list_of_tours;
};

void move_best_to_front(vector<tour> &list);
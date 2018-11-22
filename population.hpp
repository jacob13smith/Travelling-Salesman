#pragma once

#include <vector>
#include "tour.hpp"

using namespace std;

class population {
public:
    void add(tour new_tour);
    void iterate();
private:
    vector<tour> list_of_tours;
};

void move_best_to_front(vector<tour> &list);
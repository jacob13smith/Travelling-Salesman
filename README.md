# Jacob's C++/Java Travelling Salesman

This program tries to find the most optimal route of randomly located cities/dots.

## USER NOTES

If you choose to not use the default settings at the beginning, you will be asked for new values.
The program doesn't sanitize your input (I didn't feel like writing the sanitation), so make sure you are inputting
proper values.  Below is the breakdown of values. They are all integers.

Number of cities in a tour ------------ Default: 32
This is the number of cities that the "salesman" needs to visit, higher number will take more time but produce
more interesting plots.


Number of tours in a population ------- Default: 32
This is the number of different routes the salesman can take per generation.  I'm not sure the optimal value for this
relative to the number of cities, but it seems to work well if this equals number of cities.

Number of iterations ------------------ Default: 1000
This is the number of generations that will be simulated.  There seems to be a logarithmic relationship of increasing
improvement and increasing iterations.  Of course, the more iterations, the better the output will be.

Number of parents per selection pool -- Default: 8
The number of parent routes that are randomly selected, from which the offsprings parents will be chosen.  Again,
not sure how this affects performance, so try some different values.

Number of parents per offspring ------- Default: 3
The number of parent routes that will be combined to form an offspring. These are the best routes selected from every
parent pool. Not sure how many parents is optimal, but I believe in monogamy so I try to keep it around 2-3 ;)

Mutation chance ----------------------- Default: 5
Every generation, every city in every route (except the elite) has a % chance of being randomly swapped with another
random city's spot in that route.  Lower mutation rate is favourable at lower iterations, but at higher iterations,
mutation rate needs to be high enough for the algorithm to be able to try completely new routes.  It's safe to keep
this around 5-15.

Number of elites ---------------------- Default: 1
This is the number of best routes that are saved from mutation every generation.  I have no clue how this affects
the effectiveness of the algorithm, but it seems like saving more than only the best could be useful.

# JAVA PLOT

This program uses a small JavaFX application to plot the visual improvement of the algorithm.  It uses a system command
to open the java class file (Main.class) which NEEDS TO BE LOCATED one directory higher than the C++ executable.

It's also important that there is a directory called "dev" in the directory one up from where the C++ executable is.
This directory is where the JVM spits all it's warnings and errors, but we aren't interested in those.
You still need to make sure you do not close an old JVM instance while running the algorithm again, or else it may not
be able to find the dev/null directory to print the errors and it will clutter the C++ console.

#ifndef KNN_UTIL_H
#define KNN_UTIL_H

#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "point.h"

// redefine pair for readability
typedef std::pair<int, std::vector<int>> PIV;

/* auxiliary function that reads a given file, line by line,
 * populating a vector with Points.
 */
std::vector <Point> readPoints(std::string filename);

/* auxiliary function that builds the string containing Point's neighbours,
 * in the form --> "ID: id1, id2, ... idk"
 */
std::string writeNeighbours(PIV p);

#endif //KNN_UTIL_H

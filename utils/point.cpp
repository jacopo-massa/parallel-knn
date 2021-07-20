#include "point.h"

Point::Point(int id, float x, float y) : id(id), x(x), y(y) {}

Point::Point() { Point(-1, 0, 0); }

float Point::squaredEuclideanDistance(const Point &point)
{
    float x = this->x - point.x;
    float y = this->y - point.y;

    return pow(x, 2) + pow(y, 2);
}

std::istream &operator>>(std::istream &is, Point &point)
{
    char space;
    is >> point.x >> space >> point.y;
    return is;
}

std::ostream &operator<<(std::ostream &os, const Point &point)
{
    os << point.id << " " << point.x << " " << point.y << " ";
    return os;
}

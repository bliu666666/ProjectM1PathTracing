#ifndef POSITION_H
#define POSITION_H

#include <cmath>

class Position {
private:
    double x, y, z;

public:
    Position();
    Position(double x, double y, double z);

    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double val);
    void setY(double val);
    void setZ(double val);
    void set(double x, double y, double z);

    Position operator+(const Position& other) const;
    Position operator-(const Position& other) const;
    Position operator*(double scalar) const;
    Position operator/(double scalar) const;

    Position operator-() const;

    Position& operator+=(const Position& other);
    Position& operator-=(const Position& other);
    Position& operator*=(double scalar);
    Position& operator/=(double scalar);

    double distance(const Position& other) const;
    double dot(const Position& other) const;
    Position cross(const Position& other) const;
    double norm() const;
    Position normalize() const;
};

#endif // POSITION_H

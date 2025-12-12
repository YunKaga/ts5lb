#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"

class Ball {
private:
    Point ballCenter;
    Velocity ballVelocity;
    bool IsCollidable;
    double ballRadius;
    Color ballColor;
public:
    Ball(Point center, Velocity velocity, bool collision, double radius, Color color );
    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool getCollidable() const;
};

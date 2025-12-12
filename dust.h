#pragma once

#include"Velocity.h"
#include"Point.h"
#include"Color.h"
#include"Painter.h"

class Dust{
    private:
        Point DustCenter;
        Velocity DustVelocity;
        double DustRadius;
        double DustTimer;
        Color DustColor;
    public:
        Dust(Point center, Velocity velocity, double radius, Color color, double timer);
        void update(double tick);
        bool rDead() const;
        void setCenter(Point& center);
        void draw(Painter& painter) const;
    };

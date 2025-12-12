#include"dust.h"

Dust::Dust(Point center, Velocity velocity, double radius, Color color, double timer){
    DustVelocity=velocity;
    DustCenter=center;
    DustRadius=radius;
    DustColor=color;
    DustTimer=timer;
}

void Dust::update(double tick){
    DustTimer=DustTimer-tick*10.;
    DustCenter = DustCenter + DustVelocity.vector() * tick;
}
void Dust::setCenter(Point& center){
    DustCenter=center;
}
bool Dust::rDead() const{
    return DustTimer<=0.0;
}
void Dust::draw(Painter& painter) const{
    painter.draw(DustCenter,DustRadius,DustColor);
}

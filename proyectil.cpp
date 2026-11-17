#include "Proyectil.h"
#include <cmath>

Proyectil::Proyectil(double x, double y, double vx, double vy, double m, double r)
    : x(x), y(y), vx(vx), vy(vy), m(m), r(r) {}

double Proyectil::getX() const { return x; }
double Proyectil::getY() const { return y; }
double Proyectil::getR() const { return r; }
double Proyectil::getMasa() const { return m; }
double Proyectil::rapidez() const { return std::sqrt(vx * vx + vy * vy); }

void Proyectil::mover(double dt, double g) {
    vy -= g * dt;
    x += vx * dt;
    y += vy * dt;
}

bool Proyectil::rebotarParedes(double ancho, double alto) {
    bool choco = false;
    if (x - r < 0 && vx < 0)           { x = r;          vx = -vx; choco = true; }
    else if (x + r > ancho && vx > 0)  { x = ancho - r;  vx = -vx; choco = true; }
    if (y - r < 0 && vy < 0)           { y = r;          vy = -vy; choco = true; }
    else if (y + r > alto && vy > 0)   { y = alto - r;   vy = -vy; choco = true; }
    return choco;
}
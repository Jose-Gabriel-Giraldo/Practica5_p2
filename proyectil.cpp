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

// (ox, oy) es la esquina inferior izquierda; devuelve la rapidez de impacto (0 si no hubo choque)
double Proyectil::chocarRectangulo(double ox, double oy, double ancho, double alto, double e) {
    double cx = x < ox ? ox : (x > ox + ancho ? ox + ancho : x);
    double cy = y < oy ? oy : (y > oy + alto ? oy + alto : y);
    double dx = x - cx, dy = y - cy;
    double d2 = dx * dx + dy * dy;
    if (d2 >= r * r) return 0;

    double nx, ny;
    if (d2 > 0) {
        double d = std::sqrt(d2);
        nx = dx / d;
        ny = dy / d;
    } else {
        double izq = x - ox, der = ox + ancho - x, inf = y - oy, sup = oy + alto - y;
        double minX = izq < der ? izq : der, minY = inf < sup ? inf : sup;
        if (minX < minY) { nx = izq < der ? -1 : 1; ny = 0; cx = izq < der ? ox : ox + ancho; }
        else             { nx = 0; ny = inf < sup ? -1 : 1; cy = inf < sup ? oy : oy + alto; }
    }

    x = cx + nx * r;
    y = cy + ny * r;

    double vn = vx * nx + vy * ny;
    if (vn >= 0) return 0;
    double vImpacto = rapidez();
    vx -= (1 + e) * vn * nx;
    vy -= (1 + e) * vn * ny;
    return vImpacto;
}

bool Proyectil::tocaRectangulo(double ox, double oy, double ancho, double alto) const {
    double cx = x < ox ? ox : (x > ox + ancho ? ox + ancho : x);
    double cy = y < oy ? oy : (y > oy + alto ? oy + alto : y);
    double dx = x - cx, dy = y - cy;
    return dx * dx + dy * dy < r * r;
}
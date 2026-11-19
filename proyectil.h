#ifndef PROYECTIL_H
#define PROYECTIL_H

class Proyectil {
private:
    double x, y, vx, vy, m, r;

public:
    Proyectil(double x, double y, double vx, double vy, double m, double r);

    double getX() const;
    double getY() const;
    double getR() const;
    double getMasa() const;
    double rapidez() const;

    void mover(double dt, double g);
    bool rebotarParedes(double ancho, double alto);
    double chocarRectangulo(double ox, double oy, double ancho, double alto, double e);
    bool tocaRectangulo(double ox, double oy, double ancho, double alto) const;
};

#endif
#include "Obstaculo.h"

Obstaculo::Obstaculo(double x, double y, double ancho, double alto, double resistencia, int dueno)
    : x(x), y(y), ancho(ancho), alto(alto), resistencia(resistencia), dueno(dueno) {}

double Obstaculo::getX() const { return x; }
double Obstaculo::getY() const { return y; }
double Obstaculo::getAncho() const { return ancho; }
double Obstaculo::getAlto() const { return alto; }
double Obstaculo::getResistencia() const { return resistencia; }
int Obstaculo::getDueno() const { return dueno; }
bool Obstaculo::destruido() const { return resistencia <= 0; }

void Obstaculo::recibirDanio(double danio) {
    resistencia -= danio;
    if (resistencia < 0) resistencia = 0;
}

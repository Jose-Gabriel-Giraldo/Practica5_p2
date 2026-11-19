#ifndef JUEGO_H
#define JUEGO_H

#include <array>
#include <vector>
#include "Proyectil.h"
#include "Obstaculo.h"

class Juego {
private:
    double ancho, alto, tVuelo, ultimoDanio;
    int turno, ganador;
    bool enVuelo;
    Proyectil proyectil;
    std::vector<Obstaculo> obstaculos;
    std::array<std::array<double, 4>, 2> rivales;   // {x, y, ancho, alto}
    std::array<std::array<double, 2>, 2> canones;   // {x, y}

public:
    Juego();

    void reiniciar();
    bool disparar(double angulo, double velocidad);
    void paso(double dt);

    double getAncho() const;
    double getAlto() const;
    int getTurno() const;
    int getGanador() const;
    bool estaEnVuelo() const;
    double getUltimoDanio() const;
    const Proyectil& getProyectil() const;
    const std::vector<Obstaculo>& getObstaculos() const;
    const std::array<double, 4>& getRival(int jugador) const;
    const std::array<double, 2>& getCanon(int jugador) const;
};

#endif

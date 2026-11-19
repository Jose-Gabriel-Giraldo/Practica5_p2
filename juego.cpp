#include "Juego.h"
#include <cmath>

const double ANCHO = 100, ALTO = 50;          // metros
const double G = 9.8;
const double RESTITUCION = 0.6;
const double FACTOR_DANIO = 1.0;              // Daño = FACTOR_DANIO * masa * |v_impacto|
const double MASA = 2.0, RADIO = 0.6;
const double T_MAX_VUELO = 8.0;               // segundos por disparo
const double PI = 3.14159265358979;

Juego::Juego()
    : ancho(ANCHO), alto(ALTO), tVuelo(0), ultimoDanio(0), turno(0), ganador(-1),
    enVuelo(false), proyectil(0, 0, 0, 0, MASA, RADIO)
{
    canones = {{{3, 24}, {ANCHO - 3, 24}}};
    rivales = {{{17.5, 0, 3, 6}, {ANCHO - 20.5, 0, 3, 6}}};
    reiniciar();
}

void Juego::reiniciar() {
    obstaculos.clear();
    for (int j = 0; j < 2; ++j) {
        double base = j == 0 ? 10 : ANCHO - 28;
        obstaculos.push_back(Obstaculo(base,      0,  5, 10, 200, j));
        obstaculos.push_back(Obstaculo(base + 13, 0,  5, 10, 200, j));
        obstaculos.push_back(Obstaculo(base,      10, 18, 3, 100, j));
    }
    turno = 0;
    ganador = -1;
    enVuelo = false;
    tVuelo = 0;
    ultimoDanio = 0;
}

bool Juego::disparar(double angulo, double velocidad) {
    if (enVuelo || ganador >= 0) return false;
    double a = angulo * PI / 180;
    double dir = turno == 0 ? 1 : -1;
    proyectil = Proyectil(canones[turno][0], canones[turno][1],
                          dir * velocidad * std::cos(a), velocidad * std::sin(a), MASA, RADIO);
    enVuelo = true;
    tVuelo = 0;
    ultimoDanio = 0;
    return true;
}

void Juego::paso(double dt) {
    if (!enVuelo) return;
    tVuelo += dt;

    proyectil.mover(dt, G);
    proyectil.rebotarParedes(ancho, alto);

    for (Obstaculo& o : obstaculos) {
        if (o.destruido()) continue;
        double v = proyectil.chocarRectangulo(o.getX(), o.getY(), o.getAncho(), o.getAlto(), RESTITUCION);
        if (v > 0 && o.getDueno() != turno) {
            double danio = FACTOR_DANIO * proyectil.getMasa() * v;
            o.recibirDanio(danio);
            ultimoDanio += danio;
        }
    }

    const std::array<double, 4>& rival = rivales[1 - turno];
    if (proyectil.tocaRectangulo(rival[0], rival[1], rival[2], rival[3])) {
        ganador = turno;
        enVuelo = false;
        return;
    }

    if (tVuelo >= T_MAX_VUELO) {
        enVuelo = false;
        turno = 1 - turno;
    }
}

double Juego::getAncho() const { return ancho; }
double Juego::getAlto() const { return alto; }
int Juego::getTurno() const { return turno; }
int Juego::getGanador() const { return ganador; }
bool Juego::estaEnVuelo() const { return enVuelo; }
double Juego::getUltimoDanio() const { return ultimoDanio; }
const Proyectil& Juego::getProyectil() const { return proyectil; }
const std::vector<Obstaculo>& Juego::getObstaculos() const { return obstaculos; }
const std::array<double, 4>& Juego::getRival(int jugador) const { return rivales[jugador]; }
const std::array<double, 2>& Juego::getCanon(int jugador) const { return canones[jugador]; }

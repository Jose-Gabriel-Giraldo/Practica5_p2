#ifndef OBSTACULO_H
#define OBSTACULO_H

class Obstaculo {
private:
    double x, y, ancho, alto, resistencia;
    int dueno;

public:
    Obstaculo(double x, double y, double ancho, double alto, double resistencia, int dueno);

    double getX() const;
    double getY() const;
    double getAncho() const;
    double getAlto() const;
    double getResistencia() const;
    int getDueno() const;
    bool destruido() const;

    void recibirDanio(double danio);
};

#endif

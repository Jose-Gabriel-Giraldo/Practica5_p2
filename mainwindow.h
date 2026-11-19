#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <array>
#include <vector>
#include "Juego.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void disparar();
    void reiniciar();
    void actualizar();
    void refrescar();

private:
    QPointF aEscena(double x, double y) const;

    Ui::MainWindow *ui;
    QGraphicsScene *escena;
    QTimer *timer;
    QSpinBox *spinAngulo;
    QSpinBox *spinVelocidad;
    QPushButton *botonDisparar;
    QLabel *estado;
    Juego juego;
    QGraphicsEllipseItem *bala;
    QGraphicsPathItem *rastro;
    QPainterPath camino;
    std::array<QGraphicsLineItem*, 2> canones;
    std::vector<QGraphicsRectItem*> rectangulos;
    std::vector<QGraphicsSimpleTextItem*> textos;
};

#endif

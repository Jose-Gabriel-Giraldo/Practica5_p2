#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <cmath>

const double ESCALA = 10;           // pixeles por metro
const double DT = 0.005;
const int INTERVALO_MS = 15;
const int PASOS_POR_FRAME = 3;      // 3 * 5 ms = 15 ms -> tiempo real
const double LARGO_CANON = 3;
const double PI = 3.14159265358979;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    escena = new QGraphicsScene(0, 0, juego.getAncho() * ESCALA, juego.getAlto() * ESCALA, this);
    QGraphicsView *vista = new QGraphicsView(escena);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setFocusPolicy(Qt::NoFocus);
    escena->addRect(escena->sceneRect(), QPen(Qt::black, 2));

    for (const Obstaculo& o : juego.getObstaculos()) {
        QPointF p = aEscena(o.getX(), o.getY() + o.getAlto());
        QColor color = o.getAlto() > o.getAncho() ? QColor(255, 228, 196) : QColor(235, 235, 235);
        rectangulos.push_back(escena->addRect(p.x(), p.y(), o.getAncho() * ESCALA, o.getAlto() * ESCALA,
                                              QPen(Qt::darkGray), QBrush(color)));
        textos.push_back(escena->addSimpleText(""));
    }

    for (int j = 0; j < 2; ++j) {
        const std::array<double, 4>& r = juego.getRival(j);
        double cx = (r[0] + r[2] / 2) * ESCALA, pie = (juego.getAlto() - r[1]) * ESCALA;
        double h = r[3] * ESCALA;
        QPainterPath monigote;
        monigote.addEllipse(QPointF(cx, pie - h * 0.85), h * 0.15, h * 0.15);
        monigote.moveTo(cx, pie - h * 0.7);  monigote.lineTo(cx, pie - h * 0.35);
        monigote.moveTo(cx - h * 0.2, pie - h * 0.55); monigote.lineTo(cx + h * 0.2, pie - h * 0.55);
        monigote.moveTo(cx, pie - h * 0.35); monigote.lineTo(cx - h * 0.18, pie);
        monigote.moveTo(cx, pie - h * 0.35); monigote.lineTo(cx + h * 0.18, pie);
        escena->addPath(monigote, QPen(Qt::black, 2));

        const std::array<double, 2>& c = juego.getCanon(j);
        QPointF base = aEscena(c[0], c[1]);
        escena->addRect(base.x() - 25, base.y() + 8, 50, 6, QPen(Qt::black), QBrush(Qt::white));
        canones[j] = escena->addLine(0, 0, 0, 0, QPen(QColor(30, 60, 120), 6, Qt::SolidLine, Qt::RoundCap));
    }

    rastro = escena->addPath(QPainterPath(), QPen(Qt::red, 1, Qt::DashLine));
    double rb = juego.getProyectil().getR() * ESCALA;
    bala = escena->addEllipse(-rb, -rb, 2 * rb, 2 * rb, QPen(Qt::NoPen), QBrush(Qt::red));
    bala->hide();

    spinAngulo = new QSpinBox;
    spinAngulo->setRange(0, 90);
    spinAngulo->setValue(45);
    spinAngulo->setFocusPolicy(Qt::NoFocus);
    spinVelocidad = new QSpinBox;
    spinVelocidad->setRange(5, 45);
    spinVelocidad->setValue(30);
    spinVelocidad->setFocusPolicy(Qt::NoFocus);
    botonDisparar = new QPushButton("Disparar");
    botonDisparar->setFocusPolicy(Qt::NoFocus);
    QPushButton *botonReiniciar = new QPushButton("Reiniciar");
    botonReiniciar->setFocusPolicy(Qt::NoFocus);
    estado = new QLabel;

    QHBoxLayout *controles = new QHBoxLayout;
    controles->addWidget(new QLabel("Ángulo (°):"));
    controles->addWidget(spinAngulo);
    controles->addWidget(new QLabel("Velocidad (m/s):"));
    controles->addWidget(spinVelocidad);
    controles->addWidget(botonDisparar);
    controles->addWidget(botonReiniciar);
    controles->addStretch();
    controles->addWidget(new QLabel("↑↓ ángulo   ←→ velocidad   Espacio dispara"));

    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(estado);
    layout->addWidget(vista);
    layout->addLayout(controles);
    setCentralWidget(central);
    setWindowTitle("Juego de estrategia por turnos");
    setFocusPolicy(Qt::StrongFocus);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);
    connect(botonDisparar, &QPushButton::clicked, this, &MainWindow::disparar);
    connect(botonReiniciar, &QPushButton::clicked, this, &MainWindow::reiniciar);
    connect(spinAngulo, &QSpinBox::valueChanged, this, &MainWindow::refrescar);

    refrescar();
    setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPointF MainWindow::aEscena(double x, double y) const
{
    return QPointF(x * ESCALA, (juego.getAlto() - y) * ESCALA);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:    spinAngulo->stepUp();      break;
    case Qt::Key_Down:  spinAngulo->stepDown();    break;
    case Qt::Key_Right: spinVelocidad->stepUp();   break;
    case Qt::Key_Left:  spinVelocidad->stepDown(); break;
    case Qt::Key_Space: disparar();                break;
    default: QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::disparar()
{
    if (!juego.disparar(spinAngulo->value(), spinVelocidad->value())) return;
    const Proyectil& p = juego.getProyectil();
    camino = QPainterPath(aEscena(p.getX(), p.getY()));
    rastro->setPath(camino);
    botonDisparar->setEnabled(false);
    timer->start(INTERVALO_MS);
    refrescar();
}

void MainWindow::reiniciar()
{
    timer->stop();
    juego.reiniciar();
    camino = QPainterPath();
    rastro->setPath(camino);
    botonDisparar->setEnabled(true);
    refrescar();
}

void MainWindow::actualizar()
{
    for (int k = 0; k < PASOS_POR_FRAME && juego.estaEnVuelo(); ++k) {
        juego.paso(DT);
        const Proyectil& p = juego.getProyectil();
        camino.lineTo(aEscena(p.getX(), p.getY()));
    }
    rastro->setPath(camino);

    if (!juego.estaEnVuelo()) {
        timer->stop();
        botonDisparar->setEnabled(juego.getGanador() < 0);
    }
    refrescar();
}

void MainWindow::refrescar()
{
    const std::vector<Obstaculo>& obs = juego.getObstaculos();
    for (std::size_t i = 0; i < obs.size(); ++i) {
        bool visible = !obs[i].destruido();
        rectangulos[i]->setVisible(visible);
        textos[i]->setVisible(visible);
        textos[i]->setText(QString::number(std::ceil(obs[i].getResistencia())));
        QPointF centro = aEscena(obs[i].getX() + obs[i].getAncho() / 2, obs[i].getY() + obs[i].getAlto() / 2);
        QRectF caja = textos[i]->boundingRect();
        textos[i]->setPos(centro.x() - caja.width() / 2, centro.y() - caja.height() / 2);
    }

    for (int j = 0; j < 2; ++j) {
        if (j == juego.getTurno() || canones[j]->line().isNull()) {
            double a = spinAngulo->value() * PI / 180, dir = j == 0 ? 1 : -1;
            const std::array<double, 2>& c = juego.getCanon(j);
            canones[j]->setLine(QLineF(aEscena(c[0], c[1]),
                                       aEscena(c[0] + dir * LARGO_CANON * std::cos(a), c[1] + LARGO_CANON * std::sin(a))));
        }
    }

    const Proyectil& p = juego.getProyectil();
    bala->setVisible(juego.estaEnVuelo());
    bala->setPos(aEscena(p.getX(), p.getY()));

    QString texto;
    if (juego.getGanador() >= 0)
        texto = QString("¡Gana el Jugador %1! Presiona Reiniciar para jugar otra vez.").arg(juego.getGanador() + 1);
    else if (juego.estaEnVuelo())
        texto = QString("Jugador %1 disparó…").arg(juego.getTurno() + 1);
    else
        texto = QString("Turno del Jugador %1").arg(juego.getTurno() + 1);
    if (juego.getUltimoDanio() > 0)
        texto += QString("   |   Daño del último disparo: %1").arg(juego.getUltimoDanio(), 0, 'f', 1);
    estado->setText(texto);
}

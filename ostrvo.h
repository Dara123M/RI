#ifndef OSTRVO_H
#define OSTRVO_H
#include<QGraphicsScene>
#include <QList>
#include <QColor>
#include <QThread>
#include <QTimer>
#include <QMetaType>
#include <cmath>
#include <QtAlgorithms>
#include <QtNumeric>
#include <QDebug>
#include "nit.h"
#include "mouse.h"

typedef Mouse* Jedinka;

typedef QList<Jedinka> MouseList;

class Nit;

class Ostrvo : public QGraphicsScene
{
    Q_OBJECT
public:

    Ostrvo(qreal x, qreal y, qreal x1, qreal y1, QColor color);

    void addItem(Mouse* item);

    QColor getColor();

    Nit* nit;

    ~Ostrvo();

    void dodajKomsije(Ostrvo* onajKomeSaljemo);

    void primiMigranta(Jedinka m);

    bool daLiZelimDaPrihvatimMigranta(Jedinka migrant);

signals:

    void zavrsi();

public slots:

    void NitJeZavrsila();

    void startEvolving();

    void hvatamoNovuGeneraciju(MouseList nova);

    void menjamo_generacije();

private:

    MouseList mouse_list;

    MouseList nova_generacija;

    MouseList temp, temp_1;

    QColor _color;

    int id_;

    QTimer timer_;

    Ostrvo* komsija;
};

#endif // OSTRVO_H

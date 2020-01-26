#ifndef NIT_H
#define NIT_H

#include <QThread>
#include <QList>
#include "mouse.h"
#include <random>
#include <QMetaType>
#include "ostrvo.h"
#include <math.h>
#include <QDebug>
#include <experimental/random>
#include <limits>
class Ostrvo;
typedef Mouse* Jedinka;
typedef QList<Jedinka> MouseList;
class Nit : public QThread
{
    Q_OBJECT
public:
    Nit(MouseList generation, MouseList nova_generacija, QColor islandColor);

    float izracunajFitness() const;

    void selekcija();

    int turnir();

    void ukrstanje(Mouse* jedan, Mouse* drugi);

    void mutacija();

    qreal generisiRandom(double lower, double upper);

signals:

    void saljemoNovuGeneraciju(MouseList nova);

    void zavrsioSam();

protected:

    void run();

private:
    MouseList population;
    MouseList new_population;
    QColor islandColor_;
    qreal mutation_rate;
    int iteracija;

};

#endif // NIT_H

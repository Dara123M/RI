#ifndef MOUSE_H
#define MOUSE_H

#include <QGraphicsItem>

#include <QGraphicsScene>

#include <QPainter>

#include <QStyleOption>

#include <fstream>

#include <cmath>

#include "genotype.h"

class Mouse : public QGraphicsItem
{
public:

    Mouse(QColor color, QString input);


    QRectF boundingRect() const override;

    QPainterPath shape() const override;


    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

    void changeAcordingToNeighbours(qreal fit_min, qreal fit_max);

    void changeColor(QColor color);

    QColor getColor();

    QColor getParentColor();

    qreal getFitness();

    void setFitnes(qreal fitnes);

    void IsSelectedForCrossover(bool option);

    void initialize(QString input);

    void evaluate();

    Genotype getGenotype() const;

    void setGenotype(Genotype g);

    void go_away();

    qreal za_sortiranje();
protected:

    void advance(int step) override;

private:
    qreal angle_;
    qreal speed_;
    qreal mouseEyeDirection_;
    Genotype genotype;
    QColor color_, parentColor;

    bool option_;
};

#endif // MOUSE_H

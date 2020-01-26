#include "mouse.h"


static const double PI = 3.14159265358979323846264338327950288419717;
static const double TWO_PI = 2.0 * PI;


static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
    {
        angle += TWO_PI;
    }

    while (angle > TWO_PI)
    {
        angle -= TWO_PI;
    }

    return angle;
}

Mouse::Mouse(QColor color, QString input)
  : angle_(0.0)
  , speed_(0.0)
  , mouseEyeDirection_(0.0)
  , color_(color)
  , parentColor(color)
  , option_(false)
{
    initialize(input);
    evaluate();
    setRotation(qrand() % (360 * 16));
}


QRectF Mouse::boundingRect() const
{
    qreal adjust = -0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}


QPainterPath Mouse::shape() const
{
    QPainterPath path;

    path.addRect(-10, -20, 20, 40);
    return path;
}

void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option)


    painter->setBrush(color_);
    painter->drawEllipse(-10, -20, 20, 40);


    const auto details =
            QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
    if (details >= .75)
    {
        painter->setBrush(Qt::white);
        painter->drawEllipse(-10, -17, 8, 8);
        painter->drawEllipse(2, -17, 8, 8);

        painter->setBrush(Qt::black);
        painter->drawEllipse(QRectF(-2, -22, 4, 4));

        painter->drawEllipse(QRectF(-8.0 + mouseEyeDirection_, -17, 4, 4));
        painter->drawEllipse(QRectF(4.0 + mouseEyeDirection_, -17, 4, 4));

       const auto earsColor =
                scene()->collidingItems(this).isEmpty() ? Qt::darkYellow : Qt::red;
        painter->setBrush(earsColor);
        painter->drawEllipse(-17, -12, 16, 16);
        painter->drawEllipse(1, -12, 16, 16);

        QPainterPath path(QPointF(0, 20));
        path.cubicTo(-5, 22, -5, 22, 0, 25);
        path.cubicTo(5, 27, 5, 32, 0, 30);
        path.cubicTo(-5, 32, -5, 42, 0, 35);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path);
    }
}


void Mouse::advance(int step)
{
    if (!step)
    {
        return;
    }


    if(option_){
        const QLineF lineToCenter(QPointF(0, 0), mapFromScene(-500, -500));
        if (lineToCenter.length() > 100)
        {
            qreal angleToCenter = acos(lineToCenter.dx() / lineToCenter.length());
            if (lineToCenter.dy() < 0)
            {
                angleToCenter = TWO_PI - angleToCenter;
            }
            angleToCenter = normalizeAngle((PI - angleToCenter) + PI / 2);

            if (angleToCenter < PI && angleToCenter > PI / 4)
            {
                // Rotiramo na levo
                angle_ += (angle_ < -PI / 2) ? 0.25 : -0.25;
            }
            else if (angleToCenter >= PI && angleToCenter < (PI + PI / 2 + PI / 4))
            {
                // Rotiramo na desno
                angle_ += (angle_ < PI / 2) ? 0.25 : -0.25;
            }
        }
        const QPolygonF mouseSight({ mapToScene(0, 0), mapToScene(-30, -50), mapToScene(30, -50)});
        const auto dangerMice = scene()->items(mouseSight);

        for (auto item: dangerMice)
        {
            if (item == this)
            {
                continue;
            }

            QLineF lineToMouse(QPointF(0, 0), mapFromItem(item, 0, 0));

            qreal angleToMouse = acos(lineToMouse.dx() / lineToMouse.length());
            if (lineToMouse.dy() < 0)
            {
                angleToMouse = TWO_PI - angleToMouse;
            }
            angleToMouse = normalizeAngle((PI - angleToMouse) + PI / 2);

            if (angleToMouse >= 0 && angleToMouse < PI / 2)
            {
                // Rotiramo na levo
                angle_ += 0.5;
            }
            else if (angleToMouse <= TWO_PI && angleToMouse > (TWO_PI - PI / 2))
            {
                // Rotiramo na desno
                angle_ -= 0.5;
            }
        }

        // Haoticno kretanje
        if (dangerMice.size() > 1 && (qrand() % 10) == 0)
        {
            if (qrand() % 1)
            {
                angle_ += (qrand() % 100) / 500.0;
            }
            else
            {
                angle_ -= (qrand() % 100) / 500.0;
            }
        }

        speed_ += (-50 + qrand() % 100) / 100.0;

        const qreal dx = sin(angle_) * 10;
        mouseEyeDirection_ = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

        setRotation(rotation() + dx);
        setPos(mapToParent(0, -(3 + sin(speed_) * 5)));
        return;
    }
    // Linija od centra misa do centra scene
    const QLineF lineToCenter(QPointF(0, 0), mapFromScene(0, 0));

    // Ako se previse odaljavamo od centra,
    // bolje da se vracamo nazad.
    if (lineToCenter.length() > 250)
    {
        qreal angleToCenter = acos(lineToCenter.dx() / lineToCenter.length());
        if (lineToCenter.dy() < 0)
        {
            angleToCenter = TWO_PI - angleToCenter;
        }
        angleToCenter = normalizeAngle((PI - angleToCenter) + PI / 2);

        if (angleToCenter < PI && angleToCenter > PI / 4)
        {
            // Rotiramo na levo
            angle_ += (angle_ < -PI / 2) ? 0.25 : -0.25;
        }
        else if (angleToCenter >= PI && angleToCenter < (PI + PI / 2 + PI / 4))
        {
            // Rotiramo na desno
            angle_ += (angle_ < PI / 2) ? 0.25 : -0.25;
        }
    }
    else if (sin(angle_) < 0)
    {
        angle_ += 0.25;
    }
    else if (sin(angle_) > 0)
    {
        angle_ -= 0.25;
    }

    // Trazimo miseve koji su nam u vidokrugu
    const QPolygonF mouseSight({ mapToScene(0, 0), mapToScene(-30, -50), mapToScene(30, -50)});
    const auto dangerMice = scene()->items(mouseSight);

    for (auto item: dangerMice)
    {
        if (item == this)
        {
            continue;
        }

        QLineF lineToMouse(QPointF(0, 0), mapFromItem(item, 0, 0));

        qreal angleToMouse = acos(lineToMouse.dx() / lineToMouse.length());
        if (lineToMouse.dy() < 0)
        {
            angleToMouse = TWO_PI - angleToMouse;
        }
        angleToMouse = normalizeAngle((PI - angleToMouse) + PI / 2);

        if (angleToMouse >= 0 && angleToMouse < PI / 2)
        {
            // Rotiramo na levo
            angle_ += 0.5;
        }
        else if (angleToMouse <= TWO_PI && angleToMouse > (TWO_PI - PI / 2))
        {
            // Rotiramo na desno
            angle_ -= 0.5;
        }
    }

    // Haoticno kretanje
    if (dangerMice.size() > 1 && (qrand() % 10) == 0)
    {
        if (qrand() % 1)
        {
            angle_ += (qrand() % 100) / 500.0;
        }
        else
        {
            angle_ -= (qrand() % 100) / 500.0;
        }
    }

    speed_ += (-50 + qrand() % 100) / 100.0;

    const qreal dx = sin(angle_) * 10;
    mouseEyeDirection_ = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

    setRotation(rotation() + dx);
    setPos(mapToParent(0, -(3 + sin(speed_) * 3)));
}

void Mouse::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    Q_UNUSED(event)

    delete this;
}

void Mouse::IsSelectedForCrossover(bool option){
    option_=option;
}

void Mouse::setFitnes(qreal fitnes){
    genotype.fitnes=fitnes;
}

void Mouse::changeColor(QColor color){
    this->color_=color;
    update();
}

qreal Mouse::getFitness(){
    return genotype.fitnes;
}

void Mouse::initialize(QString input){
    genotype.fitnes=0;
    std::ifstream fajl(input.toStdString());
    for(int i=0; i<3; i++){
        qreal min, max;
        fajl>>min>>max;
        genotype.lower.push_back(min);
        genotype.upper.push_back(max);
        qreal p=min+(max-min)*(qrand()%1000)/10000.0;
        if(p<=min)
            p=min;
        else if(p>=max)
            p=max;
        genotype.gene.push_back(p);
    }
    evaluate();
    fajl.close();
}

void Mouse::evaluate(){

    genotype.fitnes = abs(genotype.gene[0]*genotype.gene[0] - genotype.gene[0]*genotype.gene[1]+genotype.gene[2]);


}

void Mouse::setGenotype(Genotype g)
{
    genotype=g;
    evaluate();
}

Genotype Mouse::getGenotype() const{
    return genotype;
}

QColor Mouse::getColor(){
    return color_;
}
QColor Mouse::getParentColor(){
    return parentColor;
}

void Mouse::go_away(){
    color_=Qt::darkGray;
    option_=true;
    this->update();
}


qreal Mouse::za_sortiranje(){
    return 1.0/(1+getFitness());
}

void Mouse::changeAcordingToNeighbours(qreal fit_min, qreal fit_max){
    qreal range = abs(fit_max-fit_min);
    int newRange = 255;

    int newVal= abs(abs(getFitness()-fit_min)/range)*newRange;
    if(getParentColor()==Qt::green)
        color_=QColor(0,newVal,0);
    else if(getParentColor()==Qt::yellow)
        color_=QColor(newVal,newVal,0);
    else if(getParentColor()==Qt::blue)
        color_=QColor(0,0,newVal);
}


#include <QtGui>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QColor>
#include <QVector>
#include <cmath>
#include "ostrvo.h"
#include "mouse.h"
static QVector<QGraphicsView*> views;
static QVector<Ostrvo*> scenes;
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qsrand(static_cast<unsigned>(QTime(0,0,0).secsTo(QTime::currentTime())));


    scenes.push_back(new Ostrvo(-300, -300, 600, 600, Qt::blue));
    scenes.push_back(new Ostrvo(-300, -300, 600, 600, Qt::yellow));
    scenes.push_back(new Ostrvo(-300, -300, 600, 600, Qt::green));

    scenes.at(0)->dodajKomsije(scenes.at(1));
    scenes.at(1)->dodajKomsije(scenes.at(2));
    scenes.at(2)->dodajKomsije(scenes.at(0));

    for(QGraphicsScene* scene: scenes)
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);


    views.push_back(new QGraphicsView(scenes[0]));
    views.push_back(new QGraphicsView(scenes[1]));
    views.push_back(new QGraphicsView(scenes[2]));



    for(QGraphicsView* view: views){
        view->setRenderHint(QPainter::Antialiasing);
        view->setDragMode(QGraphicsView::ScrollHandDrag);
        view->setWindowTitle("Ostrvo");
        view->resize(600,400);
        view->show();
    }

   views[0]->setBackgroundBrush(QPixmap("/home/dara/Desktop/Mice/images/rock.jpg"));
   views[1]->setBackgroundBrush(QPixmap("/home/dara/Desktop/Mice/images/sand.jpg"));
   views[2]->setBackgroundBrush(QPixmap("/home/dara/Desktop/Mice/images/index.jpeg"));


    QTimer timer;
    for(QGraphicsScene* scene: scenes){
        QObject::connect(&timer, SIGNAL(timeout()), scene, SLOT(advance()));
        QObject::connect(scene, SIGNAL(zavrsi()), &timer, SLOT(stop()));
    }

    QTimer timerForNewGeneration;
    for(QGraphicsScene* scene: scenes)
        QObject::connect(&timerForNewGeneration, SIGNAL(timeout()), scene, SLOT(startEvolving()));

    timer.start(1000 / 33);
    timerForNewGeneration.start(45000);


    return app.exec();
}


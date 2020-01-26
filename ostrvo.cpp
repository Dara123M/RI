#include "ostrvo.h"

Ostrvo::Ostrvo(qreal x, qreal y, qreal x1, qreal y1, QColor color): QGraphicsScene(x, y, x1, y1),_color(color), timer_(this)
{
    qRegisterMetaType<Jedinka>("Jedinka");
    qRegisterMetaType<MouseList>("MouseList");
    for (auto i = 0u; i < 50; ++i)
    {
        Mouse * mouse = new Mouse(this->getColor(), "/home/dara/Desktop/Mice/input.txt");
        mouse->setPos(sin((i * 6.28) / 10) * 200,
                      cos((i * 6.28) / 10) * 200);
        this->addItem(mouse);
    }
    nit= new Nit(mouse_list, nova_generacija, _color);

    std::sort(mouse_list.begin(), mouse_list.end(), [](Mouse* x, Mouse* y){ return x->za_sortiranje() < y->za_sortiranje();});
    for(auto m: mouse_list)
        m->changeAcordingToNeighbours(mouse_list.first()->getFitness(), mouse_list.last()->getFitness());
    QObject::connect(nit, &Nit::saljemoNovuGeneraciju, this, &Ostrvo::hvatamoNovuGeneraciju, Qt::QueuedConnection);
    QObject::connect(nit, &Nit::zavrsioSam, this, &Ostrvo::NitJeZavrsila, Qt::QueuedConnection);
}

void Ostrvo::addItem(Mouse *item){
    mouse_list.append(item);
    QGraphicsScene::addItem(item);
}

QColor Ostrvo::getColor(){
    return _color;
}

Ostrvo::~Ostrvo(){
    if(nit->isFinished()==false)
        nit->quit();
    for(auto m: mouse_list)
        delete m;
}

void Ostrvo::startEvolving(){
    nit->start();
}

void Ostrvo::hvatamoNovuGeneraciju(MouseList nova){
    nova_generacija=nova;
    temp=nova;
    temp_1=mouse_list;

    QObject::connect(&timer_, SIGNAL(timeout()), this, SLOT(menjamo_generacije()));
    timer_.start(1500);
}

void Ostrvo::menjamo_generacije(){

    if(mouse_list.size()==0 || nova_generacija.size()==0){
        timer_.stop();
        bool f=false;
        bool dozvoljeno;
        for(Mouse* m: temp_1){
            dozvoljeno=komsija->daLiZelimDaPrihvatimMigranta(m);
          if(qrand()%1000/1000.0<0.05 && !f && dozvoljeno){
              Jedinka mm = m;
              QGraphicsScene::removeItem(m);
              mm->changeColor(komsija->_color);
              komsija->primiMigranta(mm);
              f=true;
              continue;
          }
            QGraphicsScene::removeItem(m);
        }
        mouse_list.clear();
        mouse_list.swap(temp);


        temp_1.clear();
        temp.clear();
        nova_generacija.clear();



        return;
    }

    Mouse* m1= mouse_list.first();

    mouse_list.removeFirst();

    if(!mouse_list.isEmpty()){
        Mouse *m2= mouse_list.first();

        mouse_list.removeFirst();
        m1->go_away();
        m2->go_away();

        QGraphicsScene::addItem(nova_generacija.first());
        nova_generacija.pop_front();


        QGraphicsScene::addItem(nova_generacija.first());

        nova_generacija.pop_front();

    }else{
        m1->go_away();
        QGraphicsScene::addItem(nova_generacija.first());

        nova_generacija.pop_front();

    }



}

void Ostrvo::dodajKomsije(Ostrvo *onajKomeSaljemo){
    komsija = onajKomeSaljemo;
}

void Ostrvo::primiMigranta(Jedinka m){
    QGraphicsScene::removeItem(mouse_list.last());
    mouse_list.removeLast();
    this->addItem(m);
    std::sort(mouse_list.begin(), mouse_list.end(), [](Mouse* x, Mouse* y){ return x->za_sortiranje() < y->za_sortiranje();});
    for(auto m:mouse_list)
        m->changeAcordingToNeighbours(mouse_list.first()->getFitness(), mouse_list.last()->getFitness());
}

bool Ostrvo::daLiZelimDaPrihvatimMigranta(Jedinka migrant){
    qreal acc = std::accumulate(std::begin(mouse_list), std::end(mouse_list), 0.0, [](qreal &acc, Jedinka m){return acc+m->getFitness();})/mouse_list.size();
    return qrand()%1000/1000.0 < 0.05 || migrant->getFitness()>acc;
}

void Ostrvo::NitJeZavrsila(){
    nit->quit();
    emit zavrsi();
}

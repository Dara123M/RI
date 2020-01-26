#include "nit.h"

Nit::Nit(MouseList generation, MouseList nova_generacija, QColor islandColor)
    :population(generation)
    ,new_population(nova_generacija)
    , islandColor_(islandColor)
{
    mutation_rate=0.05;
    iteracija=1;
    qRegisterMetaType<Jedinka>("Jedinka");
    qRegisterMetaType<MouseList>("MouseList");
}

void Nit::run(){
    if(iteracija==1000){
        emit zavrsioSam();
        return;
    }
    selekcija();

    if(islandColor_==Qt::blue)
        qDebug()<<"najbolji u generaciji "+QString::number(iteracija) + " na plavom ostrvu je "+QString::number(population.at(0)->getFitness());
    else if(islandColor_==Qt::yellow)
        qDebug()<<"najbolji u generaciji "+QString::number(iteracija) + " na zutom ostrvu je "+QString::number(population.at(0)->getFitness());
    else if(islandColor_==Qt::green)
        qDebug()<<"najbolji u generaciji "+QString::number(iteracija) + " na zelenom ostrvu je "+QString::number(population.at(0)->getFitness());

    iteracija++;

}
void Nit::selekcija()
{
    for(int i=0;i<6;i++){
        Mouse* m=new Mouse(population[0]->getColor(), "/home/dara/Desktop/Mice/input.txt");
        m->setGenotype(population[i]->getGenotype());
        new_population.append(m);
    }


    int i=6;
    while (i<population.size()) {
        int prvi=turnir();
        int drugi=turnir();
        ukrstanje(population[prvi], population[drugi]);
        i+=2;
    }

    mutacija();

    std::sort(std::begin(new_population), std::end(new_population),[](Mouse* x, Mouse* y){return x->za_sortiranje()<y->za_sortiranje();});
    qreal max_fit=new_population.first()->getFitness();
    qreal min_fit=new_population.last()->getFitness();

    for(auto m: new_population)
        m->changeAcordingToNeighbours(min_fit, max_fit);

    emit saljemoNovuGeneraciju(new_population);
    population.swap(new_population);
    new_population.clear();

}

int Nit::turnir()
{

    std::vector<int> indeksi(6);
    for(int i=0; i<6; i++)
        indeksi.push_back(std::experimental::randint(6, population.size()));
    int k=indeksi[0];
    for(int i=1; i<6; i++){
        if(population[i]->getFitness()>population[k]->getFitness())
            k=i;
    }

    return k;
}

void Nit::ukrstanje(Mouse* jedan, Mouse* drugi)
{
    Mouse* md1 =new Mouse(jedan->getColor(), "/home/dara/Desktop/Mice/input.txt");
    Mouse* md2=new Mouse (jedan->getColor(), "/home/dara/Desktop/Mice/input.txt");

    Genotype dete1=jedan->getGenotype();
    Genotype dete2=drugi->getGenotype();

    int point = std::experimental::randint(0, dete1.gene.size()-1);
    double t;
    for(int i=0; i<point; i++){
        t = dete1.gene[i];
        dete1.gene[i] = dete2.gene[i];
        dete2.gene[i] = t;
    }

    md1->setGenotype(dete1);
    md2->setGenotype(dete2);

    new_population.push_back(md1);
    new_population.push_back(md2);

}


void Nit::mutacija()
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0,1.0);
    for(auto jedinka: new_population){
        qreal x = generisiRandom(0, 1);
        if(x<mutation_rate){
             Genotype dete1=jedinka->getGenotype();
             int point = rand()%dete1.gene.length();
             dete1.gene[point] = generisiRandom(dete1.lower[point], dete1.upper[point]);
        }

    }
}



qreal Nit::generisiRandom(double lower, double upper)
{

    qreal p=lower+(upper-lower)*(qrand()%1000)/10000.0;
    if(p<=lower)
        p=lower;
    else if(p>=upper)
        p=upper;
    return p;
}



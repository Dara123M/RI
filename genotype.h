#ifndef GENOTYPE_H
#define GENOTYPE_H
#include <QVector>
//napravila sam ovu strukturu da bi mogla da nam cuva one informacije koje su nam bitne da izracunamo maksimum fje x^2 - x*y+ z
//ona ima vektor gene koja sadrzi x,y,z
// lower koja cuva donje granice za x,y,z
//upper koja cuva gornje granice za x,y,z
// jedinka se inicijalizuje kao l1<=x<=u1 , l2<=y<=u2, l3<=z<=u3
// ova 3 fitnesa jos nisam provalila sta su, obican fitnes i jos 2 dodatna koja se verovatno koriste u selekciji, mutaciji i ukrstanju.
struct Genotype{
    QVector<qreal> gene;
    QVector<qreal> lower;
    QVector<qreal> upper;
    qreal fitnes;
};
#endif // GENOTYPE_H

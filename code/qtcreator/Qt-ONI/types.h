#ifndef TYPES_H
#define TYPES_H

//
// Common includes
//
#include <QDebug>

//
// Type includes
//
#include <QVector>
#include <QRect>

//
// Application Types
//
typedef unsigned int        Count;
typedef unsigned long       Stamp;

typedef float               Sample;
typedef QVector<CvRect>     Result;

typedef QVector<Sample>     SampleVector;

#endif // TYPES_H

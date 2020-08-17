#ifndef UTILSOPERATION_H
#define UTILSOPERATION_H
#include <vector>
#include <QDebug>
#include <math.h>
class UtilsOperation
{
public:
    UtilsOperation();

    static void vectorCrossProd(std::vector<float> Point1,std::vector<float> Point2,std::vector<float> Point3,std::vector<float> &Result );
    static void vectorNormalize(std::vector<float> &Result );
};

#endif // UTILSOPERATION_H

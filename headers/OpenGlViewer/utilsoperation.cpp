#include "utilsoperation.h"

UtilsOperation::UtilsOperation()
{

}

void UtilsOperation::vectorNormalize(std::vector<float> &Result)
{
    float d=sqrt(Result[0]*Result[0]+Result[1]*Result[1]+Result[2]*Result[2]);      //находим длину вектора
    if(d==0.0)
    {
        qDebug()<<"Length ==0 vectorNormalize";
        return;
    }
    Result[0]/=d;
    Result[1]/=d;
    Result[2]/=d;
}

void UtilsOperation::vectorCrossProd(std::vector<float> Point1, std::vector<float> Point2, std::vector<float> Point3, std::vector<float> &Result)
{
    std::vector<float> vect1;
    vect1.push_back(Point2[0]-Point1[0]);
    vect1.push_back(Point2[1]-Point1[1]);
    vect1.push_back(Point2[2]-Point1[2]);

    std::vector<float> vect2;
    vect2.push_back(Point3[0]-Point1[0]);
    vect2.push_back(Point3[1]-Point1[1]);
    vect2.push_back(Point3[2]-Point1[2]);


    Result[0]=vect1[1]*vect2[2]- vect1[2]*vect2[1];
    Result[1]=vect1[2]*vect2[0]- vect1[0]*vect2[2];
    Result[2]=vect1[0]*vect2[1]- vect1[1]*vect2[0];

    vectorNormalize(Result);
}

#include "BlauImage.h"
const double Pi = 3.14159265358979323846;
void InvariantMoments(ByteImage& img, double m[7]);
bool check_obstacle(double incline);
double Absolute_Value(double incline);
double GetIncline(BlauPoint p1, BlauPoint p2);
void NMoments(BlauPoint Pnt[], double p[7], int num);
double GeometricMoment(BlauPoint Pnt[], int p, int q,int num);
void NInvariantMoments(BlauPoint Pnt[], double p[7],int num);
BlauPoint Cpoint(BlauPoint Pnt[], int size);

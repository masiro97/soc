#include <iostream>
#include <cmath>
#include <conio.h>
#include "BlauImage.h"

#define PI 3.14

class BlauPoint
{
public:
	int x, y;
	BlauPoint(int X=0,int Y=0)
	{
		x = X;
		y = Y;
	}
};

class BlauScalar
{
public:
	int B;
	int G;
	int R;

	int V;

	BlauScalar(int b, int g = 0, int r = 0)
	{
		B = b;
		G = g;
		R = r;
	}
	BlauScalar(int value)
	{
		V = value;
	}
};

void BlauCircle(ByteImage& img, BlauPoint center, int radius, BlauScalar Scalar) {

	int x1, y1, x2, y2, degree;
	double radian;

	for (degree = 0; degree <= 90;degree++) {

		radian = degree*PI/ 180;
		x1 = center.x + radius*cos(radian);
		y1 = center.y + radius*sin(radian);
		x2 = x1 - 2 * radius*cos(radian);
		y2 = y1 - 2 * radius*sin(radian);

		if (img.GetChannel() == 3) {
			img.GetAt(x1, y1, 0) = Scalar.B;
			img.GetAt(x1, y1, 0) = Scalar.G;
			img.GetAt(x1, y1, 0) = Scalar.R;

			img.GetAt(x1, y2, 0) = Scalar.B;
			img.GetAt(x1, y2, 0) = Scalar.G;
			img.GetAt(x1, y2, 0) = Scalar.R;

			img.GetAt(x2, y1, 0) = Scalar.B;
			img.GetAt(x2, y1, 0) = Scalar.G;
			img.GetAt(x2, y1, 0) = Scalar.R;

			img.GetAt(x2, y2, 0) = Scalar.B;
			img.GetAt(x2, y2, 0) = Scalar.G;
			img.GetAt(x2, y2, 0) = Scalar.R;
		}
		if (img.GetChannel() == 1) {
			img.GetAt(x1, y1, 0) = Scalar.V;
			img.GetAt(x1, y2, 0) = Scalar.V;
			img.GetAt(x2, y1, 0) = Scalar.V;
			img.GetAt(x2, y2, 0) = Scalar.V;
		}
	}
}


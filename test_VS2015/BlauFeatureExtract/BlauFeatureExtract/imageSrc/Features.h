#pragma once
#include "BlauImage.h"

#define M_PI		3.14159265358979323846

void DrawCross(ByteImage& image, int x, int y, BYTE color[3]);
void DrawLine(ByteImage& canvas, int x1, int y1, int x2, int y2, BYTE val);
void DrawLine(ByteImage& canvas, int x1, int y1, int x2, int y2, BYTE R, BYTE G, BYTE B);
void SobelEdge(const ByteImage& imageIn, ByteImage& imageOut);
void CannyEdge(const ByteImage& imageIn, ByteImage& imageOut, int nThresholdHi, int nThresholdLo);
int  HoughLines(const ByteImage& imageIn, int nTNum, int nTVal, double resTheta, int numLine, double* pRho, double* pTheta);
int  HarrisCorner(const ByteImage& imageIn,  double dThreshold, double paramK, int nMaxCorner, double* posX, double* posY);

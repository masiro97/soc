
#include <math.h>
#include <stdio.h>
// 블롭들의 중심점을 잇는다. -> 가로줄무늬의 기울기와 세로 줄무늬 기울기가 다른점을 이용

double GetIncline(BlauPoint p1, BlauPoint p2){
	double incline;
	double delta_x = p2.x - p1.x;
	double delta_y = p2.y - p1.y;
	incline = delta_y / delta_x;

	return incline;
}

double Absolute_Value(double incline){

	if (incline > 0) return incline;
	else return (-1 * incline);

}

//가로줄무늬일 경우 true, 세로 줄무늬일 경우 false

bool check_obstacle(double incline){
	
	double Abs_incline = Absolute_Value(incline);
	if (Abs_incline > 0.5) return true;
	else return false;
}

struct Point
{public:
	int xpos;
	int ypos;
	
};


Point Cpoint(BlauPoint Pnt[], int size){
	
	Point Cpoint(0, 0);

	int px = 0;
	int py = 0;

	for (int s = 0; s < size; s++){
		px += Pnt[s].x;
		py += Pnt[s].y;
	}
	
	int x = px / size;
	int y = py / size;

	Cpoint = (x, y);

	return Cpoint;


}

double GeometricMoment(Point Pnt[], int p, int q, int num){

	int size = sizeof(Pnt) / (sizeof(int) * 2);

	register int h, k;
	double moment = 0, xp, yq;

	for (int h = 0; h < num; h++){
		xp = yq = 1;
		for (k = 0; k < p; k++) xp *= Pnt[h].x;
		for (k = 0; k < q; k++) yq *= Pnt[h].y;
		moment += (xp * yq *255);

	}
	
		return moment;
}



void NInvariantMoments(Point Pnt[], double p[7], int num){

	double g00, g10, g01, g20, g02, g11, g30, g03, g21, g12;
	double c00, c20, c02, c11, c30, c03, c21, c12;
	double n20, n02, n11, n30, n03, n21, n12;

	//기하학적 모멘트 구하기

	g00 = GeometricMoment(Pnt, 0, 0,num);
	g10 = GeometricMoment(Pnt, 1, 0, num);
	g01 = GeometricMoment(Pnt, 0, 1, num);
	g20 = GeometricMoment(Pnt, 2, 0, num);
	g02 = GeometricMoment(Pnt, 0, 2, num);
	g11 = GeometricMoment(Pnt, 1, 1, num);
	g30 = GeometricMoment(Pnt, 3, 0, num);
	g03 = GeometricMoment(Pnt, 0, 3, num);
	g21 = GeometricMoment(Pnt, 2, 1, num);
	g12 = GeometricMoment(Pnt, 1, 2, num);

	//중심 모멘트 구하기

	double cx = g10 / g00;
	double cy = g01 / g00;

	c00 = g00;
	c20 = g20 = cx * g10;
	c02 = g02 = cy * g01;
	c11 = g11 - cx * g01;
	c30 = g30 - 3 * cx * g20 + 2 * cx * cx * g10;
	c03 = g03 - 3 * cy * g02 + 2 * cy * cy * g01;
	c21 = g21 - 2 * cx * g11 - cy * g20 + 2 * cx * cx * g01;
	c12 = g12 - 2 * cy * g11 - cx * g02 + 2 * cy * cy * g10;


	//정규화된 중심 모멘트

	n20 = c20 / pow(c00, 2.);
	n02 = c02 / pow(c00, 2.);
	n11 = c11 / pow(c00, 2.);
	n30 = c30 / pow(c00, 2.5);
	n03 = c03 / pow(c00, 2.5);
	n21 = c21 / pow(c00, 2.5);
	n12 = c12 / pow(c00, 2.5);

	//불변 모멘트 구하기

	p[0] = n20 + n02;
	p[1] = (n20 - n02) * (n20 - n02) + 4 * n11 *n11;
	p[2] = (n30 - 3 * n12) * (n30 - 3 * n12) + (3 * n21 - n03) * (3 * n21 - n03);
	p[3] = (n30 + n12)*(n30 + n12) + (n21 + n03) * (n21 + n03);
	p[4] = (n30 - 3 * n12) * (n30 + n12) *((n30 + n12) * (n30 + n12) - 3 * (n21 + n03) * (n21 + n03)) \
		+ (3 * n21 - n03) * (n21 + n03)*(3 * (n30 + n12) - (n21 + n03)*(n21 + n03));
	p[5] = (n20 - n02) * ((n30 + n12) * (n30 + n12) - (n21 + n03) * (n21 + n03)) + 4 * n11 * (n30 + n12) * (n21 + n03);
	p[6] = (3 * n21 - n03) * (n30 + n12) *((n30 + n12) * (n30 + n12) - 3 * (n21 + n03) * (n21 + n03)) \
		+ (3 * n12 - n30) * (n21 + n03) * (3 * (n30 + n12) * (n30 + n12) - (n21 + n03) *(n21 + n03));
}
void NMoments(Point Pnt[], double p[7], int num){

	double g00, g10, g01, g20, g02, g11, g30, g03, g21, g12;
	double c00, c20, c02, c11, c30, c03, c21, c12;
	double n20, n02, n11, n30, n03, n21, n12;

	//기하학적 모멘트 구하기

	g00 = GeometricMoment(Pnt, 0, 0,num);
	g10 = GeometricMoment(Pnt, 1, 0, num);
	g01 = GeometricMoment(Pnt, 0, 1, num);
	g20 = GeometricMoment(Pnt, 2, 0, num);
	g02 = GeometricMoment(Pnt, 0, 2, num);
	g11 = GeometricMoment(Pnt, 1, 1, num);
	g30 = GeometricMoment(Pnt, 3, 0, num);
	g03 = GeometricMoment(Pnt, 0, 3, num);
	g21 = GeometricMoment(Pnt, 2, 1, num);
	g12 = GeometricMoment(Pnt, 1, 2, num);

	//중심 모멘트 구하기

	double cx = g10 / g00;
	double cy = g01 / g00;

	c00 = g00;
	c20 = g20 = cx * g10;
	c02 = g02 = cy * g01;
	c11 = g11 - cx * g01;
	c30 = g30 - 3 * cx * g20 + 2 * cx * cx * g10;
	c03 = g03 - 3 * cy * g02 + 2 * cy * cy * g01;
	c21 = g21 - 2 * cx * g11 - cy * g20 + 2 * cx * cx * g01;
	c12 = g12 - 2 * cy * g11 - cx * g02 + 2 * cy * cy * g10;


	//정규화된 중심 모멘트


	p[0] = n20 = c20 / pow(c00, 2.);
	p[1] = n02 = c02 / pow(c00, 2.);
	p[2] = n11 = c11 / pow(c00, 2.);
	p[3] = n30 = c30 / pow(c00, 2.5);
	p[4] = n03 = c03 / pow(c00, 2.5);
	p[5] = n21 = c21 / pow(c00, 2.5);
	p[6] = n12 = c12 / pow(c00, 2.5);

}

//이미지의 겹쳐진 부분을 견본(Template)과 비교한다. -> blob 1과 blob2의 유사도를 비교

//cvMatchTemplate(CvArr* image, CvArr* template, CvArr* result, int method); -> 사용 미지수

bool Check_Similarity(int fir_Moment[], int sec_Moment[], int end){

	int Simirarity = 0;
	for (int k = 0; k < 7; k++){

		if (fir_Moment[k] * 0.98 <= sec_Moment[k] && fir_Moment[k] * 1.02 >= sec_Moment[k])
			Simirarity++;
	}

	if (Simirarity > 0) return true;
	else return false;
}


//const double Pi = 3.14159265358979323846;

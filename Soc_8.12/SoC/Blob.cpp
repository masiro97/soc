//2016_07_04

//Blob Filter(Pseudo Code)
#include "BlauImage.h"

#define max_blob_num 20
#define init_points 8
#define WIDTH 640
#define HEIGHT 480
//color
#define Black 0
#define Blue 1
#define Yellow 2
#define Green 3
#define Red 4
#define Orange 5

#include <iostream>

int nblob = 0;


class Blob{
public:
	int color;
	int nP = 0, Psize = init_points;
	BlauPoint* points;

	Blob(int clr, int x, int y)
	{
		color = clr;
		points = new BlauPoint[init_points];
		points[0].x = x; points[0].y = y;
		nP++;
	}
	void exSize()
	{
		BlauPoint* newpoints = new BlauPoint[2 * Psize];

		//copy(points, points + Psize - 1, newpoints);			//copyÇÔ¼ö??
		Psize *= 2;
		delete[] points;
		points = newpoints;
	}

	void addBlob(int x, int y, bool Delete)
	{
		if (nP == Psize) exSize();
		if (!Delete){
			points[nP].x = x; points[nP].y = y;
			nP++;
		}
		else{
			int i;
			for (i = nP; i >= 0; i--)
				if (points[i].x == x) break;

			points[i].x = x; points[i].y = y;
		}
	}

};


class Map{
public:
	int* color;
	bool* Isdeleted;
	int* blobed;

	Map(int cols, int rows)
	{
		color = new int[cols*rows];
		Isdeleted = new bool[cols*rows];
		blobed = new int[cols*rows];
	}
};

/*void Mapping(BYTE* pImg, Map* FtMap, int loc)
{
	BYTE B = pImg[loc];
	BYTE G = pImg[loc + 1];
	BYTE R = pImg[loc + 2];

	if (Black range) COLOR = Black;
	else if (Blue range) COLOR = Blue;
	else if (Yellow range) COLOR = Yellow;
	else if (Green) COLOR = Green;
	else if (Red) COLOR = Red;
	else if (Orange) COLOR = Orange;
	else COLOR = 999;

	color[loc] = COLOR;
	Isdeleted[loc] = false;
	blobed[loc] = 999;
}*/

bool checkMap(Map* FtMap, Blob* blobs, int x, int y, int u, int v)
{
	int* clr = FtMap->color;
	int* blb = FtMap->blobed;
	if (clr[x + WIDTH*y] == clr[u + WIDTH*v])
	{
		int bindex = blb[u + WIDTH*v];

		if (y - 1 >= 0 && (clr[x + WIDTH*y] == clr[x + WIDTH*(y - 1)]))
			blobs[bindex].addBlob(x, y, true);//delete
		else {
			blobs[bindex].addBlob(x, y, false);//not delete
			FtMap->Isdeleted[x + WIDTH*(y - 1)] = true;
		}
	}
	else return false;
}


void Associate(Map* FtMap, Blob* blobs, int x, int y)
{
	bool diax = false, diay = false;

	if (FtMap->color[x + WIDTH*y] == 999) return;//no color

	if (x - 1 >= 0){ diax = true; if (checkMap(FtMap, blobs, x, y, x - 1, y)) return; }
	if (y - 1 >= 0){ diay = true; if (checkMap(FtMap, blobs, x, y, x, y - 1)) return; }
	if (diax&&diay && checkMap(FtMap, blobs, x, y, x - 1, y - 1)) return;
	if (x + 1 <= WIDTH && diay && checkMap(FtMap, blobs, x, y, x + 1, y - 1)) return;

	blobs[nblob] = Blob(FtMap->color[x + WIDTH*y], x, y);
	FtMap->blobed[x + WIDTH*y] = nblob;
	nblob++;
}


/*void BlobFilter(ByteImage img, Blob* blobs)
{
	BYTE* pImg = img.pImageData;

	Map FilterMap(img.Height, img.Width);

	for (int cols = 0; cols<img.Height; cols++)
		for (int rows = 0; rows<img.Width; rows++)
		{
			Mapping(pImg, &FilterMap, rows + cols*img.Width);

			Associate(&FilterMap, blobs, rows, cols);

			pImg += 3;
		}
	
	return;

}*/

/*int main()
{
	ByteImage img;

	Blob* blobs = new Blob[max_blob_num];

	BlobFilter(img, blobs);
}
*/
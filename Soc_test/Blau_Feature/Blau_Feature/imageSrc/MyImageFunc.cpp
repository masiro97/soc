#include "stdafx.h"
#include "MyImageFunc.h"
#include "BlauImage.h"

// 논리 연산자 정의
void ANDImage(const ByteImage& src1, const ByteImage& src2, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src1.GetPtr(r);
		BYTE* pSrc2 = src2.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc1[c] & pSrc2[c]);
		}
	}
}

ByteImage operator&(const ByteImage& src1, const ByteImage& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	ASSERT(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2);

	ByteImage ret(nWidth1, nHeight1, nChan1);
	ANDImage(src1, src2, ret);
	return ret;
}

void ANDConst(const ByteImage& src, const BYTE val, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;


	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc = src.GetPtr(r);
		BYTE* pDst = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc[c] & val);
		}
	}
}

ByteImage operator&(const ByteImage& src, const BYTE val)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	ANDConst(src, val, ret);
	return ret;
}

void ORImage(const ByteImage& src1, const ByteImage& src2, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src1.GetPtr(r);
		BYTE* pSrc2 = src2.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc1[c] | pSrc2[c]);
		}
	}
}

ByteImage operator|(const ByteImage& src1, const ByteImage& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	ASSERT(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2);

	ByteImage ret(nWidth1, nHeight1, nChan1);
	ORImage(src1, src2, ret);
	return ret;
}

void ORConst(const ByteImage& src, const BYTE val, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;


	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc = src.GetPtr(r);
		BYTE* pDst = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc[c] | val);
		}
	}
}

ByteImage operator|(const ByteImage& src, const BYTE val)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	ORConst(src, val, ret);
	return ret;
}

void NOTImage(const ByteImage& src, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (~pSrc1[c]);
		}
	}
}

ByteImage operator~(const ByteImage& src)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	NOTImage(src, ret);
	return ret;
}

void XORImage(const ByteImage& src1, const ByteImage& src2, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src1.GetPtr(r);
		BYTE* pSrc2 = src2.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc1[c] ^ pSrc2[c]);
		}
	}
}

ByteImage operator^(const ByteImage& src1, const ByteImage& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	ASSERT(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2);

	ByteImage ret(nWidth1, nHeight1, nChan1);
	XORImage(src1, src2, ret);
	return ret;
}

void XORConst(const ByteImage& src, const BYTE val, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc = src.GetPtr(r);
		BYTE* pDst = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc[c] ^ val);
		}
	}
}

ByteImage operator^(const ByteImage& src, const BYTE val)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	XORConst(src, val, ret);
	return ret;
}

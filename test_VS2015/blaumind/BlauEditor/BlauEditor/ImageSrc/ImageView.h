#pragma once

#include "BlauImage.h"

// CImageView 뷰입니다.

class CImageView : public CView
{
	DECLARE_DYNCREATE(CImageView)

public:
	CImageView();           // 동적 만들기 생성자, ImageFrameWnd의 멤버변수사용을 위해 public
	virtual ~CImageView();
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	void CImageView::SetImage(const ByteImage& input);
	BOOL CImageView::CreateEx(CWnd *pWnd);


	ByteImage m_image;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


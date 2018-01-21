#pragma once

#include "ImageView.h"
#include "BlauImage.h"
#include "BlauFeatureExtractor.h"
// CImageFrameWnd 프레임입니다.

class CImageFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CImageFrameWnd)
public:
	CImageFrameWnd::CImageFrameWnd();
	CImageFrameWnd::CImageFrameWnd(const ByteImage &image, const char *name);
		// 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CImageFrameWnd();
	CImageView& GetImageView() { return m_view; };

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PostNcDestroy();

	CImageView	m_view;
};



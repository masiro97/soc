
// Blau_FeatureDlg.h : 헤더 파일
//

#pragma once
#include "imageSrc\BlauImage.h"

// CBlau_FeatureDlg 대화 상자
class CBlau_FeatureDlg : public CDialogEx
{
// 생성입니다.
public:
	CBlau_FeatureDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLAU_FEATURE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	ByteImage Image_in;
	ByteImage Image_out;
	afx_msg void OnBnClickedButtonSobel();
	afx_msg void OnBnClickedButtonLoadInput();
	afx_msg void OnBnClickedButtonSaveImage();
};

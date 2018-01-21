
// BlauEditorDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "ImageSrc\BlauImage.h"
#include "ImageSrc\BlauCam.h"
#include "afxwin.h"

// CBlauEditorDlg 대화 상자
class CBlauEditorDlg : public CDialogEx
{
// 생성입니다.
public:
	CBlauEditorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	static UINT ThreadFirst(LPVOID _mothod);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLAUEDITOR_DIALOG };
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
	BlauCam m_webCam;
	

	
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CStatic m_pic;
	afx_msg void OnBnClickedButtonDetect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

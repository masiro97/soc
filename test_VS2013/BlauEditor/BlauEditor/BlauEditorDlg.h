
// BlauEditorDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "ImageSrc\BlauImage.h"
#include "ImageSrc\BlauCam.h"
#include "afxwin.h"
#include <windows.h>
#include <process.h>

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
	afx_msg void OnBnClickedButtonCapture();
	CStatic m_pic;
	afx_msg void OnBnClickedButton();
	CProgressCtrl progress;
	afx_msg void OnNMCustomdrawProgress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAuto();
	afx_msg void OnBnClickedButtonStep();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonFps();
};

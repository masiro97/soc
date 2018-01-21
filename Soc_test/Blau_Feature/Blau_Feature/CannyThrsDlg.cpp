// CannyThrsDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Blau_Feature.h"
#include "CannyThrsDlg.h"
#include "afxdialogex.h"


// CCannyThrsDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCannyThrsDlg, CDialogEx)

CCannyThrsDlg::CCannyThrsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CANNY, pParent)
{

}

CCannyThrsDlg::~CCannyThrsDlg()
{
}

void CCannyThrsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCannyThrsDlg, CDialogEx)
END_MESSAGE_MAP()


// CCannyThrsDlg 메시지 처리기입니다.

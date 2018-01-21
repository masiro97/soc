
// BlauEditorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BlauEditor.h"
#include "BlauEditorDlg.h"
#include "afxdialogex.h"
#include "ImageFrameWnd.h"
#include "ImageFrameWndManager.h"
#include "LoadImageFromFileDialog.h"
#include "MyImageFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ByteImage ImgIn1;
ByteImage ImgIn2;
ByteImage ImgOut;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBlauEditorDlg 대화 상자



CBlauEditorDlg::CBlauEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BLAUEDITOR_DIALOG, pParent)
	
	, m_bImgOperand(FALSE)
	, m_nOperator(0)
	, m_dOperandVal(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBlauEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_IMAGE, m_bImgOperand);
	DDX_Radio(pDX, IDC_RADIO_PLUS, m_nOperator);
	DDX_Text(pDX, IDC_EDIT_CONST_VAL, m_dOperandVal);
}

BEGIN_MESSAGE_MAP(CBlauEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD1, &CBlauEditorDlg::OnBnClickedButtonLoad1)
	ON_BN_CLICKED(IDC_BUTTON_LOAD2, &CBlauEditorDlg::OnBnClickedButtonLoad2)
	ON_BN_CLICKED(IDC_RADIO_IMAGE, &CBlauEditorDlg::OnBnClickedRadioImage)
	ON_BN_CLICKED(IDC_RADIO_CONST, &CBlauEditorDlg::OnBnClickedRadioConst)
END_MESSAGE_MAP()


// CBlauEditorDlg 메시지 처리기

BOOL CBlauEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBlauEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBlauEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBlauEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBlauEditorDlg::OnBnClickedButtonLoad1()
{
	ImgIn1 = LoadImageFromDialog();
	if (!ImgIn1.IsEmpty()) ShowImage(ImgIn1, "입력 영상1");
}


void CBlauEditorDlg::OnBnClickedButtonLoad2()
{
	ImgIn2 = LoadImageFromDialog();
	if (!ImgIn2.IsEmpty()) ShowImage(ImgIn2, "입력 영상2");
}


void CBlauEditorDlg::OnBnClickedRadioImage()
{
	GetDlgItem(IDC_BUTTON_LOAD2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_CONST_VAL)->EnableWindow(FALSE);

	if (!ImgIn2.IsEmpty()) ShowImage(ImgIn2, "입력 영상2");
}


void CBlauEditorDlg::OnBnClickedRadioConst()
{
	GetDlgItem(IDC_BUTTON_LOAD2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CONST_VAL)->EnableWindow(TRUE);
	CloseImage("입력 영상2");
}


bool CBlauEditorDlg::_ImageOpImage()
{
	int nWidth = ImgIn1.GetWidth();
	int nHeight = ImgIn1.GetHeight();
	int nCh = ImgIn1.GetChannel();

	if (m_nOperator != 6 &&
		(nWidth != ImgIn2.GetWidth() ||
			nHeight != ImgIn2.GetHeight() ||
			nCh != ImgIn2.GetChannel()))
	{
		AfxMessageBox("입력 영상의 규격이 다릅니다.");
		return false;
	}

	switch (m_nOperator)
	{
	case 0: // +
		ImgOut = ImgIn1 + ImgIn2;
		break;
	case 1: // -
		ImgOut = ImgIn1 - ImgIn2;
		break;
	case 2: // *
		ImgOut = ImgIn1 * ImgIn2;
		break;
	case 3: // /
		ImgOut = ImgIn1 / ImgIn2;
		break;
	case 4: // &
		ImgOut = ImgIn1 & ImgIn2;
		break;
	case 5: // |
		ImgOut = ImgIn1 | ImgIn2;
		break;
	case 6: // ~
		ImgOut = ~ImgIn1;
		break;
	case 7: // ^
		ImgOut = ImgIn1 ^ ImgIn2;
		break;
	default:
		break;
	}

	return true;
}

bool CBlauEditorDlg::_ImageOpConst()
{
	switch (m_nOperator)
	{
	case 0: // +
		ImgOut = ImgIn1 + m_dOperandVal;
		break;
	case 1: // -
		ImgOut = ImgIn1 - m_dOperandVal;
		break;
	case 2: // *
		ImgOut = ImgIn1 * m_dOperandVal;
		break;
	case 3: // /
		ImgOut = ImgIn1 / m_dOperandVal;
		break;
	case 4: // &
		ImgOut = ImgIn1 & m_dOperandVal;
		break;
	case 5: // |
		ImgOut = ImgIn1 | m_dOperandVal;
		break;
	case 6: // ~
		ImgOut = ~ImgIn1;
		break;
	case 7: // ^
		ImgOut = ImgIn1 ^ m_dOperandVal;
		break;
	default:
		break;
	}

	return true;
}

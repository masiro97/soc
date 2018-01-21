
// BlauEditorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BlauEditor.h"
#include "BlauEditorDlg.h"
#include "afxdialogex.h"

#include "ImageSrc/ImageFrameWndManager.h"
#include "ImageSrc/LoadImageFromFileDialog.h"
#include "ImageSrc/MyImageFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ByteImage gImageBuf;
BITMAPINFO gBmpInfo;

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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBlauEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAM, m_pic);
}

BEGIN_MESSAGE_MAP(CBlauEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON_START, &CBlauEditorDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CBlauEditorDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CBlauEditorDlg::OnBnClickedButtonCapture)
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

void YUY2ToRGB24(int nWidth, int nHeight, const BYTE* pYUY2, BYTE* pRGB24)
{
	int nSize = nHeight * (nWidth / 2);

	int idxSrc = 0;
	int idxDst = 0;

	BYTE y1, u, y2, v;
	BYTE r, g, b;

	for (int i = 0; i<nSize; i++)
	{
		y1 = pYUY2[idxSrc];
		u = pYUY2[idxSrc + 1];
		y2 = pYUY2[idxSrc + 2];
		v = pYUY2[idxSrc + 3];

		// 첫 번째 픽셀
		b = CLIP((76284 * (y1 - 16) + 132252 * (u - 128)) >> 16);
		g = CLIP((76284 * (y1 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
		r = CLIP((76284 * (y1 - 16) + 104595 * (v - 128)) >> 16);

		pRGB24[idxDst] = b;
		pRGB24[idxDst + 1] = g;
		pRGB24[idxDst + 2] = r;

		idxDst += 3;

		// 두 번째 픽셀
		b = CLIP((76284 * (y2 - 16) + 132252 * (u - 128)) >> 16);
		g = CLIP((76284 * (y2 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
		r = CLIP((76284 * (y2 - 16) + 104595 * (v - 128)) >> 16);

		pRGB24[idxDst] = b;
		pRGB24[idxDst + 1] = g;
		pRGB24[idxDst + 2] = r;

		idxDst += 3;
		idxSrc += 4;
	}
}

ByteImage gBgImage, gBinImage;

LRESULT ProcessCamFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	if (gBmpInfo.bmiHeader.biCompression == BI_RGB) // RGB 영상
	{
		memcpy(gImageBuf.GetPtr(), lpVHdr->lpData,
			gBmpInfo.bmiHeader.biHeight*gBmpInfo.bmiHeader.biWidth*gImageBuf.GetChannel());
	}
	else if (gBmpInfo.bmiHeader.biCompression == MAKEFOURCC('Y', 'U', 'Y', '2')) // 16비트 영상
	{
		YUY2ToRGB24(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight,
			lpVHdr->lpData, gImageBuf.GetPtr());
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void CBlauEditorDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_STATIC_CAM)->SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOZORDER | SWP_NOMOVE);
	m_webCam.StartCam(GetDlgItem(IDC_STATIC_CAM), 640, 480, gBmpInfo);
	gImageBuf = ByteImage(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight, 3);

	m_webCam.SetCallBackOnFrame(ProcessCamFrame);
}


void CBlauEditorDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_webCam.StopCam();
}

void CBlauEditorDlg::OnBnClickedButtonCapture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	ShowImage(gImageBuf.Flip(), "capture");
}

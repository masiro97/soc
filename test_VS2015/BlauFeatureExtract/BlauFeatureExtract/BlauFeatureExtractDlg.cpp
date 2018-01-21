
// BlauFeatureExtractDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "imageSrc\BlauFeatureExtract.h"
#include "afxdialogex.h"
#include "imageSrc\Features.h"
#include "imageSrc\MyImageFunc.h"
#include "imageSrc\ImageFrameWndManager.h"
#include "imageSrc\LoadImageFromFileDialog.h"
#include <math.h>
#include "BlauFeatureExtractDlg.h"
#include "imageSrc\CannyThrsDlg.h"
#include "imageSrc\HoughParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CBlauFeatureExtractDlg 대화 상자



CBlauFeatureExtractDlg::CBlauFeatureExtractDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BLAUFEATUREEXTRACT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBlauFeatureExtractDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBlauFeatureExtractDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CBlauFeatureExtractDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMAGE, &CBlauFeatureExtractDlg::OnBnClickedButtonSaveImage)
	ON_BN_CLICKED(IDC_BUTTON_SOBEL, &CBlauFeatureExtractDlg::OnBnClickedButtonSobel)
	ON_BN_CLICKED(IDC_BUTTON_CANNY, &CBlauFeatureExtractDlg::OnBnClickedButtonCanny)
	ON_BN_CLICKED(IDC_BUTTON_HOUGH, &CBlauFeatureExtractDlg::OnBnClickedButtonHough)
	ON_BN_CLICKED(IDC_BUTTON_HARRIS, &CBlauFeatureExtractDlg::OnBnClickedButtonHarris)
END_MESSAGE_MAP()


// CBlauFeatureExtractDlg 메시지 처리기

BOOL CBlauFeatureExtractDlg::OnInitDialog()
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

void CBlauFeatureExtractDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBlauFeatureExtractDlg::OnPaint()
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
HCURSOR CBlauFeatureExtractDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBlauFeatureExtractDlg::OnBnClickedButtonLoadImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Image_in = LoadImageFromDialog();
	ShowImage(Image_in, "입력 영상");
}


void CBlauFeatureExtractDlg::OnBnClickedButtonSaveImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Image_out.SaveImage("Result.bmp");
}


void CBlauFeatureExtractDlg::OnBnClickedButtonSobel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (Image_in.GetChannel() == 3)
		SobelEdge(RGB2Gray(Image_in), Image_out);
	else
		SobelEdge(Image_in, Image_out);
	ShowImage(Image_out, "Sobel 검출기");
}


void CBlauFeatureExtractDlg::OnBnClickedButtonCanny()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCannyThrsDlg cannyDlg;
	if (cannyDlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		CannyEdge(RGB2Gray(Image_in), Image_out, cannyDlg.m_nThresholdHi, cannyDlg.m_nThresholdLo);
		ShowImage(Image_out, "캐니 경계선 검출기");
	}
}


void CBlauFeatureExtractDlg::OnBnClickedButtonHough()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ByteImage imageEdge;
	CHoughParam houghDlg;
	if (houghDlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		if (houghDlg.m_nEdgeMethod == 0)
		{
			if (Image_in.GetChannel() == 3)
				SobelEdge(RGB2Gray(Image_in), imageEdge);
			else
				SobelEdge(Image_in, imageEdge);
		}
		else
		{
			if (Image_in.GetChannel() == 3)
				CannyEdge(RGB2Gray(Image_in), imageEdge, 60, 30);
			else
				CannyEdge(Image_in, imageEdge, 60, 30);
		}
		//		ShowImage(imageEdge, "경계선 영상");

		double arrRho[100];
		double arrTheta[100];
		int nLine = HoughLines(imageEdge, houghDlg.m_nTNum, houghDlg.m_NTVal, houghDlg.m_dResTheta, 100, arrRho, arrTheta);

		Image_out = Image_in;
		// 직선 그리기
		for (int i = 0; i<nLine; i++)
		{
			if (arrTheta[i] == 90) // 수직선
			{
				DrawLine(Image_out, arrRho[i], 0, arrRho[i], Image_out.GetHeight() - 1, 255, 0, 0);
			}
			else
			{
				int x1 = 0;
				int y1 = (int)(arrRho[i] / cos(arrTheta[i] * M_PI / 180) + 0.5);
				int x2 = Image_out.GetWidth() - 1;
				int y2 = (int)((arrRho[i] - x2*sin(arrTheta[i] * M_PI / 180)) / cos(arrTheta[i] * M_PI / 180) + 0.5);
				DrawLine(Image_out, x1, y1, x2, y2, 255, 0, 0);
			}
		}
		ShowImage(Image_out, "직선 검출 결과");
	}
}


void CBlauFeatureExtractDlg::OnBnClickedButtonHarris()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double cornerX[1000]; // 최대 코너의 수: 1000
	double cornerY[1000];

	int numCorner;
	if (Image_in.GetChannel() == 3)
		numCorner = HarrisCorner(RGB2Gray(Image_in), 1e8, 0.04, 1000, cornerX, cornerY);
	else
		numCorner = HarrisCorner(Image_in, 1e9, 0.04, 1000, cornerX, cornerY);

	// 추출된 코너를 영상에 표시
	BYTE colorCross[3] = { 0, 0, 255 };
	if (Image_in.GetChannel() == 3)
		Image_out = Image_in;
	else
		Image_out = Gray2RGB(Image_in);

	for (int n = 0; n<numCorner; n++)
	{
		if (Image_in.GetChannel() == 3)
			DrawCross(Image_out, cornerX[n], cornerY[n], colorCross);
		else
			DrawCross(Image_out, cornerX[n], cornerY[n], colorCross);
	}
	ShowImage(Image_out, "코너 추출 결과");
}

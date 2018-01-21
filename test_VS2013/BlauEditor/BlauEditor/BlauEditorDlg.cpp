
// BlauEditorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BlauEditor.h"
#include "BlauEditorDlg.h"
#include "afxdialogex.h"
#include "ImageSrc/ImageFrameWndManager.h"
#include "ImageSrc/LoadImageFromFileDialog.h"
#include "ImageSrc/MyImageFunc.h"
#include <windows.h>
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ByteImage gImageBuf;
BITMAPINFO gBmpInfo;

UINT CBlauEditorDlg::ThreadFirst(LPVOID _mothod) {

	CBlauEditorDlg *fir = (CBlauEditorDlg*)_mothod;

	//CBlauEditorDlg *pDlg = (CBlauEditorDlg*)AfxGetApp()->m_pMainWnd;
	//pDlg->mNumOutOut = 100;

	while (1) {

		//..이 곳에 로프시 수행할 동작을 선언하세요.
//		UpdateFPS();
		//CImageView *m = NULL;
		//m->OnLButtonDown(1, CPoint(100,100));
		// -> 따라서 반지름 r, 중심 x,y인 원을 그리려면
		//HDC hdc = ::GetDC(hWnd);
		//Ellipse(hdc, 50, 50, 150, 150);

		Sleep(10);
	}
	return 0;
}
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
	DDX_Control(pDX, IDC_PROGRESS, progress);
}

BEGIN_MESSAGE_MAP(CBlauEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON_START, &CBlauEditorDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CBlauEditorDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CBlauEditorDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_BUTTON, &CBlauEditorDlg::OnBnClickedButton)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS, &CBlauEditorDlg::OnNMCustomdrawProgress)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CBlauEditorDlg::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_STEP, &CBlauEditorDlg::OnBnClickedButtonStep)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CBlauEditorDlg::OnBnClickedButtonReset)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CBlauEditorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBlauEditorDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FPS, &CBlauEditorDlg::OnBnClickedButtonFps)
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
	progress.SetRange(0, 100);
	progress.SetPos(50);
	
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
	SetTimer(100, 1.0 / 30.0, 0);
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

	
	CWinThread *p1 = NULL;
	p1 = AfxBeginThread(ThreadFirst, this);

	if (p1 == NULL) {

		AfxMessageBox("Error");
	}
	CloseHandle(p1);
}


void CBlauEditorDlg::OnBnClickedButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//ShowImage(gImageBuf.Flip(), "capture");
	ByteImage Img = RGB2Gray(gImageBuf.Flip());

	DrawLines(Img, 50, 1, 50, 200,10,255);

	ShowImage(Img, "capture");
}


void CBlauEditorDlg::OnNMCustomdrawProgress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	progress.SetRange(0, 100);  //진행폭을 결정
	progress.SetStep(1);          //얼마씩 진행할 것인지를 결정

	for (int a = 0; a <= 99; a++)
	{
		progress.StepIt();  //SetStep에서 결정한 값대로 진행
		progress.GetPos();
		//Sleep(100);                 //지연설정(1000을 넣으면 1초 delay)
	}


}


void CBlauEditorDlg::OnButtonAuto()
{
	for (int i = 0;i<100;i++)
	{
		progress.SetPos(i);
		Sleep(50);   //변화의 차이를 보기 위해 슬립
	}
}

void CBlauEditorDlg::OnButtonStep()
{
	progress.SetStep(2); //눌릴때마다 단계적으로 증가
	progress.StepIt();

}

void CBlauEditorDlg::OnButtonReset()
{
	progress.SetPos(0);
}



void CBlauEditorDlg::OnBnClickedButtonAuto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0;i<100;i++)
	{
		progress.SetPos(i);
		Sleep(50);   //변화의 차이를 보기 위해 슬립
	}
}


void CBlauEditorDlg::OnBnClickedButtonStep()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	progress.SetStep(2); //눌릴때마다 단계적으로 증가
	progress.StepIt();
}
//PBS_smoothS

void CBlauEditorDlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	progress.SetPos(0);
}


void CBlauEditorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CTime timer;
	timer = CTime::GetCurrentTime();
	CString strTimer;
	strTimer = timer.Format("%H시 %M분 %S 초");
	
	SetTimer(10, 1.0 / 30.0, 0);
	switch (nIDEvent) {

	case 100:
		//1초마다 처리할 내용
		ShowImage(gImageBuf.Flip(), "Capture");

		break;

	case 10:
		//처리할 내용

		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CBlauEditorDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(0);
	CDialogEx::OnOK();
}


void CBlauEditorDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(0);
	CDialogEx::OnCancel();
}


void CBlauEditorDlg::OnBnClickedButtonFps()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// frame rate setting 
	DWORD dwFrames;
	DWORD dwCurrentTime;
	DWORD dwLastUpdateTime;
	DWORD dwElapsedTime;
	char  szFPS[32];
	// Zero out the frames per second variables:
	dwFrames = 0;
	dwCurrentTime = 0;
	dwLastUpdateTime = 0;
	dwElapsedTime = 0;
	szFPS[0] = '\0';

	// Calculate the number of frames per one second:
	dwFrames++;
	dwCurrentTime = GetTickCount(); // Even better to use timeGetTime()
	dwElapsedTime = dwCurrentTime - dwLastUpdateTime;

	if (dwElapsedTime >= 1000)
	{
		sprintf(szFPS, ("FPS = %2.2f"), (float)(dwFrames * 1000.0 / dwElapsedTime));
		dwFrames = 0;
		dwLastUpdateTime = dwCurrentTime;
	}
	std::cout << szFPS << std::endl;


}

/*void UpdateFPS()
{

	static DWORD frameCount = 0;            //프레임 카운트수
	static float timeElapsed = 0.0f;            //흐른 시간
	static DWORD lastTime = timeGetTime();   //마지막 시간(temp변수)

	DWORD curTime = timeGetTime();      //현재 시간
	float timeDelta = (curTime - lastTime)*0.001f;        //timeDelta(1번생성후 흐른 시간) 1초단위로 바꿔준다.
	TCHAR pFPS[256];
	timeElapsed += timeDelta;

	frameCount++;

	if (timeElapsed >= 1.0f)         //흐른시간이 1초이상이면 내가 하고싶은것 처리
	{
		float fps = (float)frameCount / timeElapsed;
		//sprintf_s(pFPS, sizeof(pFPS), TEXT("게임속도 FPS: %f"), fps);     //FPS출력 하는 소스

		frameCount = 0;
		timeElapsed = 0.0f;
	}
	else
	{
		//흐른 시간이 1초가 안되면 생략함  
		//Sleep() 해도되고 안해도 되구~ 
	}



	lastTime = curTime;
}
*/
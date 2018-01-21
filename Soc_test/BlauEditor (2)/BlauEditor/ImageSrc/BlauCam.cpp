#include "stdafx.h"
#include "BlauCam.h"

///////////////////////////////////////////////////////////////////////////////
// 카메라 캡쳐 클래스 
///////////////////////////////////////////////////////////////////////////////
BlauCam::BlauCam() :
	m_hCam(NULL)
{

}

BlauCam::~BlauCam()
{
	if (m_hCam)
		StopCam();
}

bool BlauCam::StartCam(CWnd* wnd, int nWidth, int nHeight, BITMAPINFO& bmpInfo)
{
	// 캡쳐 영상을 출력할 윈도우의 크기 및 위치 조절
	m_hCam = capCreateCaptureWindow("Cam Capture", WS_CHILD | WS_VISIBLE,
		0, 0, nWidth, nHeight, wnd->m_hWnd, NULL);

	if (!capDriverConnect(m_hCam, 0))
	{
		AfxMessageBox("연결된 카메라를 찾을 수 없습니다.");
		return false;
	}

	//   capDlgVideoFormat(m_hCam);
	capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));

	// 캡쳐 영상의 크기를 희망 값으로 설정
	bmpInfo.bmiHeader.biWidth = nWidth;
	bmpInfo.bmiHeader.biHeight = nHeight;
	bmpInfo.bmiHeader.biSizeImage = nWidth*nHeight*bmpInfo.bmiHeader.biBitCount / 8;
	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))
	{
		// 설정에 실패하면 원래 값으로 되돌림
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	// 캡쳐 영상이 24비트 RGB가 될 수 있도록 설정
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))
	{
		// 설정에 실패하면 원래 값으로 되돌림
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	capOverlay(m_hCam, TRUE);
	capPreviewRate(m_hCam, 30);
	capPreview(m_hCam, TRUE);

	return true;
}

void BlauCam::StopCam()
{
	capCaptureAbort(m_hCam);
	capDriverDisconnect(m_hCam);
	m_hCam = NULL;
}

BOOL BlauCam::SetCallBackOnFrame(LRESULT(*fpProc)(HWND, LPVIDEOHDR))
{
	return capSetCallbackOnFrame(m_hCam, fpProc);
}
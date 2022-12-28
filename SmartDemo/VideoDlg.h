#pragma once
#include "VideoWnd.h"
#include "SDK.h"

#define MAX_SUBWND_ROWCOL 8
#define MAX_SUBWND_COUNT	(MAX_SUBWND_ROWCOL*MAX_SUBWND_ROWCOL)
// VideoDlg dialog

class VideoDlg : public CDialog
{
	DECLARE_DYNAMIC(VideoDlg)

public:
	VideoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~VideoDlg();

// Dialog Data
	enum { IDD = IDD_VIDEO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	RECT m_rectAllSubWnd;
	CVideoWnd* m_pWndSubVideo[MAX_SUBWND_COUNT];
	RealHandle m_hReal[MAX_SUBWND_COUNT];
	int m_nMultiType;
	int m_nCurSubWnd;
	bool m_bFullScreen;
	int m_nLastSubWnd;
	RealOpenPara m_hRealOpenPara[MAX_SUBWND_COUNT];
	CVideoWnd* m_currentMax;
	WINDOWPLACEMENT _temppl;

	virtual BOOL OnInitDialog();
	void RefreshMultiWnd(int nSquare, bool bFullScreen);
	void UpdateVideoWndUI(void);
	void ShowAllSubWnd(bool bShow);
	void SubWndLButtonDblClk(int nSubNumber);
	void QuitFullScreeen(void);
	void SubWndRButtonDown(int nSubNumber);
	void SubWndLButtonDown(int nSubNumber);
	int OnPreview(NvdHandle hNvd, int winNum);
	BOOL PreTranslateMessage(MSG* pMsg);

	int Stop(int winNum);
};

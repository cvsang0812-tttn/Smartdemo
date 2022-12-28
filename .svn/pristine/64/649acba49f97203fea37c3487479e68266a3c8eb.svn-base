#pragma once
#include "afxcmn.h"
#include "sdk.h"
#include "afxwin.h"


// RecogScene dialog

class RecogScene : public CDialog
{
	DECLARE_DYNAMIC(RecogScene)

public:
	RecogScene(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecogScene();

// Dialog Data
	enum { IDD = IDD_RECOGDETAIL_SCENE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	BOOL RecogScene::ShowImage(CDC* pDC,CString strPath, int x, int y);

	NvdHandle m_hDev;
	unsigned long long m_pts64;
	unsigned char m_scene[2 * 1024 * 1024];
	BOOL setParam(NvdHandle hDev, unsigned long long pts64);

};

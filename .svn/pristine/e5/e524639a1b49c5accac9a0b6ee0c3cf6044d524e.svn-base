#pragma once
#include "afxcmn.h"
#include "sdk.h"
#include "afxwin.h"
#include "RecogScene.h"

// RecogDetailDlg dialog

class RecogDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(RecogDetailDlg)

public:
	RecogDetailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecogDetailDlg();

// Dialog Data
	enum { IDD = IDD_RECOGDETAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void setParam(NvdHandle hDev, FaceRecogTopNSimi *recog);
	BOOL ShowImage(CDC* pDC,CString strPath, int x, int y);
	NvdHandle m_hDev;
	FaceRecogTopNSimi *m_recog;
	FaceNetPhotoDescVector m_desc;
	int m_faceCount;
	int m_hasShowPic;
    RecogScene m_RecogSceneDlg;

	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonViewScene();
};

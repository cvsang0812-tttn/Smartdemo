#pragma once
#include "afxcmn.h"
#include "sdk.h"
#include "afxwin.h"

// FaceDetialDlg dialog

class FaceDetialDlg : public CDialog
{
	DECLARE_DYNAMIC(FaceDetialDlg)

public:
	FaceDetialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~FaceDetialDlg();

// Dialog Data
	enum { IDD = IDD_PHOTODETAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void setParam(NvdHandle hDev);
	void setFaceId(unsigned int inputId);
	BOOL ShowImage(CDC* pDC,CString strPath, int x, int y);
	NvdHandle m_hDev;
	unsigned int id;
	FaceNetPhotoDescVector *photoDesc;
	afx_msg void OnBnClickedDetailModify();
	CEdit m_CtrlNewName;
	afx_msg void OnPaint();
};

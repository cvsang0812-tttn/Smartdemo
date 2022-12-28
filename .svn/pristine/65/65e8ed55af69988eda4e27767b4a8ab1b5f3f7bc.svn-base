#pragma once
#include "SDK.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include "FaceDetialDlg.h"

// FindPhotoDlg dialog

class FindPhotoDlg : public CDialog
{
	DECLARE_DYNAMIC(FindPhotoDlg)

public:
	FindPhotoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~FindPhotoDlg();

// Dialog Data
	enum { IDD = IDD_FINDPHOTO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void LoadData(NvdHandle hNvd);
	NvdHandle m_hNvd;
	CComboBox m_ctrlProvince;
	CComboBox m_ctrlCity;
	CComboBox m_ctrlCertType;
	afx_msg void OnCbnSelchangeComboFindFaceProv();
	afx_msg void OnBnClickedBtnFindFaceFind();
	CEdit m_ctrlName;
	CComboBox m_ctrlGender;
	CEdit m_ctrlCertNumber;
	CDateTimeCtrl m_ctrlBirthDayBegin;
	CDateTimeCtrl m_ctrlBirthDayEnd;
	CListCtrl m_ctrlFaceList;
	afx_msg void OnBnClickedBtnFindFaceReset();
	afx_msg void OnBnClickedBtnFaceDel();
	afx_msg void OnBnClickedBtnViewFace();

	FaceDetialDlg m_FaceDetailDlg;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnFaceDelAll();
};

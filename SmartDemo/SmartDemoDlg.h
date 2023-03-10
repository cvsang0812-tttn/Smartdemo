// SmartDemoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "VideoDlg.h"
#include "SDK.h"
#include "ParaDlg.h"

// CSmartDemoDlg 对话框
class CSmartDemoDlg : public CDialog
{
// 构造
public:
	CSmartDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SMARTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_Ip;
	CString m_UserName;
	CString m_PassWord;
	CButton m_LoginBTN;
	CButton m_ExitBTN;
	CButton m_PlayVideBTN;
	NvdHandle m_hDev;
	CStatic m_ctrlVideoWin;
	VideoDlg m_ctrlVideoDlg;
	CButton m_CloseVideoBTN;
	CButton m_ParamBTN;
	SOCKET sVdcp;
	SOCKET sFaceCmd;
	SOCKET sFaceNotify;
	CButton m_RegBTN;
	int m_RegFlag;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedLogin();
	BOOL CSmartDemoDlg::ShowImage(CDC* pDC,CString strPath, int x, int y);
	afx_msg void OnBnClickedButtonOpenvideo();
	afx_msg void OnBnClickedButtonClosevideo();
	afx_msg void OnBnClickedButtonParam();
	CStatic m_ctrlSimilarity;
	afx_msg void OnBnClickedButtonRegister();
	void registerCallback(void *pPara, SmartRegClientHandle regHandle, SmartRegClientInfo regInfo);
	LRESULT OnDisconnectMsg(WPARAM wParam, LPARAM lParam);
	void hello_Thread();
	afx_msg void OnStnClickedStaticSdkVersion();
	CStatic m_ANH;
	afx_msg void OnStnClickedAnh();
	CBitmap CBITMAP_;

};

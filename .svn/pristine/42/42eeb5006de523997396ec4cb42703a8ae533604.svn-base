// VideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "VideoDlg.h"

int realDataCallback(void* pPara, void* vBuffer, int nSize)
{
	FrameHeader* pHdr = NULL;
	unsigned char *frameData = NULL;

	pHdr = (FrameHeader*)vBuffer;
	
	switch( pHdr->type )
    {
	case VDFP_FRAME_TYPE_VIDEO:
        break;
	case VDFP_FRAME_TYPE_AUDIO:
		return 0;
	case VDFP_FRAME_TYPE_AI_FACE_DETECT:
		return 0;
	case VDFP_FRAME_TYPE_AI_FACE_RECOG:
		return 0;
	default:
		return 0;
    }

	frameData = (unsigned char *)vBuffer+sizeof(FrameHeader);

	return 0;
}

// VideoDlg dialog

IMPLEMENT_DYNAMIC(VideoDlg, CDialog)

VideoDlg::VideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(VideoDlg::IDD, pParent)
	, m_nMultiType(0)
	, m_nCurSubWnd(0)
	, m_bFullScreen(false)
	, m_nLastSubWnd(0)
	, m_currentMax(NULL)
{
	for (int i=0; i<MAX_SUBWND_COUNT; i++)
	{
		m_pWndSubVideo[i] = NULL;
		m_hReal[i] = NULL;
	}

}

VideoDlg::~VideoDlg()
{
}

void VideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(VideoDlg, CDialog)
END_MESSAGE_MAP()


// VideoDlg message handlers
BOOL VideoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//大窗口区域
	GetDlgItem(IDC_STATIC_WNDALL)->GetWindowRect(&m_rectAllSubWnd);
	ScreenToClient(&m_rectAllSubWnd);

	//创建小视频窗口(但未显示)
	for (int i = 0; i < MAX_SUBWND_COUNT; i++)
	{
		m_pWndSubVideo[i] = new CVideoWnd(i, this);
		m_pWndSubVideo[i]->Create(IDD_VIDEOWND, this);
	}

	//显示多画面效果
	m_pWndSubVideo[0]->m_color = CVideoWnd::ACTIVECOLOR;
	RefreshMultiWnd(m_nMultiType+1, FALSE);
	UpdateVideoWndUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void VideoDlg::RefreshMultiWnd(int nSquare, bool bFullScreen)
{
	if (0 == nSquare || nSquare > MAX_SUBWND_ROWCOL)
	{
		return;
	}

	//调整环境状态
	if (bFullScreen)
	{
		if (!m_bFullScreen)		//切换到全屏
		{
			ShowAllSubWnd(FALSE);
			::SetWindowLong(m_hWnd, GWL_STYLE, ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_CAPTION);
			ShowWindow(SW_MAXIMIZE);
		}
	}
	else
	{
		if (m_bFullScreen)		//切换到窗口
		{
			ShowAllSubWnd(FALSE);
			::SetWindowLong(m_hWnd, GWL_STYLE, ::GetWindowLong(m_hWnd, GWL_STYLE) | ~WS_CAPTION);
			ShowWindow(SW_NORMAL);
			ShowAllSubWnd(TRUE);
		}
	}

	//隐藏背景框
	m_bFullScreen = bFullScreen;
	GetDlgItem(IDC_STATIC_WNDALL)->ShowWindow(SW_HIDE);

	//检测是否有放大窗口,并隐藏所有视频窗口
	int nIndex = -1;
	for (int i=0; i<MAX_SUBWND_COUNT; i++)
	{
		m_pWndSubVideo[i]->ShowWindow(SW_HIDE);
		if (m_pWndSubVideo[i]->m_bMaxFlag)
		{
			nIndex = i;
		}
	}

	//背景框范围调整
	CRect rc;
	int nInterval = 0;				//小窗口间隔

	if (bFullScreen)
	{
		GetWindowRect(&rc);
		ScreenToClient(&rc);

		//调整区域,减小一圈,否则可能超出屏幕
		rc.left += 4;
		rc.top += 4;
		rc.right -= 4;
		rc.bottom -= 4;
	}
	else
	{
		rc = m_rectAllSubWnd;
	}

	GetDlgItem(IDC_STATIC_WNDALL)->MoveWindow(&rc, FALSE);

	//多窗口显示
	int nSubWidth = (rc.Width() - (nSquare + 1) * nInterval) / nSquare;
	int nSubHeight = (rc.Height() - (nSquare + 1) * nInterval) / nSquare;
	for (int i=0; i<nSquare; i++)
	{
		for (int j=0; j<nSquare; j++)
		{
			m_pWndSubVideo[i*nSquare + j]->MoveWindow(rc.left + (j+1)*nInterval + j*nSubWidth, rc.top + (i+1)*nInterval + i*nSubHeight, nSubWidth, nSubHeight);

			if (-1 == nIndex)
			{
				m_pWndSubVideo[i*nSquare + j]->ShowWindow(SW_SHOW);
			}
			else
			{
				if (i*nSquare + j == nIndex)//有放大窗口则放大窗口并显示
				{
					m_pWndSubVideo[nIndex]->MoveWindow(&rc, FALSE);
					m_pWndSubVideo[i*nSquare + j]->ShowWindow(SW_SHOW);
				}
			}
		}
	}

	return;
}

void VideoDlg::UpdateVideoWndUI(void)
{
	//窗口颜色块更新
	if (m_nCurSubWnd != m_nLastSubWnd)
	{
		m_pWndSubVideo[m_nLastSubWnd]->m_color = CVideoWnd::UNACTIVECOLOR;
		m_pWndSubVideo[m_nLastSubWnd]->Invalidate(TRUE);
		m_pWndSubVideo[m_nLastSubWnd]->UpdateWindow();

		m_pWndSubVideo[m_nCurSubWnd]->m_color = CVideoWnd::ACTIVECOLOR;
		m_pWndSubVideo[m_nCurSubWnd]->Invalidate(TRUE);
		m_pWndSubVideo[m_nCurSubWnd]->UpdateWindow();
	}
}

void VideoDlg::ShowAllSubWnd(bool bShow)
{
	CWnd *pWnd = GetWindow(GW_CHILD);
	while (NULL != pWnd)
	{
		if (bShow)
		{
			pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pWnd->ShowWindow(SW_HIDE);
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void VideoDlg::SubWndLButtonDblClk(int nSubNumber)
{
	m_nLastSubWnd = m_nCurSubWnd;
	m_nCurSubWnd = nSubNumber;

	// 在全屏模式下不再支持双击
	if (!m_bFullScreen)
	{
#if 0
		RefreshMultiWnd(3, m_bFullScreen);
		UpdateVideoWndUI();
#else
		//m_rectBeforeFull = m_rectAllSubWnd;

		int nIndex = -1;
		for (int i=0; i<MAX_SUBWND_COUNT; i++)
		{
			if (m_pWndSubVideo[i]->m_bMaxFlag)
			{
				nIndex = i;
			}
		}

		int cx=GetSystemMetrics(SM_CXSCREEN);
		int cy=GetSystemMetrics(SM_CYSCREEN);

		//保存位置信息
		m_currentMax = m_pWndSubVideo[nIndex];
		m_currentMax->GetWindowPlacement(&_temppl);
		m_currentMax->SetParent(NULL); // GetDesktopWindow()
		m_currentMax->SetFocus();

		//修改风格
		m_currentMax->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST); 
		m_currentMax->ModifyStyle(WS_CHILD, WS_POPUP);
		m_currentMax->MoveWindow(0, 0, cx, cy);
		m_bFullScreen = TRUE;
#endif
	}
	else
	{
		QuitFullScreeen();
	}
}

void VideoDlg::QuitFullScreeen(void)
{
	//退出全屏模式
	if (m_bFullScreen)
	{
		m_currentMax->SetParent(this);
		m_currentMax->ModifyStyle(WS_POPUP, WS_CHILD);
		::SetWindowPos(m_currentMax->GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_currentMax->SetWindowPlacement(&_temppl);

		m_bFullScreen = FALSE;
		UpdateVideoWndUI();
		Invalidate(TRUE);
		return;
	}
}

void VideoDlg::SubWndRButtonDown(int nSubNumber)
{
	m_nLastSubWnd = m_nCurSubWnd;
	m_nCurSubWnd = nSubNumber;
	UpdateVideoWndUI();
}

void VideoDlg::SubWndLButtonDown(int nSubNumber)
{
	m_nLastSubWnd = m_nCurSubWnd;
	m_nCurSubWnd = nSubNumber;
	UpdateVideoWndUI();

	if (m_hReal[m_nLastSubWnd] != NULL)
	{
		NvdStopSound(m_hReal[m_nLastSubWnd]);
	}

	if ( ( 0 != m_hRealOpenPara[m_nCurSubWnd].channel) && (m_hReal[m_nCurSubWnd] != NULL) )
	{
		NvdPlaySound(m_hReal[m_nCurSubWnd]);
	}
}

int VideoDlg::OnPreview(NvdHandle hNvd, int winNum)
{
	if (m_hReal[winNum] != NULL)
	{
		int nRet = NvdRealStop(m_hReal[winNum]);
		//if (nRet != 0)
		//{
		//	return nRet;
		//}

		nRet = NvdRealClose(m_hReal[winNum]);
		//if (nRet != 0)
		//{
		//	return nRet;
		//}
		m_hReal[winNum] = NULL;

		UpdateVideoWndUI();
		Invalidate(TRUE);
		UpdateWindow();
	}

	m_hRealOpenPara[winNum].channel = 1;
	m_hRealOpenPara[winNum].subStream = 1;
	m_hRealOpenPara[winNum].transProc = 1;
	m_hRealOpenPara[winNum].ip = 0;


	/// 调用SDK		 
	CVideoWnd* pWnd = m_pWndSubVideo[winNum];

	HWND hPlayWnd = NULL;
	hPlayWnd = pWnd->GetSafeHwnd();

	int nRet = NvdRealOpen(hNvd, &m_hRealOpenPara[winNum], hPlayWnd, this->GetSafeHwnd(), WM_REAL_MSG, &m_hReal[winNum]);
	if ( nRet != 0 || m_hReal == NULL )
	{
		m_hReal[winNum] = NULL;
		return nRet;
	}

	nRet = NvdRealStart(m_hReal[winNum]);
	if ( nRet != 0  )
	{
		m_hReal[winNum] = NULL;
		return nRet;
	}

	//NvdPlayShowAiLang(m_hReal[winNum], 1);
	// 开启声音
	if (0 != m_hRealOpenPara[winNum].channel)
	{
		nRet = NvdPlaySound(m_hReal[winNum]);
	}
	UpdateVideoWndUI();

	NvdRealSetDataCallback( m_hReal[winNum], realDataCallback, NULL );//设置获取原始数据回调
	return 0;
}

BOOL VideoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

int VideoDlg::Stop(int winNum)
{
	int nRet = NvdRealStop(m_hReal[winNum]);
	if (nRet != 0)
	{
		return nRet;
	}

	nRet = NvdRealClose(m_hReal[winNum]);
	if (nRet != 0)
	{
		return nRet;
	}
	m_hReal[winNum] = NULL;
	UpdateVideoWndUI();
	Invalidate(TRUE);
	UpdateWindow();

	return nRet;
}
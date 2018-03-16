// 连连看外挂Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "连连看外挂.h"
#include "连连看外挂Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
	m_qp = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
	DDX_Text(pDX, IDC_EDIT1, m_qp);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONReadchess, OnBUTTONReadchess)
	ON_BN_CLICKED(IDC_BUTTONdelete, OnBUTTONdelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//获取棋盘数据

HANDLE hd;
void CMyDlg::OnBUTTONReadchess() 
{
	UpdateData(true);
	m_qp.Format("");
	LPCSTR GameCaption = "QQ游戏 - 连连看角色版";
	HWND GameHwnd = ::FindWindowA(NULL,GameCaption);
	DWORD DW;

	if(GameHwnd!=NULL)
	{
		::GetWindowThreadProcessId(GameHwnd,&DW);
		hd = OpenProcess(PROCESS_ALL_ACCESS,false,DW);
		byte b[11][19]={0};
		::ReadProcessMemory(hd,(LPCVOID)0x00189F78,&b,11*19,NULL);
		for(int i=0;i<11;i++)
		{
			for(int j=0;j<19;j++)
			{
				CString temp;
				temp.Format("%3.0d",b[i][j]);
				m_qp+=temp+"";
			}
			m_qp+="\r\n";
		}
		UpdateData(false);
	}
	
}

//模拟鼠标点击程序

int x = 35;
int y = 195;
HWND GameHwnd;
int code = 0;
byte b[11][19] = {0};
void kill(int xx,int yy,int num)
{
	for(int ii=0;ii<11;ii++)
	{
		for(int jj=0;jj<19;jj++)
		{
			if(b[ii][jj]!=0 && num == b[ii][jj])
			{
				::PostMessage(GameHwnd,WM_LBUTTONDOWN,0,MAKEWPARAM(xx,yy));
				::PostMessage(GameHwnd,WM_LBUTTONUP,0,MAKEWPARAM(xx,yy));
				::PostMessage(GameHwnd,WM_LBUTTONDOWN,0,MAKEWPARAM(x+jj*30,y+ii*35));
				::PostMessage(GameHwnd,WM_LBUTTONUP,0,MAKEWPARAM(x+jj*30,y+ii*35));
				int te = 0;
				::ReadProcessMemory(hd,(LPCVOID)0x00174DDC,&te,4,NULL);
				if(te < code)
				{
					code = te;
					b[ii][jj] = 0;
					b[xx][yy] = 0;
					Sleep(100);
				}
			}
		}
	}
}


//实现消除

void CMyDlg::OnBUTTONdelete() 
{
	CString GameCaption = "QQ游戏 - 连连看角色版";
	GameHwnd = ::FindWindowA(NULL,GameCaption);
	DWORD DW;
	if(GameHwnd != NULL)
	{
		::GetWindowThreadProcessId(GameHwnd,&DW);
		hd = OpenProcess(PROCESS_ALL_ACCESS,false,DW);
		::ReadProcessMemory(hd,(LPCVOID)0x00174DDC,&code,4,NULL);
		::ReadProcessMemory(hd,(LPCVOID)0x00189F78,&b,11*19,NULL);

		for(int i=0;i<11;i++)
		{
			for(int j=0;j<19;j++)
			{
				if(b[i][j] != 0)
				{
					kill(x+j*30,y+i*35,b[i][j]);
				}
			}
		}
	}	
}

// StringSetSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StringSetSample.h"
#include "StringSetSampleDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
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
// CStringSetSampleDlg dialog

CStringSetSampleDlg::CStringSetSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringSetSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStringSetSampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_strSearch = _T("The quick red fox jumped over the lazy brown dog.  Now is the time for ")
		_T("all good men to come\r\nto the aid of their country.  Ask not what your ")
		_T("country can do for you, but what you can do\r\nfor your country.");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStringSetSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStringSetSampleDlg)
	DDX_Control(pDX, IDC_SEARCH_STRING, m_editSearch);
	DDX_Text(pDX, IDC_STRINGS_TO_SEEK, m_strSearch);
	DDX_Control(pDX, IDC_STRINGS_TO_SEEK, m_editSeekTokens);
	DDX_Control(pDX, IDC_SEEK_TOKENS, m_lbSeekTokens);
	DDX_Control(pDX, IDC_FOUND_TOKENS, m_lbFoundTokens);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStringSetSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CStringSetSampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_KILLFOCUS(IDC_STRINGS_TO_SEEK, OnKillFocusStringsToSeek)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStringSetSampleDlg message handlers

BOOL CStringSetSampleDlg::OnInitDialog()
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
	CRect rectListBoxes;

	m_lbSeekTokens.GetWindowRect(rectListBoxes);

	int nColWid = rectListBoxes.Width() / 3;

	m_lbFoundTokens.SetColumnWidth(nColWid);
	m_lbFoundTokens.SetColumnWidth(nColWid);
	m_editSeekTokens.SetFocus();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStringSetSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CStringSetSampleDlg::OnPaint() 
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
HCURSOR CStringSetSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStringSetSampleDlg::OnKillFocusStringsToSeek()
{
	m_lbSeekTokens.ResetContent();
	m_setStrings.clear();

	CString strTokens;

	m_editSeekTokens.GetWindowText(strTokens);

	int nCount = m_setStrings.Add(strTokens, _T(", |;"));

	if( nCount > 0 )
	{
		CIVStringSet::iterator it = m_setStrings.begin();

		while( it != m_setStrings.end() )
		{
			_tstring str = *it++;

			m_lbSeekTokens.AddString(str.c_str());
		}
	}
}

void CStringSetSampleDlg::OnSearch()
{
	m_lbFoundTokens.ResetContent();
	m_editSearch.GetWindowText(m_strSearch);

	size_t nWord;
	UINT nPos = m_setStrings.FindFirstIn(static_cast<LPCTSTR>(m_strSearch), nWord);

	if( nPos != MAXDWORD )
	{
		CString strWord;

		do {
			strWord.Format(_T("%s (%u)"), m_setStrings[nWord].c_str(), nPos);
			m_lbFoundTokens.AddString(strWord);
			nPos = m_setStrings.FindNext(nWord);
		}while( nPos != MAXDWORD );
	}
}

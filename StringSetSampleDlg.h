// StringSetSampleDlg.h : header file
//

#if !defined(AFX_STRINGSETSAMPLEDLG_H__A028E814_CCC7_446D_AAA5_4A0BCC937723__INCLUDED_)
#define AFX_STRINGSETSAMPLEDLG_H__A028E814_CCC7_446D_AAA5_4A0BCC937723__INCLUDED_

#include "StringSet.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStringSetSampleDlg dialog

class CStringSetSampleDlg : public CDialog
{
// Construction
public:
	CStringSetSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStringSetSampleDlg)
	enum { IDD = IDD_STRINGSETSAMPLE_DIALOG };
	CEdit m_editSearch;
	CString m_strSearch;
	CEdit m_editSeekTokens;
	CListBox m_lbSeekTokens;
	CListBox m_lbFoundTokens;
	//}}AFX_DATA
	CIVStringSet m_setStrings;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringSetSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStringSetSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnKillFocusStringsToSeek();
	afx_msg void OnSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGSETSAMPLEDLG_H__A028E814_CCC7_446D_AAA5_4A0BCC937723__INCLUDED_)

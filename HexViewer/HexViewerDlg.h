
// HexViewerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
typedef struct stPatData
{
    int size;
    UCHAR *data;
}stPatData, *pstPatData;



// CHexViewerDlg dialog
class CHexViewerDlg : public CDialogEx
{
// Construction
public:
	CHexViewerDlg(CWnd* pParent = NULL);	// standard constructor
    ~CHexViewerDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEXVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnEnChangeMfcFileName();
    DECLARE_MESSAGE_MAP()

    stPatData   fdata;
    CFont *     font;
    CString     m_FileName;
    CString     m_Hex;
    int         m_Size;
    int         m_FileSize;
    int         LoadFileToScan(char *fname, pstPatData pData);
    CString     createHexBuffer(UCHAR *data, int size);
public:
    afx_msg void OnBnClickedOk();
    void    freeAll();
};

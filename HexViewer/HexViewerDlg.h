
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

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HexViewer_DIALOG };
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
	DECLARE_MESSAGE_MAP()

    stPatData fdata;
public:
    CString     m_FileName;
    CString     m_Hex;
    int         m_Size;
    int         m_FileSize;
    afx_msg void OnEnChangeMfcFileName();
    afx_msg void OnBnClickedOk();
    void    freeAll();
    int     LoadFileToScan(char *fname, pstPatData pData);
    CString createHexBuffer(UCHAR *data, int size);
};

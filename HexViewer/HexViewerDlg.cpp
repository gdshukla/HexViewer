
// HexViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexViewer.h"
#include "HexViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHexViewerDlg dialog



CHexViewerDlg::CHexViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HEXVIEWER_DIALOG, pParent)
    , m_FileName(_T(""))
    , m_Hex(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    font = NULL;

    m_Size = 0;
    m_FileSize = 0;

}
CHexViewerDlg::~CHexViewerDlg()
{
    freeAll();
    m_FileName.Empty();
    if (font != NULL)
    {
        delete font;
        font = NULL;
    }

}
void CHexViewerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MFC_FILE_NAME, m_FileName);
    DDX_Text(pDX, IDC_EDIT_HEX, m_Hex);
}

BEGIN_MESSAGE_MAP(CHexViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_EN_CHANGE(IDC_MFC_FILE_NAME, &CHexViewerDlg::OnEnChangeMfcFileName)
    ON_BN_CLICKED(IDOK, &CHexViewerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHexViewerDlg message handlers

BOOL CHexViewerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    fdata = { 0 };
    font = new CFont();
    font->CreateFont(14, 0, 0, 0, FW_NORMAL, false, false,
        0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, _T("Consolas"));
    GetDlgItem(IDC_EDIT_HEX)->SetFont(font);
    GetDlgItem(IDC_STATIC_HEADER)->SetFont(font);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHexViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHexViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define     TEXT_WIDTH      24

int CHexViewerDlg::LoadFileToScan(char *fname, pstPatData pData)
{

    DWORD	cbSize;
    char *	testVirFName = fname;
    DWORD	cbRead;
    UCHAR * dbBuffer;
    CString cs;

    HANDLE	hFile;
    hFile = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != 0)
    {
        // Read in the complete file -- it should be pretty short
        cbSize = GetFileSize(hFile, NULL);
        if (!cbSize || cbSize == INVALID_FILE_SIZE)
        {
            return ERROR_INVALID_DATA;
        }
        else
        {
            m_FileSize = cbSize;
            m_Size = (cbSize / TEXT_WIDTH + 1) * TEXT_WIDTH;      // make it divisible by 16, extended to next block
            dbBuffer = new UCHAR[m_Size];
            memset(dbBuffer, 0x00, m_Size);
            if (!dbBuffer)
            {
                return ERROR_NOT_ENOUGH_MEMORY;
            }
            else
            {
                if (!ReadFile(hFile, dbBuffer, cbSize, &cbRead, NULL))
                {
                    return GetLastError();
                }
                else
                {
                    pData->data = dbBuffer;
                    pData->size = m_Size;
                }
            }
        }
        CloseHandle(hFile);
    }

    return ERROR_SUCCESS;

}

CString CHexViewerDlg::createHexBuffer(UCHAR *data, int size)
{
    CString cs, csHex;
	CString csText;
    CString csTotal;
    for (int i = 0; i < size/ TEXT_WIDTH; i++)
    {
        cs.Format("%06d:   ", i*TEXT_WIDTH);
        csHex += cs;

        for (int j = 0; j < TEXT_WIDTH; j++)
        {
            cs.Format("%02x ", data[i*TEXT_WIDTH + j]);
            csHex += cs;
            char ch = data[i*TEXT_WIDTH + j];
            if (ch == '\r' || ch == '\n')
            {
                csText += '\'';
            }
            else if (ch < 31 ||  ch > 128 )
            {
                csText += '.';
            }
            else
            {
                csText += data[i*TEXT_WIDTH + j];
            }
        }
        csTotal += csHex + "   " + csText + "\r\n";
		csHex.Empty();
		csText.Empty();
     
    }
    return csTotal;
}
void CHexViewerDlg::freeAll()
{
    if (fdata.data != NULL)
    {
        delete fdata.data;
        fdata.data = NULL;
    }
    m_Hex.Empty();

}


void CHexViewerDlg::OnEnChangeMfcFileName()
{
    UpdateData(true);
    freeAll();
    char *fname = m_FileName.GetBuffer();
    fdata = { 0 };
    CString cs;
    cs = "Offset     00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23       Text Data";
    GetDlgItem(IDC_STATIC_HEADER)->SetWindowTextA(cs);

    LoadFileToScan(fname, &fdata);
    cs.Format("Size: %d", m_FileSize);      // display size
    GetDlgItem(IDC_STATIC_FILE_SIZE)->SetWindowTextA(cs);
//    UpdateData(false);


    cs.Format("  \\r &&\\n displayed as  (.)  |  < 31 && > 128 displayed as (') ");      // display size
    GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowTextA(cs);

    
    m_Hex = createHexBuffer(fdata.data, fdata.size);    // load file data
//    m_text = cs;
    UpdateData(false);

}


void CHexViewerDlg::OnBnClickedOk()
{
    freeAll();
    CDialogEx::OnOK();
}

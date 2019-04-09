#pragma once


// MainWindow dialog

class MainWindow : public CDialog
{
	DECLARE_DYNAMIC(MainWindow)

public:
	MainWindow(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MainWindow();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainWnd };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

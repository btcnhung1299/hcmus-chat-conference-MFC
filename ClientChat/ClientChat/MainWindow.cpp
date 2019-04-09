// MainWindow.cpp : implementation file
//

#include "stdafx.h"
#include "ClientChat.h"
#include "MainWindow.h"
#include "afxdialogex.h"


// MainWindow dialog

IMPLEMENT_DYNAMIC(MainWindow, CDialog)

MainWindow::MainWindow(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MainWnd, pParent)
{

}

MainWindow::~MainWindow()
{
}

void MainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainWindow, CDialog)
END_MESSAGE_MAP()


// MainWindow message handlers

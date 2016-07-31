
// DigitalImageProcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProc.h"
#include "DigitalImageProcDlg.h"
#include "afxdialogex.h"

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDigitalImageProcDlg 对话框



CDigitalImageProcDlg::CDigitalImageProcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDigitalImageProcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDigitalImageProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDigitalImageProcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CDigitalImageProcDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_VIDEO, &CDigitalImageProcDlg::OnBnClickedVideo)
END_MESSAGE_MAP()


// CDigitalImageProcDlg 消息处理程序

BOOL CDigitalImageProcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	//关联OpenCV窗口与Picture控件
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDigitalImageProcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDigitalImageProcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDigitalImageProcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDigitalImageProcDlg::OnBnClickedOpen()
{
	// TODO:  在此添加控件通知处理程序代码

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	dlg.m_ofn.lpstrTitle = _T("打开图像文件"); //对话框标题
	dlg.m_ofn.lpstrInitialDir = "F:\\"; //默认打开路径
	dlg.m_ofn.lpstrFilter = "bmp (*.bmp)\0*.bmp\0 jpg (*.jpg)\0*.jpg\0 All Files (*.*) \0*.*\0\0"; //打开文件类型
	
	if (dlg.DoModal() != IDOK)             // 判断是否获得图片         
		return;
	m_path = dlg.GetPathName();



	
	//多字节字符集下 CString 转 char*  (LPSTR)(LPCSTR)
	src = imread((LPSTR)(LPCSTR)m_path);

	/****** Unicode字符集下 CString 转 char*  ******/
	/*
	int nLength = m_path.GetLength();
	int nBytes = WideCharToMultiByte(CP_ACP, 0, m_path, nLength, NULL, 0, NULL, NULL);
	char* VoicePath = new char[nBytes + 1];
	memset(VoicePath, 0, nLength + 1);
	WideCharToMultiByte(CP_OEMCP, 0, m_path, nLength, VoicePath, nBytes, NULL, NULL);
	VoicePath[nBytes] = 0;
	src = imread(VoicePath);
	*/
	/************************************************/

	Mat img;  //保存缩放后的图像
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect); //获取图像显示区

	resize(src, img, Size(rect.Width(), rect.Height()), 0, 0);
		
	imshow("view", img);
}


void CDigitalImageProcDlg::OnBnClickedVideo()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	dlg.m_ofn.lpstrTitle = _T("打开视频文件"); //对话框标题
	dlg.m_ofn.lpstrInitialDir = "F:\\"; //默认打开路径
	dlg.m_ofn.lpstrFilter = "avi (*.avi)\0*.avi\0 All Files (*.*) \0*.*\0\0"; //打开文件类型

	if (dlg.DoModal() != IDOK)             // 判断是否获得图片         
		return;
	m_path = dlg.GetPathName();

	VideoCapture capture((LPSTR)(LPCSTR)m_path);
	//0 open default camera
	//VideoCapture capture(0);
	//检查视频是否打开
	if (!capture.isOpened())
	{
		MessageBox(_T("Can't open video!"));
	}
	else
	{
		// 得到帧率
		double rate = capture.get(CV_CAP_PROP_FPS);
		bool stop(false);
		Mat frame; // 现在的视频帧
		Mat img;//保存缩放后的图像
		CRect rect;
		GetDlgItem(IDC_PIC)->GetClientRect(&rect);

		// 两帧之间的间隔时间
		//    int delay= 1000/rate;
		// 循环播放所有的帧
		while (!stop)
		{
			// 读下一帧
			if (!capture.read(frame))
				break;
			//在窗口中显示图像
			
			resize(frame, img, Size(rect.Width(), rect.Height()), 0, 0);
			imshow("view", img);
			// 按任意键停止视频播放
			//	if (waitKey(500)>=0)
			//	    stop= true;
			waitKey(300);
		}
		// 关闭视频文件
			
		capture.release();
	}
}

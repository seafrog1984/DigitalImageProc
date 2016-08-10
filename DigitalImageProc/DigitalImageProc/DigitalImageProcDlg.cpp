
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
	, m_width(0)
	, m_height(0)
	, m_angle(0)
	, m_shear_ratio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDigitalImageProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDX_Text(pDX, IDC_ANGLE, m_angle);
	DDX_Text(pDX, IDC_SHEAR_RATIO, m_shear_ratio);
}

BEGIN_MESSAGE_MAP(CDigitalImageProcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CDigitalImageProcDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_VIDEO, &CDigitalImageProcDlg::OnBnClickedVideo)
	ON_BN_CLICKED(IDC_RESIZE, &CDigitalImageProcDlg::OnBnClickedResize)
	ON_BN_CLICKED(IDC_ROTATION, &CDigitalImageProcDlg::OnBnClickedRotation)
	ON_BN_CLICKED(IDC_LINEAR, &CDigitalImageProcDlg::OnBnClickedLinear)
	ON_BN_CLICKED(IDC_HSHEAR, &CDigitalImageProcDlg::OnBnClickedHshear)
	ON_BN_CLICKED(IDC_VSHEAR, &CDigitalImageProcDlg::OnBnClickedVshear)
	ON_BN_CLICKED(IDC_HMIRROR, &CDigitalImageProcDlg::OnBnClickedHmirror)
	ON_BN_CLICKED(IDC_VMIRROR, &CDigitalImageProcDlg::OnBnClickedVmirror)
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

	//【1a】从视频文件载入视频 
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	dlg.m_ofn.lpstrTitle = _T("打开视频文件"); //对话框标题
	dlg.m_ofn.lpstrInitialDir = "F:\\"; //默认打开路径
	dlg.m_ofn.lpstrFilter = "avi (*.avi)\0*.avi\0 All Files (*.*) \0*.*\0\0"; //打开文件类型

	if (dlg.DoModal() != IDOK)             // 判断是否获得图片         
		return;
	m_path = dlg.GetPathName();

	VideoCapture capture;
	capture.open((LPSTR)(LPCSTR)m_path);
 
	//【1b】从本地安装的摄像头载入视频 0 open default camera
	//	capture.open(0);
	//【1c】从网络摄像头载入视频  
	//capture.open("rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp"); //公开的RTSP测试服务器  
	//capture.open("rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp"); //公开的RTSP测试服务器 拱北口岸珠海过澳门大厅  
	//capture.open("rtsp://218.204.223.237:554/live/1/67A7572844E51A64/f68g2mj7wjua3la7.sdp"); //公开的RTSP测试服务器 横琴口岸入境大厅  


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
			
			imshow("Video", frame);
			// 按任意键停止视频播放
				if (waitKey(300)>=0)
				    stop= true;
		}
		// 关闭视频文件
		
		capture.release();
	}
}


void CDigitalImageProcDlg::OnBnClickedResize()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);

	Mat dst;

	resize(src, dst, Size(m_width, m_height), 0, 0);

	imshow("缩放后图像", dst);

}


void CDigitalImageProcDlg::OnBnClickedRotation()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	Mat rot_mat(2, 3, CV_32FC1);//旋转变换矩阵
	Mat rotate_dst;
	
	// 计算旋转后图像尺寸
	double a = sin(m_angle  * CV_PI / 180);
	double b = cos(m_angle  * CV_PI / 180);
	int width = src.cols;
	int height = src.rows;
	int width_rotate = int(height * fabs(a) + width * fabs(b));
	int height_rotate = int(width * fabs(a) + height * fabs(b));

	//计算图像旋转变换矩阵
	Point center = Point(src.cols / 2, src.rows / 2);
	double angle = m_angle;
	double scale = 1;
	rot_mat = getRotationMatrix2D(center, angle, scale); 

	// 修改坐标偏移
	rot_mat.at<double>(0, 2) += (width_rotate - width) / 2;    
	rot_mat.at<double>(1, 2) += (height_rotate - height) / 2; 

	/// 旋转图像
	warpAffine(src, rotate_dst, rot_mat, Size(width_rotate, height_rotate));

	namedWindow("旋转后图像");
	imshow("旋转后图像", rotate_dst);

}


void CDigitalImageProcDlg::OnBnClickedLinear()
{
	// TODO:  在此添加控件通知处理程序代码
	Mat g_src, dst;
	
	cvtColor(src, g_src, CV_BGR2GRAY);

	dst.create(src.size(), g_src.type());

	for (int i = 0; i<g_src.rows; i++)//线性拉伸
	{
	
		for (int j = 0; j<g_src.cols; j++)
		{
			int tmp = g_src.at<uchar>(i,j);

			if (tmp<64)
			{
				dst.at<uchar>(i,j) = tmp / 2;
			}
			else if (tmp<192)
			{
				dst.at<uchar>(i, j) = tmp + tmp / 2;
			}
			else
			{
				dst.at<uchar>(i, j) = tmp / 2;
			}
		}
	}

	namedWindow("线性拉伸");
	imshow("线性拉伸", dst);
}


void CDigitalImageProcDlg::OnBnClickedHshear()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//计算错切后图像大小
	double ratio = m_shear_ratio;
	int dst_wid;
	int dst_hei;
	
	if (ratio < 0)
	{
		dst_wid = src.cols + src.rows*ratio*(-1);
	}
	else 
	{
		dst_wid = src.cols + src.rows*ratio;
	}

	dst_hei = src.rows;

	Mat dst;

	dst.create(Size(dst_wid, dst_hei), src.type());

	switch (src.channels())
	{
	case 1:
		for (int i = 0; i<src.rows; i++)
		{
			if (ratio >= 0)
			for (int j = 0; j<src.cols; j++)
			{
				dst.at<uchar>(i, j + i*ratio) = src.at<uchar>(i, j);
			}
			else
			{
				int offset = (-1)*ratio*src.rows;
				for (int j = src.cols - 1; j >= 0; j--)
				{
					dst.at<uchar>(i, j + i*ratio + offset) = src.at<uchar>(i, j);
				}
			}
		}
	case 3:
		for (int i = 0; i<src.rows; i++)
		{
			if (ratio >= 0)
			for (int j = 0; j<src.cols; j++)
			{
				dst.at<Vec3b>(i, j + i*ratio) = src.at<Vec3b>(i, j);
			}
			else
			{
				int offset = (-1)*ratio*src.rows;
				for (int j = src.cols - 1; j >= 0; j--)
				{
					dst.at<Vec3b>(i, j + i*ratio + offset) = src.at<Vec3b>(i, j);
				}
			}
		}
	default:
		break;
	}
	
	namedWindow("水平错切");
	imshow("水平错切", dst);

}


void CDigitalImageProcDlg::OnBnClickedVshear()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//计算错切后图像大小
	double ratio = m_shear_ratio;
	int dst_wid;
	int dst_hei;

	if (ratio < 0)
	{
		dst_hei = src.rows + src.cols*ratio*(-1);
	}
	else
	{
		dst_hei = src.rows + src.cols*ratio;
	}

	dst_wid = src.cols;

	Mat dst;

	dst.create(Size(dst_wid, dst_hei), src.type());

	switch (src.channels())
	{
	case 1:
		for (int i = 0; i<src.rows; i++)
		{
			if (ratio >= 0)
			for (int j = 0; j<src.cols; j++)
			{
				dst.at<uchar>(i + j*ratio, j) = src.at<uchar>(i, j);
			}
			else
			{
				int offset = (-1)*ratio*src.rows;
				for (int j = src.cols - 1; j >= 0; j--)
				{
					dst.at<uchar>(i + j*ratio + offset, j) = src.at<uchar>(i, j);
				}
			}
		}
	case 3:
		for (int i = 0; i<src.rows; i++)
		{
			if (ratio >= 0)
			for (int j = 0; j<src.cols; j++)
			{
				dst.at<Vec3b>(i + j*ratio, j) = src.at<Vec3b>(i, j);
			}
			else
			{
				int offset = (-1)*ratio*src.rows;
				for (int j = src.cols - 1; j >= 0; j--)
				{
					dst.at<Vec3b>(i + j*ratio + offset, j) = src.at<Vec3b>(i, j);
				}
			}
		}
	default:
		break;
	}

	namedWindow("垂直错切");
	imshow("垂直错切", dst);
}


void CDigitalImageProcDlg::OnBnClickedHmirror()
{
	// TODO:  在此添加控件通知处理程序代码

	Mat dst;

	dst.create(src.rows, src.cols, src.type());

	int rows = src.rows;
	int cols = src.cols;

	switch (src.channels())
	{
	case 1:
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				dst.at<uchar>(i,j) = src.at<uchar>(i,cols - 1 - j);
			}
		}
		break;
	case 3:
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				dst.at<Vec3b>(i,j) = src.at<Vec3b>(i,cols - 1 - j);
			}
		}
		break;
	default:
		break;
	}

	namedWindow("水平镜像");
	imshow("水平镜像", dst);
}


void CDigitalImageProcDlg::OnBnClickedVmirror()
{
	// TODO:  在此添加控件通知处理程序代码
	Mat dst;

	dst.create(src.rows, src.cols, src.type());

	int rows = src.rows;
	int cols = src.cols;

	switch (src.channels())
	{
	case 1:
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				dst.at<uchar>(i, j) = src.at<uchar>(rows-1-i, j);
			}
		}
		break;
	case 3:
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				dst.at<Vec3b>(i, j) = src.at<Vec3b>(rows-1-i, j);
			}
		}
		break;
	default:
		break;
	}

	namedWindow("垂直镜像");
	imshow("垂直镜像", dst);
}

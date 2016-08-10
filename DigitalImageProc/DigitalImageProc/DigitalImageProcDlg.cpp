
// DigitalImageProcDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDigitalImageProcDlg �Ի���



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


// CDigitalImageProcDlg ��Ϣ�������

BOOL CDigitalImageProcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	//����OpenCV������Picture�ؼ�
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDigitalImageProcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDigitalImageProcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDigitalImageProcDlg::OnBnClickedOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	dlg.m_ofn.lpstrTitle = _T("��ͼ���ļ�"); //�Ի������
	dlg.m_ofn.lpstrInitialDir = "F:\\"; //Ĭ�ϴ�·��
	dlg.m_ofn.lpstrFilter = "bmp (*.bmp)\0*.bmp\0 jpg (*.jpg)\0*.jpg\0 All Files (*.*) \0*.*\0\0"; //���ļ�����
	
	if (dlg.DoModal() != IDOK)             // �ж��Ƿ���ͼƬ         
		return;
	m_path = dlg.GetPathName();



	
	//���ֽ��ַ����� CString ת char*  (LPSTR)(LPCSTR)
	src = imread((LPSTR)(LPCSTR)m_path);

	/****** Unicode�ַ����� CString ת char*  ******/
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

	Mat img;  //�������ź��ͼ��
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect); //��ȡͼ����ʾ��

	resize(src, img, Size(rect.Width(), rect.Height()), 0, 0);
		
	imshow("view", img);
}


void CDigitalImageProcDlg::OnBnClickedVideo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��1a������Ƶ�ļ�������Ƶ 
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	dlg.m_ofn.lpstrTitle = _T("����Ƶ�ļ�"); //�Ի������
	dlg.m_ofn.lpstrInitialDir = "F:\\"; //Ĭ�ϴ�·��
	dlg.m_ofn.lpstrFilter = "avi (*.avi)\0*.avi\0 All Files (*.*) \0*.*\0\0"; //���ļ�����

	if (dlg.DoModal() != IDOK)             // �ж��Ƿ���ͼƬ         
		return;
	m_path = dlg.GetPathName();

	VideoCapture capture;
	capture.open((LPSTR)(LPCSTR)m_path);
 
	//��1b���ӱ��ذ�װ������ͷ������Ƶ 0 open default camera
	//	capture.open(0);
	//��1c������������ͷ������Ƶ  
	//capture.open("rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp"); //������RTSP���Է�����  
	//capture.open("rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp"); //������RTSP���Է����� �����ڰ��麣�����Ŵ���  
	//capture.open("rtsp://218.204.223.237:554/live/1/67A7572844E51A64/f68g2mj7wjua3la7.sdp"); //������RTSP���Է����� ���ٿڰ��뾳����  


	//�����Ƶ�Ƿ��
	if (!capture.isOpened())
	{
		MessageBox(_T("Can't open video!"));
	}
	else
	{
		// �õ�֡��
		double rate = capture.get(CV_CAP_PROP_FPS);
		bool stop(false);
		Mat frame; // ���ڵ���Ƶ֡
		Mat img;//�������ź��ͼ��
		CRect rect;
		GetDlgItem(IDC_PIC)->GetClientRect(&rect);

		// ��֮֡��ļ��ʱ��
		//    int delay= 1000/rate;
		// ѭ���������е�֡
		while (!stop)
		{
			// ����һ֡
			if (!capture.read(frame))
				break;
			//�ڴ�������ʾͼ��
			
			imshow("Video", frame);
			// �������ֹͣ��Ƶ����
				if (waitKey(300)>=0)
				    stop= true;
		}
		// �ر���Ƶ�ļ�
		
		capture.release();
	}
}


void CDigitalImageProcDlg::OnBnClickedResize()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	Mat dst;

	resize(src, dst, Size(m_width, m_height), 0, 0);

	imshow("���ź�ͼ��", dst);

}


void CDigitalImageProcDlg::OnBnClickedRotation()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	Mat rot_mat(2, 3, CV_32FC1);//��ת�任����
	Mat rotate_dst;
	
	// ������ת��ͼ��ߴ�
	double a = sin(m_angle  * CV_PI / 180);
	double b = cos(m_angle  * CV_PI / 180);
	int width = src.cols;
	int height = src.rows;
	int width_rotate = int(height * fabs(a) + width * fabs(b));
	int height_rotate = int(width * fabs(a) + height * fabs(b));

	//����ͼ����ת�任����
	Point center = Point(src.cols / 2, src.rows / 2);
	double angle = m_angle;
	double scale = 1;
	rot_mat = getRotationMatrix2D(center, angle, scale); 

	// �޸�����ƫ��
	rot_mat.at<double>(0, 2) += (width_rotate - width) / 2;    
	rot_mat.at<double>(1, 2) += (height_rotate - height) / 2; 

	/// ��תͼ��
	warpAffine(src, rotate_dst, rot_mat, Size(width_rotate, height_rotate));

	namedWindow("��ת��ͼ��");
	imshow("��ת��ͼ��", rotate_dst);

}


void CDigitalImageProcDlg::OnBnClickedLinear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Mat g_src, dst;
	
	cvtColor(src, g_src, CV_BGR2GRAY);

	dst.create(src.size(), g_src.type());

	for (int i = 0; i<g_src.rows; i++)//��������
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

	namedWindow("��������");
	imshow("��������", dst);
}


void CDigitalImageProcDlg::OnBnClickedHshear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	//������к�ͼ���С
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
	
	namedWindow("ˮƽ����");
	imshow("ˮƽ����", dst);

}


void CDigitalImageProcDlg::OnBnClickedVshear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	//������к�ͼ���С
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

	namedWindow("��ֱ����");
	imshow("��ֱ����", dst);
}


void CDigitalImageProcDlg::OnBnClickedHmirror()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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

	namedWindow("ˮƽ����");
	imshow("ˮƽ����", dst);
}


void CDigitalImageProcDlg::OnBnClickedVmirror()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	namedWindow("��ֱ����");
	imshow("��ֱ����", dst);
}

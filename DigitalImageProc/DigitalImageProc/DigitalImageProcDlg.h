
// DigitalImageProcDlg.h : 头文件
//

#pragma once

#include<opencv2/core/core.hpp>

using namespace cv;

// CDigitalImageProcDlg 对话框
class CDigitalImageProcDlg : public CDialogEx
{
// 构造
public:
	CDigitalImageProcDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DIGITALIMAGEPROC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();

public:
	
	CString m_path;
	Mat src;


	afx_msg void OnBnClickedVideo();
	int m_width;
	int m_height;
	afx_msg void OnBnClickedResize();
	double m_angle;
	afx_msg void OnBnClickedRotation();
	afx_msg void OnBnClickedLinear();
	afx_msg void OnBnClickedHshear();
	double m_shear_ratio;
	afx_msg void OnBnClickedVshear();
};

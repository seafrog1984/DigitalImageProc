
// DigitalImageProcDlg.h : ͷ�ļ�
//

#pragma once

#include<opencv2/core/core.hpp>

using namespace cv;

// CDigitalImageProcDlg �Ի���
class CDigitalImageProcDlg : public CDialogEx
{
// ����
public:
	CDigitalImageProcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DIGITALIMAGEPROC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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


};

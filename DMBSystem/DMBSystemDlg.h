
// DMBSystemDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
//#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc_c.h"

// CDMBSystemDlg �Ի���
class CDMBSystemDlg : public CDialogEx
{
// ����
public:
	CDMBSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void Display(IplImage * img, UINT ID);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DMBSYSTEM_DIALOG };
#endif

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
	


	afx_msg void OnBnClickedStartvideo();


	afx_msg void OnBnClickedStartwebcam();
	afx_msg void OnBnClickedStartpicture();
	afx_msg void OnBnClickedFindpath();
};

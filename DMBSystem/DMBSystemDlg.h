
// DMBSystemDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
//#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc_c.h"

// CDMBSystemDlg 对话框
class CDMBSystemDlg : public CDialogEx
{
// 构造
public:
	CDMBSystemDlg(CWnd* pParent = NULL);	// 标准构造函数
	void Display(IplImage * img, UINT ID);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DMBSYSTEM_DIALOG };
#endif

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
	


	afx_msg void OnBnClickedStartvideo();


	afx_msg void OnBnClickedStartwebcam();
	afx_msg void OnBnClickedStartpicture();
	afx_msg void OnBnClickedFindpath();
};


// DMBSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DMBSystem.h"
#include "DMBSystemDlg.h"
#include "afxdialogex.h"
#include <afxdlgs.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "kcftracker.hpp"


using namespace std;
using namespace cv;

//画矩形框

Rect selection;
bool selectObject = false;
Point origin;
Mat frame, img;
bool flag = false;
bool pause = false;
CString strr;

static void onMouse(int event, int x, int y, int, void*)
{
	if (!flag&&pause)
	{
		if (selectObject)
		{
			selection.x = MIN(x, origin.x);
			selection.y = MIN(y, origin.y);
			selection.width = std::abs(x - origin.x);
			selection.height = std::abs(y - origin.y);
			selection &= Rect(0, 0, frame.cols, frame.rows);
		}

		switch (event)
		{
		case CV_EVENT_LBUTTONDOWN:
			origin = Point(x, y);
			selection = Rect(x, y, 0, 0);
			selectObject = true;
			break;
		case CV_EVENT_LBUTTONUP:
			selectObject = false;
			if (selection.width > 0 && selection.height > 0)
				rectangle(img, selection, Scalar(0, 255, 255));
			imshow("Image", img);
			flag = true;
			break;
		}
	}
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDMBSystemDlg 对话框



CDMBSystemDlg::CDMBSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DMBSYSTEM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDMBSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


}

BEGIN_MESSAGE_MAP(CDMBSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_STARTVIDEO, &CDMBSystemDlg::OnBnClickedStartvideo)
	ON_BN_CLICKED(IDC_STARTWEBCAM, &CDMBSystemDlg::OnBnClickedStartwebcam)
	ON_BN_CLICKED(IDC_STARTPICTURE, &CDMBSystemDlg::OnBnClickedStartpicture)
	ON_BN_CLICKED(IDC_FINDPATH, &CDMBSystemDlg::OnBnClickedFindpath)
END_MESSAGE_MAP()


// CDMBSystemDlg 消息处理程序

BOOL CDMBSystemDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDMBSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDMBSystemDlg::OnPaint()
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
HCURSOR CDMBSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CDMBSystemDlg::OnBnClickedStartvideo()
{
	// TODO: 在此添加控件通知处理程序代码
	
	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;



	// Create KCFTracker object// 创建KCF跟踪器
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
	// Tracker results 跟踪结果目标框
	Rect result;

	string path;
	path = CT2A(strr);

	VideoCapture cap;
	cap.open(path);


	namedWindow("Image", 2);
	setMouseCallback("Image", onMouse, 0);
	int nFrames = 0;


	for (;;)
	{
		if (!pause)
		{
			cap  >> frame;
			if (frame.empty())
				break;
			frame.copyTo(img);
			if (flag)
			{
				result = tracker.update(img);
				//cout << "X坐标:" << result.x << "  " << "Y的坐标：" << result.y << endl;
				//cout << "左上X坐标:" << result.x << "  " << "左上角Y坐标：" << result.y << "  " << "右上角X坐标:：" << result.x + result.width << "  " << "右上Y角坐标:：" << result.y << endl;
				//cout << "左下X坐标:" << result.x << "  " << "左下角Y坐标：" << result.y + result.height << "  " << "右下角X坐标:：" << result.x + result.width << "  " << "右下Y角坐标:：" << result.y + result.height << endl;

				rectangle(img, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
				//write.write(img); //将帧存为视频
				imshow("Image", img);

				nFrames++;
			}
		}
		else
		{
			if (flag&&nFrames == 0)
			{
				tracker.init(selection, img);//Rect(xMin, yMin, width, height)
			}
		}
		imshow("Image", img);

		char c = (char)waitKey(10);
		if (c == 27)
			break;
		switch (c)
		{
		case 'p':
			pause = !pause;
			break;
		case 'r':
			flag = false;
			nFrames = 0;
			frame.copyTo(img);
			imshow("Image", img);
			break;
		}

		if (!cvGetWindowHandle("Image"))

		{
			break;
		}

	}

}



void CDMBSystemDlg::OnBnClickedStartwebcam()
{
	// TODO: 在此添加控件通知处理程序代码

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;


	// Create KCFTracker object// 创建KCF跟踪器
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
	// Tracker results 跟踪结果目标框
	Rect result;
	VideoCapture cap;
	cap.open(0);

	namedWindow("Image", 2);
	setMouseCallback("Image", onMouse, 0);
	int nFrames = 0;


	for (;;)
	{
		if (!pause)
		{
			cap >> frame;

			if (frame.empty())
				break;
			frame.copyTo(img);
			if (flag)
			{
				result = tracker.update(img);
				//cout << "X坐标:" << result.x << "  " << "Y的坐标：" << result.y << endl;
				//cout << "左上X坐标:" << result.x << "  " << "左上角Y坐标：" << result.y << "  " << "右上角X坐标:：" << result.x + result.width << "  " << "右上Y角坐标:：" << result.y << endl;
				//cout << "左下X坐标:" << result.x << "  " << "左下角Y坐标：" << result.y + result.height << "  " << "右下角X坐标:：" << result.x + result.width << "  " << "右下Y角坐标:：" << result.y + result.height << endl;

				rectangle(img, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
				//write.write(img); //将帧存为视频
				imshow("Image", img);

				nFrames++;
			}
		}
		else
		{
			if (flag&&nFrames == 0)
			{
				tracker.init(selection, img);//Rect(xMin, yMin, width, height)
			}
		}
		imshow("Image", img);

		char c = (char)waitKey(10);
		if (c == 27)
			break;
		switch (c)
		{
		case 'p':
			pause = !pause;
			break;
		case 'r':
			flag = false;
			nFrames = 0;
			frame.copyTo(img);
			imshow("Image", img);
			break;
		}

		if (!cvGetWindowHandle("Image"))

		{
			break;
		}

	}

}


void CDMBSystemDlg::OnBnClickedStartpicture()
{
	// TODO: 在此添加控件通知处理程序代码

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = false;
	bool LAB = false;


	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// Frame readed
	Mat frame;

	// Tracker results
	Rect result;

	// Path to list.txt
	ifstream listFile;
	string fileName = "D:\\kcf\\DMBSystem\\images.txt";
	listFile.open(fileName);

	// Read groundtruth for the 1st frame
	ifstream groundtruthFile;
	string groundtruth = "D:\\kcf\\DMBSystem\\region.txt";
	groundtruthFile.open(groundtruth);
	string firstLine;
	getline(groundtruthFile, firstLine);
	groundtruthFile.close();

	istringstream ss(firstLine);

	// Read groundtruth like a dumb
	float xMin, yMin, width, height;
	char ch;
	ss >> xMin;
	ss >> ch;
	ss >> yMin;
	ss >> ch;
	ss >> width;
	ss >> ch;
	ss >> height;


	// Read Images
	ifstream listFramesFile;
	string listFrames = "D:\\kcf\\DMBSystem\\images.txt";
	listFramesFile.open(listFrames);
	string frameName;


	// Write Results
	ofstream resultsFile;
	string resultsPath = "output.txt";
	resultsFile.open(resultsPath);

	// Frame counter
	int nFrames = 0;


	/*用void glob(String pattern, std::vector<String>& result, bool recursive = false);当recursive为false时，
	仅仅遍历指定文件夹内符合模式的文件，当recursive为true时，会同时遍历指定文件夹的子文件夹
	pattern要绝对路径*/
	string pattern = "D:/kcf/DMBSystem/img/*.jpg";
	vector<Mat> images;
	// 必须cv的String
	vector<String> fn;
	glob(pattern, fn, false);//函数目的是将pattern路径下的所用文件名存进fn中
	size_t count = fn.size();
	for (nFrames = 0; nFrames < count; nFrames++) {
		images.push_back(imread(fn[nFrames]));
		// Read each frame from the list
		frame = images[nFrames];

		// First frame, give the groundtruth to the tracker
		if (nFrames == 0) {
			tracker.init(Rect(xMin, yMin, width, height), frame);
			rectangle(frame, Point(xMin, yMin), Point(xMin + width, yMin + height), Scalar(0, 255, 255), 1, 8);
			resultsFile << xMin << "," << yMin << "," << width << "," << height << endl;
		}
		// Update
		else {
			result = tracker.update(frame);
			rectangle(frame, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
			resultsFile << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
		}
		if (!SILENT) {
			imshow("Image", frame);
			waitKey(1);
		}
	}
	resultsFile.close();

	listFile.close();

}




void CDMBSystemDlg::OnBnClickedFindpath()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this);
	CString strFilePath;

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strr = fileDlg.GetPathName();
	}

}

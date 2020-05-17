
// DMBSystemDlg.cpp : ʵ���ļ�
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

//�����ο�

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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDMBSystemDlg �Ի���



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


// CDMBSystemDlg ��Ϣ�������

BOOL CDMBSystemDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDMBSystemDlg::OnPaint()
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
HCURSOR CDMBSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CDMBSystemDlg::OnBnClickedStartvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;



	// Create KCFTracker object// ����KCF������
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
	// Tracker results ���ٽ��Ŀ���
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
				//cout << "X����:" << result.x << "  " << "Y�����꣺" << result.y << endl;
				//cout << "����X����:" << result.x << "  " << "���Ͻ�Y���꣺" << result.y << "  " << "���Ͻ�X����:��" << result.x + result.width << "  " << "����Y������:��" << result.y << endl;
				//cout << "����X����:" << result.x << "  " << "���½�Y���꣺" << result.y + result.height << "  " << "���½�X����:��" << result.x + result.width << "  " << "����Y������:��" << result.y + result.height << endl;

				rectangle(img, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
				//write.write(img); //��֡��Ϊ��Ƶ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;


	// Create KCFTracker object// ����KCF������
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
	// Tracker results ���ٽ��Ŀ���
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
				//cout << "X����:" << result.x << "  " << "Y�����꣺" << result.y << endl;
				//cout << "����X����:" << result.x << "  " << "���Ͻ�Y���꣺" << result.y << "  " << "���Ͻ�X����:��" << result.x + result.width << "  " << "����Y������:��" << result.y << endl;
				//cout << "����X����:" << result.x << "  " << "���½�Y���꣺" << result.y + result.height << "  " << "���½�X����:��" << result.x + result.width << "  " << "����Y������:��" << result.y + result.height << endl;

				rectangle(img, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
				//write.write(img); //��֡��Ϊ��Ƶ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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


	/*��void glob(String pattern, std::vector<String>& result, bool recursive = false);��recursiveΪfalseʱ��
	��������ָ���ļ����ڷ���ģʽ���ļ�����recursiveΪtrueʱ����ͬʱ����ָ���ļ��е����ļ���
	patternҪ����·��*/
	string pattern = "D:/kcf/DMBSystem/img/*.jpg";
	vector<Mat> images;
	// ����cv��String
	vector<String> fn;
	glob(pattern, fn, false);//����Ŀ���ǽ�pattern·���µ������ļ������fn��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this);
	CString strFilePath;

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strr = fileDlg.GetPathName();
	}

}

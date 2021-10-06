#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat img; Mat src_gray;
int thresh = 100; // ����� ��������
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, int)
{
	Mat canny_output; // ����������� �� �������� ������
	vector<vector<Point>> contours; // ��������� ������ ��� �������� ������ ��������� ��������
	vector<Vec4i> hierarchy; // ���������, ������� ����� ��������� �� ������ ������� ������������������, ���������� ������ ��������� ��������

	// ��������� ���������� ����
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/*
	src_gray � ������������� ����������� ��� ��������� (�������� ������)
	canny_output � ������������� ����������� ��� �������� ������, ��������� ��������
	thresh � ����� �������� (100)
    thresh * 2 � ����� ��������� (200)
    3 � ������ ��� ��������� ������ (�������� ������� ����������� � ������ �����.)
	*/

	// ������� �������
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/*
	 canny_output � ����������� �� �������� ������ ���������� �� Canny()
     contours � ��������� ������ ��� �������� ������ ��������� ��������
	 hierarchy � ���������, ������� ����� ��������� �� ������ ������� ������������������, ���������� ������ ��������� ��������
	 RETR_TREE - ����� ��� ������� � ���������� �� � �������� ��������� ��������
	 CHAIN_APPROX_SIMPLE - ������� ��������������, ������������ � ������������ �������� � ��������� ������ �� �������� �����
	*/

	// ��������� �������
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	  {
		mu[i] = moments(contours[i], false);
	  }
	
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3); // CV_8UC3 - ����������� ��� ����� � 3 ��������
	
	// ������� ����� ����
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	  {
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00); 
		// ����� ���� ������ ������� - ����� � �� �� ����� � ����� � �� �� �����
	  }
	
	for (int i = 0; i< contours.size(); i++)
	  {
		printf(" ������ � %d: ����� ���� - x = %.2f  � = %.2f; ����� - %.2f \n", i, mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00, arcLength(contours[i], true));
	  }
	
	// ������ �������
	for (int i = 0; i < contours.size(); i++)
	  {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		/*
		drawing � ����������� �� ������� ����� ���������� �������
contour � ��������� �� ������ ������
external_color � ���� ������� ��������
hole_color � ���� ���������� ��������(���������)
max_level � ������������ ������� ��� ����������� �������� (0 � ������ ������ ������, 1 � ������ � ��� ��������� �� ������ ������, 2 � ��� ��������� ������� � ��� ������� �� ��������� ������ � �.�. ) ���� �������� �������������, �� ����� ���������� ������� �� ���������� ������ ����� contour.
thickness � ������� ����� ��� ����������� �������� (���� �������� �������������, �� �������, ������������ �������� ���������� ��������� ������ )
line_type � ��� �����
		*/
		circle(drawing, mc[i], 4, color, -1, 5, 0);
		/*
		drawing � ����������� �� ������� ����� ���������� �������
		mc[i] � ��������� �� ������ ������
		4 � ���� ������� ��������
		color � ���� ���������� ��������(���������)
		-1 - ����� ���������� ������� �� ���������� ������ ����� contour.
		8 � ������� ����� ��� ����������� �������� 
		line_type � ��� �����
		*/
	}

	// ������� � ����
	namedWindow("�������", WINDOW_AUTOSIZE);
	imshow("�������", drawing);
}

int main()
{
	// ������������ ������� ������ ��� �������
	setlocale(LC_ALL, "Russian");
	
	char filename[80]; // road.jpg

	cout << "������� ��� �����, � ������� ������ ������ ���������, � ������� Enter" << endl;
	cin.getline(filename, 80);
	cout << "������ ���� ";
	cout << filename << endl;
    
	// ��������� �����������
    Mat img = imread(filename, 1 );
    
    // ������������ ����������� � ����� � ������������ ���
     cvtColor(img, src_gray, COLOR_RGB2GRAY);
     blur(src_gray, src_gray, Size(3,3) );
    
    /// ������� ����
    char* source_window = "�������� �����������";
    namedWindow(source_window, WINDOW_AUTOSIZE);
    imshow(source_window, img ); // ����� ��� �����������, 
	//source_window - ��� �������� ����, � ������� ����� ���������� �����������,
	//src �  ��� ����������, ������� ������ ������ �����������. 
    
    //createTrackbar( " Canny thresh:", "�������� �����������", &thresh, max_thresh, thresh_callback );
    thresh_callback(138, 300);
	/*" Canny thresh:" � �������� ��������
		"Source" � �������� ����, � ������� ����� ������������ ��������
		&thresh � ��������� �� ������������� ����������, ������� ������ ������� ������� ��������.
		max_thresh � ������������ ������� ��������(����������� ������ 0).
		thresh_callback � ��������� �� �������, ������� ����� ���������� ������ ��� ��� ��������� ��������� ��������.
		*/
    waitKey(0); //  ������ ������� ������������� ���������� ������� �� ������� ������� �� ����������. 
	// �������� 0 �������� ��� ������� ����� ������� ����� ���������.
    return(0);
}
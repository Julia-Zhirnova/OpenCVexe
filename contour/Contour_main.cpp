#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat img; Mat src_gray; Mat canny_output; Mat _img; // ������� ��� �����������
int thresh = 110; // ����� ��������
RNG rng(12345); // ��������� ����� ��� ������
vector<vector<Point>> contours; // ��������� ������ ��� �������� ������ ��������� ��������
vector<Vec4i> hierarchy; // ���������, ������� ����� ��������� �� ������ ������� ������������������, ���������� ������ ��������� ��������

string filename()
{
	//������������ ������� ������ ��� �������
	setlocale(LC_ALL, "Russian");
	string filename; // road.jpg
	cout << "������� ��� �����, � ������� ������ ������ ���������, � ������� Enter" << endl;
	cin >> filename;
	cout << "������ ���� ";
	cout << filename << endl;
	// ��������� �����������
	return filename;
}

Mat LoadImage(string filename)
{
	Mat img = imread(filename, 1);
	return img;
}

void ShowImage(string filename, Mat Limg)
{
	string source_window = "�������� ����������� " + filename;
	namedWindow(source_window, WINDOW_AUTOSIZE);
	imshow(source_window, Limg); // ����� ��� �����������, 
					//source_window - ��� �������� ����, � ������� ����� ���������� �����������,
								//src �  ��� ����������, ������� ������ ������ �����������. 
}

Mat Canny(Mat Limg)
{
	Mat src_gray;
	// ������������ ����������� � ����� � ������������ ���
	cvtColor(Limg, src_gray, COLOR_RGB2GRAY);
	imwrite("cvtColor.jpg", src_gray);
	blur(src_gray, src_gray, Size(3, 3));
	imwrite("blur33.jpg", src_gray);

	// ���������� ����� �������� � ���������
	double otsu_thresh_val = threshold(src_gray, _img, 0, 255, THRESH_BINARY | THRESH_OTSU);
	double high_thresh_val = otsu_thresh_val, lower_thresh_val = otsu_thresh_val * 0.5;
	cout << "����� ���������� " << otsu_thresh_val; // ������� �� �����
	Mat canny_output;
	Canny(src_gray, canny_output, lower_thresh_val, high_thresh_val, 3);
	/*
	src_gray � ������������� ����������� ��� ��������� (�������� ������)
	canny_output � ������������� ����������� ��� �������� ������, ��������� ��������
	thresh � ����� �������� (100)
	thresh * 2 � ����� ��������� (200)
	3 � ������ ��� ��������� ������ (�������� ������� ����������� � ������ �����.)
	*/

	/// ������� ���� ��� ������ �����������
	char* source_grey_window = "�������� Canny";
	namedWindow(source_grey_window, WINDOW_AUTOSIZE);
	imshow(source_grey_window, canny_output);
	imwrite("canny_output.jpg", canny_output);
	return canny_output;
}



int main()
{
	string file_name = filename();
	Mat Limg = LoadImage(file_name);
	ShowImage(file_name, Limg);
	Mat canny_output = Canny(Limg);
	
	
	
	// ������� �������
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/*
	canny_output � ����������� �� �������� ������ ���������� �� Canny()
	contours � ��������� ������ ��� �������� ������ ��������� ��������
	hierarchy � ���������, ������� ����� ��������� �� ������ ������� ������������������, ���������� ������ ��������� ��������
	RETR_TREE - ����� ��� ������� � ���������� �� � �������� ��������� ��������
	CHAIN_APPROX_SIMPLE - ������� ��������������, ������������ � ������������ �������� � ��������� ������ �� �������� �����
	*/

	// ������� �������
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	//������� ����������� ���� ����� �������� �������
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
	
	/*int ex;
	cout << "�����" << endl;
	cin >> ex;*/

	waitKey(0); //  ������ ������� ������������� ���������� ������� �� ������� ������� �� ����������. 
					// �������� 0 �������� ��� ������� ����� ������� ����� ���������.
	
	system("PAUSE");
	return(0);
	

}
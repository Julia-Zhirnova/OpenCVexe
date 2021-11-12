#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat img; Mat src_gray; Mat canny_output; Mat _img; // матрицы для изображений
int thresh = 110; // порог минимума
RNG rng(12345); // рандомные числа для цветов
vector<vector<Point>> contours; // хранилище памяти для хранения данных найденных контуров
vector<Vec4i> hierarchy; // указатель, который будет указывать на первый элемент последовательности, содержащей данные найденных контуров

string filename()
{
	//устанвливаем русскую локаль для консоли
	setlocale(LC_ALL, "Russian");
	string filename; // road.jpg
	cout << "Введите имя файла, в который хотите внести изменения, и нажмите Enter" << endl;
	cin >> filename;
	cout << "Открыт файл ";
	cout << filename << endl;
	// загружаем изображение
	return filename;
}

Mat LoadImage(string filename)
{
	Mat img = imread(filename, 1);
	return img;
}

void ShowImage(string filename, Mat Limg)
{
	string source_window = "Исходное изображение " + filename;
	namedWindow(source_window, WINDOW_AUTOSIZE);
	imshow(source_window, Limg); // метод для отображения, 
					//source_window - это название окна, в котором будет отрисовано изображение,
								//src –  имя переменной, которая хранит данное изображение. 
}

Mat Canny(Mat Limg)
{
	Mat src_gray;
	// конвертируем изображение в серый и затуманиваем его
	cvtColor(Limg, src_gray, COLOR_RGB2GRAY);
	imwrite("cvtColor.jpg", src_gray);
	blur(src_gray, src_gray, Size(3, 3));
	imwrite("blur33.jpg", src_gray);

	// определяем порог минимума и максимума
	double otsu_thresh_val = threshold(src_gray, _img, 0, 255, THRESH_BINARY | THRESH_OTSU);
	double high_thresh_val = otsu_thresh_val, lower_thresh_val = otsu_thresh_val * 0.5;
	cout << "Порог фильтрации " << otsu_thresh_val; // выводим на экран
	Mat canny_output;
	Canny(src_gray, canny_output, lower_thresh_val, high_thresh_val, 3);
	/*
	src_gray — одноканальное изображение для обработки (градации серого)
	canny_output — одноканальное изображение для хранения границ, найденных функцией
	thresh — порог минимума (100)
	thresh * 2 — порог максимума (200)
	3 — размер для оператора Собеля (градиент яркости изображения в каждой точке.)
	*/

	/// создаем окно для серого изображения
	char* source_grey_window = "Оператор Canny";
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
	
	
	
	// находит контуры
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/*
	canny_output — изображения из градаций серого полученное из Canny()
	contours — хранилище памяти для хранения данных найденных контуров
	hierarchy — указатель, который будет указывать на первый элемент последовательности, содержащей данные найденных контуров
	RETR_TREE - найти все контуры и разместить их в иерархии вложенных контуров
	CHAIN_APPROX_SIMPLE - сжимает горизонтальные, вертикальные и диагональные сегменты и оставляет только их конечные точки
	*/

	// находим моменты
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	//создаем изображение куда будем заносить контуры
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3); // CV_8UC3 - изображение без знака с 3 каналами

	// находим центр масс
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		// центр масс одного объекта - сумма х на их число и сумма у на их число
	}

	for (int i = 0; i< contours.size(); i++)
	{
		printf(" Контур № %d: центр масс - x = %.2f  у = %.2f; длина - %.2f \n", i, mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00, arcLength(contours[i], true));
	}

	// рисуем контуры
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		/*
		drawing — изображение на котором будут нарисованы контуры
		contour — указатель на первый контур
		external_color — цвет внешних контуров
		hole_color — цвет внутренних контуров(отверстие)
		max_level — максимальный уровень для отображения контуров (0 — только данный контур, 1 — данный и все следующие на данном уровне, 2 — все следующие контуры и все контуры на следующем уровне и т.д. ) Если величина отрицательная, то будут нарисованы контуры на предыдущем уровне перед contour.
		thickness — толщина линии для отображения контуров (если величина отрицательная, то область, ограниченная контуром заливается выбранным цветом )
		line_type — тип линии
		*/
		circle(drawing, mc[i], 4, color, -1, 5, 0);
		/*
		drawing — изображение на котором будут нарисованы контуры
		mc[i] — указатель на первый контур
		4 — цвет внешних контуров
		color — цвет внутренних контуров(отверстие)
		-1 - будут нарисованы контуры на предыдущем уровне перед contour.
		8 — толщина линии для отображения контуров
		line_type — тип линии
		*/
	}

	// выводим в окно
	namedWindow("Контуры", WINDOW_AUTOSIZE);
	imshow("Контуры", drawing);
	
	/*int ex;
	cout << "Выход" << endl;
	cin >> ex;*/

	waitKey(0); //  Данная команда останавливает выполнение скрипта до нажатия клавиши на клавиатуре. 
					// Параметр 0 означает что нажатие любой клавиши будет засчитано.
	
	system("PAUSE");
	return(0);
	

}
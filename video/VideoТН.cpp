#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <ctime>

using namespace cv;
using namespace std;

int main() {

	//// устанвливаем русскую локаль для консоли
	setlocale(LC_ALL, "Russian");

	//char filename[100]; // New_year.mp4
	//cout << "Введите имя файла, который хотите проанализировать, и нажмите Enter" << endl;
	//cin.getline(filename, 100);
	//cout << "Открыт файл ";
	//cout << filename << endl;

	VideoCapture vidos("Figyres.mp4");
	if (!vidos.isOpened()) cout << "Видео не найдено, проверьте указанный путь" << endl;

	auto fps = vidos.get(CAP_PROP_FPS); // frames per seconds
	auto delay = 1000 / fps;
	cout << "Задержка в мс = " << delay << endl;

	Mat frame;

	while (1)
	{
		auto time = clock();
		vidos >> frame; //считаем фрейм
		if (frame.channels() != 1) cvtColor(frame, frame, COLOR_RGB2GRAY);
		Mat x, y;
		Sobel(frame, x, CV_16S, 1, 0);
		Sobel(frame, y, CV_16S, 0, 1);
		// модуль собеля
		convertScaleAbs(x, x);
		convertScaleAbs(y, y);
		addWeighted(x, 0.5, y, 0.5, 0, frame); //совмещаем картинки
		imshow("sobel", frame);
		// скорость видео
		double time_work = (clock() - time) * 1000 / CLOCKS_PER_SEC;
		if (time_work > delay - 1) time_work = 1;
		else time_work = delay - time_work;
		//  если обработка собелем больше чем задержка тогда получится отрицательное значение(ошибка)
		int vidos = waitKey(time_work);
		if (vidos >= 0) break;
	}
	destroyAllWindows();
	return 0;
}
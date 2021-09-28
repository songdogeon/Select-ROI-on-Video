#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

bool mouseIsPressing = false;
int start_x, start_y, end_x, end_y;
int step = 0;
Mat img_color;

void swap(int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

void mouseCallback(int event, int x, int y,int flag, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		mouseIsPressing = true;
		start_x = x;
		start_y = y;
	}
	if (event == EVENT_MOUSEMOVE)
	{
		if (mouseIsPressing)
		{
			end_x = x;
			end_y = y;

			step = 2;
		}
	}
	if (event == EVENT_LBUTTONUP)
	{
		mouseIsPressing = false;
		end_x = x;
		end_y = y;
		step = 3;
	}
}
int main()
{
	string videoFile1 = "C:\\opencv\\challenge.mp4";
	
	VideoCapture cap(videoFile1);
	if (!cap.isOpened())
	{
		cout << "에러:영상을 열 수 없습니다.";
	}

	namedWindow("Color", 1);
	setMouseCallback("Color", mouseCallback);

	while (1)
	{
		cap.read(img_color);
		imshow("Color", img_color);

		switch (step)
		{
		case 1:
			circle(img_color, Point(start_x, start_y), 5, Scalar(0, 0, 255), -1, 8);
			break;
		case 2:
			rectangle(img_color, Rect(Point(start_x, start_y), Point(end_x, end_y)), Scalar(0, 0, 255), 3, 8);
			break;
		case 3:
			if (start_x > end_x)
			{
				swap(&start_x, &end_x);
				swap(&start_y, &end_y);
			}

			Mat ROI(img_color, Rect(start_x, start_y, end_x - start_x, end_y - start_y));
			cvtColor(ROI, ROI, COLOR_BGR2GRAY);
			Canny(ROI, ROI, 150, 50);
			cvtColor(ROI, ROI, COLOR_GRAY2BGR);

			ROI.copyTo(img_color(Rect(start_x, start_y, end_x - start_x, end_y - start_y)));
			break;

		}

		imshow("Color", img_color);

		if (waitKey(25) >= 0) break;
	}
	
	destroyAllWindows();

	return 0;
}
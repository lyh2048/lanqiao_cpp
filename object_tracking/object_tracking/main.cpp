#include <opencv2/opencv.hpp>


bool selectObject = false;
int trackObject = 0;
cv::Rect selection;
cv::Mat image;


void onMouse(int event, int x, int y, int, void*)
{
	static cv::Point origin;
	if (selectObject)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);
		selection &= cv::Rect(0, 0, image.cols, image.rows);
	}
	switch (event)
	{
	case cv::EVENT_LBUTTONDOWN:
		origin = cv::Point(x, y);
		selection = cv::Rect(x, y, 0, 0);
		selectObject = true;
		break;
	case cv::EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			trackObject = -1;
		break;
	}
}


int main(int argc, char* argv[])
{
	cv::VideoCapture video("video.ogv");
	cv::namedWindow("CamShift");
	cv::setMouseCallback("CamShift", onMouse, 0);

	cv::Mat frame, hsv, hue, mask, hist, backproj;
	cv::Rect trackWindow;
	int hsize = 16;
	float hranges[] = { 0, 180 };
	const float* phranges = hranges;
	while (true)
	{
		video >> frame;
		if (frame.empty())
			break;
		frame.copyTo(image);
		// 转换到HSV
		cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

		if (trackObject)
		{
			cv::inRange(hsv, cv::Scalar(0, 30, 10), cv::Scalar(180, 256, 256), mask);
			int ch[] = { 0, 0 };
			hue.create(hsv.size(), hsv.depth());
			cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);
			if (trackObject < 0)
			{
				cv::Mat roi(hue, selection), maskroi(mask, selection);
				// 计算 ROI所在区域的直方图
				cv::calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
				// 直方图归一化
				cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
				trackWindow = selection;
				trackObject = 1;
			}
			// 直方图反向投影
			cv::calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
			// 取公共部分
			backproj &= mask;
			// 使用CamShift算法
			cv::RotatedRect trackBox = cv::CamShift(backproj, trackWindow, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1));
			// 追踪面积过小的情况
			if (trackWindow.area() <= 1)
			{
				int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5) / 6;
				trackWindow = cv::Rect(trackWindow.x - r, trackWindow.y - r, trackWindow.x + r, trackWindow.y + r) & cv::Rect(0, 0, cols, rows);
			}
			// 绘制追踪区域
			cv::ellipse(image, trackBox, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
		}
		// 如果正在选择追踪目标，则画出选择框
		if (selectObject && selection.width > 0 && selection.height > 0)
		{
			cv::Mat roi(image, selection);
			cv::bitwise_not(roi, roi);
		}

		cv::imshow("CamShift", image);
		char c = (char)cv::waitKey(1000 / 15);
		if (c == 27)
			break;
	}
	cv::destroyAllWindows();
	video.release();
	return 0;
}
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;


Mat g_img;
Mat g_templ;
Mat g_result;
int g_match_method = 0;
int g_max_trackbar = 5;
const char* g_source_window_name = "Source Window";
const char* g_result_window_name = "Result Window";


void Match(int, void*)
{
	Mat img_display;
	g_img.copyTo(img_display);
	// �����������ľ���
	int result_cols = g_img.cols - g_templ.cols + 1;
	int result_rows = g_img.rows - g_templ.rows + 1;
	g_result.create(result_cols, result_rows, CV_32FC1);
	// ����ƥ��ͱ�׼��
	matchTemplate(g_img, g_templ, g_result, g_match_method);
	normalize(g_result, g_result, 0, 1, NORM_MINMAX, -1, Mat());
	// �����ƥ��λ��
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(g_result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	// ����SQDIFF �� SQDIFF_NORMED����ֵԽС��ƥ����Խ��
	// �����������ķ�������ֵԽ��ƥ����Խ��
	if (g_match_method == TM_SQDIFF || g_match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	// ��ԭͼ�ϱ�ע
	rectangle(img_display, matchLoc, Point(matchLoc.x + g_templ.cols, matchLoc.y + g_templ.rows), Scalar(0, 0, 255), 2, 8, 0);
	// ���ƥ����
	imshow(g_source_window_name, img_display);
	imwrite("result.jpg", img_display);
	// ����������
	imshow(g_result_window_name, g_result);
}


int main(int argc, char* argv[])
{
	// ����ͼ��
	g_img = imread("original.jpg");
	g_templ = imread("template.jpg");
	// ��������
	namedWindow(g_source_window_name, WINDOW_AUTOSIZE);
	namedWindow(g_result_window_name, WINDOW_AUTOSIZE);
	// ����������
	createTrackbar("bar", g_source_window_name, &g_match_method, g_max_trackbar, Match);
	// ��ʼ��
	Match(0, 0);
	// �����ȴ�
	waitKey(0);
	return 0;
}
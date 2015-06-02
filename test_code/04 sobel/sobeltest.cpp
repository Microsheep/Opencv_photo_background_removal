#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(){
	Mat src,src_gray,grad;
	int scale = 1,delta = 0,ddepth = CV_16S;
	src = imread("test.png", -1);
	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
	cvtColor( src, src_gray, CV_RGB2GRAY );
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
	imwrite("test-out.png", grad);
	return 0;
}

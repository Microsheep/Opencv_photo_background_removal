#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(){
	Mat src;
	src = imread("test.png", -1);
	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
	imwrite("test-out.png", src);
	return 0;
}

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(){
	Mat src,out;
	src = imread("test.png", -1);
	bitwise_not ( src, out );
	imwrite("test-out.png", out);
	return 0;
}

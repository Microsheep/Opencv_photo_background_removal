#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(){
	Mat img = imread("super.png", -1);
	Mat mat(img.rows, img.cols, CV_8UC4);
	for(int y=0;y<img.rows;y++){
		for(int x=0;x<img.cols;x++){
			Vec3b color = img.at<Vec3b>(Point(x,y));
			Vec4b newcolor;
			newcolor[0]=color[0];
			newcolor[1]=color[1];
			newcolor[2]=color[2];
			newcolor[3]=255;
			if(color[0]==255&&color[1]==255&&color[2]==255){
				newcolor[3]=0;
			}
			mat.at<Vec4b>(Point(x,y))=newcolor;
		}
	}
	imwrite("outcome_super.png", mat);
	return 0;
}


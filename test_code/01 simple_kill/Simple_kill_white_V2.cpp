#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void killcolor1(Mat& in,Mat& out,int x,int y,int z){
	for(int yy=0;yy<in.rows;yy++){
		for(int xx=0;xx<in.cols;xx++){
			Vec3b color = in.at<Vec3b>(Point(xx,yy));
			Vec4b newcolor;
			newcolor[0]=color[0];
			newcolor[1]=color[1];
			newcolor[2]=color[2];
			newcolor[3]=255;
			if(color[0]==x&&color[1]==y&&color[2]==z){
				newcolor[3]=0;
			}
			out.at<Vec4b>(Point(xx,yy))=newcolor;
		}
	}
	return;
}

void killcolor2(Mat& in,Mat& out,int x,int y,int z){
	for(int yy=0;yy<in.rows;yy++){
		for(int xx=0;xx<in.cols;xx++){
			Vec4b color = in.at<Vec4b>(Point(xx,yy));
			if(color[0]==x&&color[1]==y&&color[2]==z){
				color[3]=0;
			}
			out.at<Vec4b>(Point(xx,yy))=color;
		}
	}
	return;
}

void work(int a,int b,int c,int cnt){
	Mat img = imread("test.png", -1);
	Mat mat(img.rows, img.cols, CV_8UC4);
	if(cnt){killcolor2(img,mat,a,b,c);}
	else{killcolor1(img,mat,a,b,c);}
	imwrite("test.png", mat);
	return;
}

int main(){
	int x=0,a=255,b=255,c=255;
	cin >> x;
	for(int cnt=0;cnt<x;cnt++){
		cin >> a >> b >> c;
		work(a,b,c,cnt);
	}
	return 0;
}


#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

//定義rgb_point
class rgb_num{
public:
	rgb_num(){
		r=255;g=255;b=255;
	}
	rgb_num(int rin, int gin, int bin){
		r=rin;g=gin;b=bin;
	}
	void setrgb(int rin, int gin, int bin){
		r=rin;g=gin;b=bin;
	}
	int getr(){return r;}
	int getg(){return g;}
	int getb(){return b;}
private:
	int r,g,b;
};

//產生邊框 (24bit->8bit)
void soler_guass(char* input_image_name, char* output_image_name){
	Mat src,src_gray,out;
	int scale = 1,delta = 0,ddepth = CV_16S;
	src = imread(input_image_name, -1);
	bitwise_not (src, src);
	GaussianBlur(src, src, Size(3,3), 0, 0, BORDER_DEFAULT);
	cvtColor(src, src_gray, CV_RGB2GRAY);
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, out);
	GaussianBlur(out, out, Size(3,3), 0, 0, BORDER_DEFAULT);
	imwrite(output_image_name, out);
	return;
}

//(24bit->32bit)
void init24to32(char* input_image_name, char* output_image_name){
	Mat in = imread(input_image_name, -1);
	Mat out(in.rows, in.cols, CV_8UC4);
	for(int yy=0;yy<in.rows;yy++){
		for(int xx=0;xx<in.cols;xx++){
			Vec3b color = in.at<Vec3b>(Point(xx,yy));
			Vec4b newcolor;
			newcolor[0]=color[0];
			newcolor[1]=color[1];
			newcolor[2]=color[2];
			newcolor[3]=255;
			out.at<Vec4b>(Point(xx,yy))=newcolor;
		}
	}
	imwrite(output_image_name, out);
	return;
}

//去除特定顏色
//x,y,z被去除的顏色
void killcolor(Mat& in, Mat& out, rgb_num x){
	for(int yy=0;yy<in.rows;yy++){
		for(int xx=0;xx<in.cols;xx++){
			Vec4b color = in.at<Vec4b>(Point(xx,yy));
			if(color[0]==x.getb()&&color[1]==x.getg()&&color[2]==x.getr()){
				color[3]=0;
			}
			out.at<Vec4b>(Point(xx,yy))=color;
		}
	}
	return;
}

//開檔並去除特定顏色
void work_kill(char* input_image_name, char* output_image_name, rgb_num x){
	Mat img = imread(input_image_name, -1);
	Mat mat(img.rows, img.cols, CV_8UC4);
	killcolor(img, mat, x);
	imwrite(output_image_name, mat);
	return;
}

//填滿特定顏色外的顏色
//leave don't want color
//rep replace with color
void fillcolor(Mat& in, Mat& out, rgb_num leave, rgb_num rep){
	for(int yy=0;yy<in.rows;yy++){
		for(int xx=0;xx<in.cols;xx++){
			Vec4b color = in.at<Vec4b>(Point(xx,yy));
			if(color[0]==leave.getb()&&color[1]==leave.getg()&&color[2]==leave.getr()){
				out.at<Vec4b>(Point(xx,yy))=color;
				continue;
			}
			color[0]=rep.getb();color[1]=rep.getg();color[2]=rep.getr();
			out.at<Vec4b>(Point(xx,yy))=color;
		}
	}
	return;
}

//開檔並填滿特定顏色外的顏色
void work_fill(char* input_image_name, char* output_image_name, rgb_num leave, rgb_num rep){
	Mat img = imread(input_image_name, -1);
	Mat mat(img.rows, img.cols, CV_8UC4);
	fillcolor(img, mat, leave, rep);
	imwrite(output_image_name, mat);
	return;
}

//把不是黑色的地方填成白色 (8-bit)
void fillcolor_bw(Mat& in, Mat& out){
	for(int yy=0;yy<in.rows;yy++){
		for(int xx=0;xx<in.cols;xx++){
			uchar color = in.at<uchar>(Point(xx,yy));
			if(color==0){
				out.at<uchar>(Point(xx,yy))=color;
			}
			else{
				color=255;
				out.at<uchar>(Point(xx,yy))=color;
			}
		}
	}
	return;
}

//開檔並把不是黑色的地方填成白色 (8-bit)
void work_fill_nonblack(char* input_image_name, char* output_image_name){
	Mat img = imread(input_image_name, 0);
	Mat mat(img.rows, img.cols, CV_8UC1);
	fillcolor_bw(img, mat);
	imwrite(output_image_name, mat);
	return;
}

int main(int argc, char** argv){
	//define colors
	rgb_num black(0,0,0);
	rgb_num white(255,255,255);
	//record in out file
	char* in=argv[1];
	char* out=argv[2];
	//start processing argv[1]=input argv[2]=output
	soler_guass(in,out);
	//The following worked bad! It is not pure black for outside region!
	work_fill_nonblack(out,out);
    //To be implemented...

	return 0;
}

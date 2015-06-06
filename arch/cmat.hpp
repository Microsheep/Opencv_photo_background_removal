#ifndef CMAT_H
#define CMAT_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <queue>
#include <string>
using namespace cv;
using namespace std;

class CMat{
	private:
		Mat pic;
		string in_file_path;
		string out_file_path;
		void change_channels_24_32();
	public:
		CMat();
		CMat(const CMat& rhs);
		CMat& operator=(const CMat& rhs);
		~CMat();

		void set_in_file_path(string _string);
		void set_out_file_path(string _string);

		bool empty() const ;
		void clear();
		
		int channels();
		void change_channels(int from, int to);

		void open();
//		void open8();
//		void open24();
		void open32();

		void save();
//		void save8();
//		void save24();
//		void save32();
		
		void close();
		
		void guass();
//		void guass_1();
//		void guass_2();
//		void guass_3();

		void soble();
//		void soble_1();
//		void soble_2();
//		void soble_3();

		void black_white(int threshold);
		void lonely(int white, int black);
		
		vector<pair<int, int> > flood(vector<pair<int, int> > source);
		
		void kill_pos_color(vector<pair<int, int> > pos);	//col, row

		void process();

		void print();
};

#endif

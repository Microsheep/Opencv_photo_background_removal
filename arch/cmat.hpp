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

		CMat& set_in_file_path(string _string);
		CMat& set_out_file_path(string _string);

		bool empty() const ;
		void clear();
		
		int channels();
		CMat& change_channels(const int from, const int to);

		CMat& open(const int type = 32);

		CMat& save();
		
		void close();
		
		CMat& guass();

		CMat& soble();

		CMat& cvt_color();

		CMat& black_white(const int threshold);
		CMat& lonely(const int white, const int black);
		
		vector<pair<int, int> > flood(vector<pair<int, int> > source);
		
		CMat& kill_pos_color(vector<pair<int, int> > pos);	//col, row

		CMat& print();

		string& get_in_file_path(){
			return in_file_path;
		}
		string& get_out_file_path(){
			return out_file_path;
		}
};

#endif

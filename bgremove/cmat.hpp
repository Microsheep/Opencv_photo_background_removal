#ifndef CMAT_H
#define CMAT_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <ctime>
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

		int get_cols();
		int get_rows();
		Vec3b get_3b_at(int, int);
		Vec4b get_4b_at(int, int);

		int channels();
		CMat& change_channels(const int from, const int to);

		CMat& open(const int type = 32);

		CMat& save();

		void close();

		CMat& guass();

		CMat& soble();

		CMat& cvt_color(Vec3b color=Vec3b(255,255,255));

		CMat& black_white(const int threshold);
		CMat& lonely(const int white, const int black);

		vector<pair<int, int> > flood(vector<pair<int, int> > source);

		CMat& kill_pos_color(vector<pair<int, int> > pos, int x = 1);	//col, row

		CMat& print();

		string& get_in_file_path(){
			return in_file_path;
		}
		string& get_out_file_path(){
			return out_file_path;
		}
};

#endif

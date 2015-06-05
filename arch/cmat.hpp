#ifndef CMAT_H
#define CMAT_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

class CMat{
	private:
		Mat pic;
		string in_file_path;
		string out_file_path;
	public:
		CMat();
		CMat(const CMat& rhs);
		CMat& operator=(const CMat& rhs);
		~CMat();

		void set_in_file_path(string _string);
		void set_out_file_path(string _string);

		bool empty() const ;
		void clear();
		

		void open();
//		void open8();
//		void open24();
//		void open32();

		void save();
//		void save8();
//		void save24();
//		void save32();
		
		void close();


//		void guass_1();
//		void guass_2();
//		void guass_3();

//		void soble_1();
//		void soble_2();
//		void soble_3();

		void process();
		
};

#endif

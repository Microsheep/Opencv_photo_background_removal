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

int main(){
	CMat me;
	me.set_in_file_path("girl.png");
	me.set_out_file_path("girl_end.png");
	me.process();
	return 0;
}

/* constructor and assign operator and destructor */
CMat::CMat(){
}
CMat::~CMat(){
	clear();
}
bool CMat::empty() const {
	return pic.empty();
}
void CMat::clear(){
	pic.release();
}
/* set about path */
void CMat::set_in_file_path(string _string){
	in_file_path = _string;
}
void CMat::set_out_file_path(string _string){
	out_file_path = _string;
}
/* about open */
void CMat::open(){
	pic = imread(in_file_path.c_str(), -1);
}
/* about save */
void CMat::save(){
	if(!empty())
		imwrite(out_file_path, pic);
}
/* about close */
void CMat::close(){
	if(!empty())
		pic.release();
}
void CMat::process(){
	open();
	save();
	close();
}

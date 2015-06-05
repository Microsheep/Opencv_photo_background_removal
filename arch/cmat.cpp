#include "cmat.hpp"
/* private function */
void CMat::change_channels_24_32(){
	Mat tmp(pic);
	cout << "OK" << endl;
	pic = Mat(tmp.rows, pic.cols, CV_8UC4);
	for(int y = 0 ; y < tmp.rows ; y++){
		for(int x = 0 ; x < tmp.cols ; x++){
			Vec3b color = tmp.at<Vec3b>(Point(x, y));
			Vec4b transcolor;
			transcolor[0] = color[0];
			transcolor[1] = color[1];
			transcolor[2] = color[2];
			transcolor[3] = 255;
			pic.at<Vec4b>(Point(x, y)) = transcolor;
		}
	}
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
int CMat::channels(){
	return pic.channels();
}
void CMat::change_channels(int from, int to){
	if(from == 24 && to == 32)
		change_channels_24_32();
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
	change_channels(24, 32);
	save();
	std::cout << channels() << std::endl;
	close();
}

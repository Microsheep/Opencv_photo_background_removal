#include "cmat.hpp"
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

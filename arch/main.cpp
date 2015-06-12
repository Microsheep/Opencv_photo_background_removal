#include "cmat.hpp"
int main(){
	CMat pic;
	CMat mask = pic.set_in_file_path("girl.png").set_out_file_path("girl.out.png").open();
	mask.guass().cvt_color().soble().guass().black_white(20).lonely(6, 20).set_out_file_path("girl.mask.png").save();
	vector<pair<int, int> > source;
	source.push_back(pair<int, int>(0,0));
	pic.kill_pos_color(mask.flood(source)).set_out_file_path("girl.after.png").save();
	return 0;
}


#include "cmat.hpp"
int main(){
	CMat pic;
	CMat mask = pic.set_in_file_path("32.png").set_out_file_path("32.out.png").open();
	mask.print();
//	mask.set_out_file_path("32.mask.out.png").guass();
//	mask.set_out_file_path("32.mask.out.png").guass().soble().guass().save();
	return 0;
}


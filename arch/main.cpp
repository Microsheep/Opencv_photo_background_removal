#include "cmat.hpp"
int main(){
	CMat me2;
	me2.set_in_file_path("32.png");
	me2.set_out_file_path("32_out.png");
	me2.process();
	return 0;
}


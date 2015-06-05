#include "cmat.hpp"
int main(){
	CMat me;
	me.set_in_file_path("girl.png");
	me.set_out_file_path("girl_end.png");
	me.process();
	return 0;
}


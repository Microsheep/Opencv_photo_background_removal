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
void CMat::open32(){
	open();
	if(channels() == 3)
		change_channels_24_32();
}
/* about save */
void CMat::save(){
	cout << "Save" << endl;
	if(!empty()){
		imwrite(out_file_path, pic);
	} else {
		cout << "GG" << endl;
	}
}
/* about close */
void CMat::close(){
	if(!empty())
		pic.release();
}
void CMat::guass(){
	GaussianBlur(pic, pic, Size(3,3), 0, 0, BORDER_DEFAULT);
}
void CMat::soble(){
	Mat src_gray(pic);
	pic.release();
	int scale = 1,delta = 0,ddepth = CV_16S;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, pic);
	src_gray.release();
	grad_x.release(), grad_y.release();
	abs_grad_x.release(), abs_grad_y.release();
}
void CMat::black_white(int threshold){
	for(int y = 0 ; y < pic.rows ; y++)
		for(int x = 0 ; x < pic.cols ; x++)
			pic.at<uchar>(Point(x,y)) = pic.at<uchar>(Point(x,y)) <= threshold ? 0 : 255;
}
void CMat::lonely(int white, int black){
	Mat tmp = pic;
	for(int y = 2 ; y < pic.rows ; y++)
		for(int x = 2 ; x < pic.cols ; x++){
			int black_num = 0;
			for(int i = x - 2 ; i < x + 2 ; i++)
				for(int j = y - 2 ; j < y + 2 ; j++)
					black_num += (tmp.at<uchar>(Point(i, j)) == 0);
			if(black_num < white){
				pic.at<uchar>(Point(x, y)) = 255;
			} else if(black_num < black){
			} else {
				pic.at<uchar>(Point(x, y)) = 0;
			}
		}
	tmp.release();
}
vector<pair<int, int> > CMat::flood(vector<pair<int, int> > source){
	bool v[pic.cols][pic.rows];
	cout << "OK" << endl;
	memset(v, 0, sizeof(v));
	queue<pair<int, int> > Queue;
	for(int i = 0 ; i < (int)source.size() ; i++){
		Queue.push(source[i]);
		v[source[i].first][source[i].second] = 1;
	}
	int d[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
	while(!Queue.empty()){
		pair<int, int> now = Queue.front();
		Queue.pop();
		for(int i = 0 ; i < 4 ; i++){
			pair<int, int> tmp = now;
			tmp.first += d[i][0];
			tmp.second += d[i][1];
			if(tmp.first < 0 || tmp.first >= pic.cols || tmp.second < 0 || tmp.second >= pic.rows)
				continue;
			if(v[tmp.first][tmp.second] == 1)
				continue;
			v[tmp.first][tmp.second] = 1;
			Queue.push(tmp);
		}
	}
	vector<pair<int, int> > re;
	for(int i = 0 ; i < pic.cols ; i++)
		for(int j = 0 ; j < pic.rows ; j++)
			if(v[i][j])
				re.push_back(pair<int, int>(i, j));
	return re;
}
/* kill color */
void CMat::kill_pos_color(vector<pair<int, int> > pos){
	for(int i = 0 ; i < (int)pos.size() ; i++){
		pic.at<Vec4b>(Point(pos[i].first,pos[i].second))[3] = 0;
	}
}
void CMat::process(){
	open32();

	guass();
	cvtColor(pic, pic, CV_RGB2GRAY);
	soble();
	guass();

	black_white(20);
	lonely(6, 20);
	
	vector<pair<int, int> > source;
	for(int i = 0 ; i < pic.rows ; i++)
		for(int j = 0 ; j < 20 ; j++)
			source.push_back(pair<int, int>(j, i));
	kill_pos_color(source);
//	for(int i = 0 ; i < pic.rows ; i++)
//		source.push_back(pair<int,int>(0,0));
//	vector<pair<int, int> > end = flood(source);
//	cout << end.size() << endl;
//	kill_pos_color(flood(source));
//
	save();
	close();
}
void CMat::print(){
	cout << pic << endl;
}

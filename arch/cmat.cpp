#include "cmat.hpp"



CMat::CMat(){
}
CMat::CMat(const CMat& rhs){
	clear();
	(*this) = rhs;
}
CMat& CMat::operator=(const CMat& rhs){
	rhs.pic.copyTo(pic);
	in_file_path = rhs.in_file_path;
	out_file_path = rhs.out_file_path;
	return (*this);
}
CMat::~CMat(){
	clear();
}

CMat& CMat::set_in_file_path(string _string){
	in_file_path = _string;
	return (*this);
}
CMat& CMat::set_out_file_path(string _string){
	out_file_path = _string;
	return (*this);
}
bool CMat::empty() const {
	return pic.empty();
}
void CMat::clear(){
	if(!pic.empty())
		pic.release();
}
int CMat::get_cols(){
	return pic.cols;
}
int CMat::get_rows(){
	return pic.rows;
}

Vec3b CMat::get_3b_at(int col, int row){
	return pic.at<Vec3b>(Point(col, row));
}
Vec4b CMat::get_4b_at(int col, int row){
	return pic.at<Vec4b>(Point(col, row));
}

int CMat::channels(){
	return pic.channels();
}
CMat& CMat::change_channels(const int from, const int to){
	if(from == 24 && to == 32){
		Mat tmp;
		pic.copyTo(tmp);
		pic = Mat(tmp.rows, tmp.cols, CV_8UC4);
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
		tmp.release();
	}
	return (*this);
}
CMat& CMat::open(const int type){
	pic = imread(in_file_path.c_str(), -1);
	change_channels(channels() * 8, type);
	return (*this);
}
CMat& CMat::save(){
	imwrite(out_file_path, pic);
	return (*this);
}
CMat& CMat::guass(){
	GaussianBlur(pic, pic, Size(3,3), 0, 0, BORDER_DEFAULT);
	return (*this);
}

CMat& CMat::soble(){
	Mat src_gray;
	pic.copyTo(src_gray);
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
	return (*this);
}

CMat& CMat::cvt_color(Vec3b color){
	Mat tmp;
	pic.copyTo(tmp);
	pic = Mat(tmp.rows, tmp.cols, CV_8UC1);
	for(int y = 0 ; y < pic.rows ; y++)
		for(int x = 0 ; x < pic.cols ; x++){
			Vec4b now_color = tmp.at<Vec4b>(Point(x, y));
			pic.at<uchar>(Point(x,y)) = (abs(now_color[0] - color[0]) + abs(now_color[1] - color[1]) + abs(now_color[2] - color[2])) / 3;
		}
	//cvtColor(pic, pic, CV_RGB2GRAY );
	return (*this);
}

CMat& CMat::black_white(const int threshold){
	for(int y = 0 ; y < pic.rows ; y++)
		for(int x = 0 ; x < pic.cols ; x++)
			pic.at<uchar>(Point(x,y)) = pic.at<uchar>(Point(x,y)) <= threshold ? 0 : 255;
	return (*this);
}
CMat& CMat::lonely(const int white, const int black){
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
	return (*this);
}

vector<pair<int, int> > CMat::flood(vector<pair<int, int> > source){
	bool v[pic.cols][pic.rows];
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
			if(pic.at<uchar>(Point(tmp.first, tmp.second)) == 0){
				v[tmp.first][tmp.second] = 1;
				Queue.push(tmp);
			}
		}
	}
	vector<pair<int, int> > re;
	for(int i = 0 ; i < pic.cols ; i++)
		for(int j = 0 ; j < pic.rows ; j++)
			if(v[i][j])
				re.push_back(pair<int, int>(i, j));
	return re;
}

CMat& CMat::kill_pos_color(vector<pair<int, int> > pos){	//col, row
	for(int i = 0 ; i < (int)pos.size() ; i++)
		pic.at<Vec4b>(Point(pos[i].first,pos[i].second))[3] = 0;
	return (*this);
}

CMat& CMat::print(){
	cout << pic << endl;
	return (*this);
}

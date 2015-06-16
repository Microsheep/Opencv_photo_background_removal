#include "cmat.hpp"
#include "msgpack.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

void print(value &v){
	stringstream ss;
	v.serialize(ss);
	string str;
	getline(ss, str);
	for(int i = 0 ; i < (int)str.size() ; i++)
		cout << setw(2) << setfill('0') << hex << (((int)str[i])&255) << ' ';
	cout << endl;
}
struct DEFAULT{
	value m_default;
	DEFAULT(){
		map<string, value> empty_value_map;
		m_default = value(empty_value_map);

		m_default.get_map()["file_in"]  = value(string("in.png"));
		m_default.get_map()["file_out"] = value(string("out.png"));

		m_default.get_map()["mask_file_out"] = value(string("mask.out.png"));

		m_default.get_map()["lonely"] = value(empty_value_map);
		m_default.get_map()["lonely"].get_map()["start"] = value(int64_t(6));
		m_default.get_map()["lonely"].get_map()["end"]   = value(int64_t(20));
		m_default.get_map()["lonely"].get_map()["times"] = value(int64_t(1));

		m_default.get_map()["cvt"]	= value(empty_value_map);
		m_default.get_map()["cvt"].get_map()["R"]	= value(int64_t(0));
		m_default.get_map()["cvt"].get_map()["G"]	= value(int64_t(0));
		m_default.get_map()["cvt"].get_map()["B"]	= value(int64_t(0));
		
		m_default.get_map()["black_white"] = value(int64_t(20));

		m_default.get_map()["margin"] = value(int64_t(2));
	}
	string& INPUT(){
		return m_default.get_map()["file_in"].get_string();
	}
	string& OUTPUT(){
		return m_default.get_map()["file_out"].get_string();
	}
	string& MASK_OUTPUT(){
		return m_default.get_map()["mask_file_out"].get_string();
	}
	int64_t& LONELY_START(){
		return m_default.get_map()["lonely"].get_map()["start"].get_integer();
	}
	int64_t& LONELY_END(){
		return m_default.get_map()["lonely"].get_map()["end"].get_integer();
	}
	int64_t& LONELY_TIMES(){
		return m_default.get_map()["lonely"].get_map()["times"].get_integer();
	}
	int64_t& BLACK_WHITE(){
		return m_default.get_map()["black_white"].get_integer();
	}
	int64_t& MARGIN(){
		return m_default.get_map()["margin"].get_integer();
	}
};
int string2int(string str){
	stringstream ss(str);
	int re;
	ss >> re;
	return re;
}
int main(int argc, char **argv){

	DEFAULT Default;
	for(int i = 1 ; i < argc ; i++){
		if(string(argv[i]) == string("-i")) {
			Default.INPUT() = string(argv[i+1]);
			++i;
		} else if (string(argv[i]) == string("-o")) {
			Default.OUTPUT() = string(argv[i+1]);
			++i;
		} else if (string(argv[i]) == string("-mo")) {
			Default.MASK_OUTPUT() = string(argv[i+1]);
			++i;
		} else if (string(argv[i]) == string("-l")) {
			Default.LONELY_START() = string2int(string(argv[i+1]));
			Default.LONELY_END()   = string2int(string(argv[i+2]));
			Default.LONELY_TIMES() = string2int(string(argv[i+3]));
			i+=3;
		} else if (string(argv[i]) == string("-bw")) {
			Default.BLACK_WHITE() = string2int(string(argv[i+1]));
			++i;
		} else if (string(argv[i]) == string("-m")) {
			Default.MARGIN() = string2int(string(argv[i+1]));
			++i;
		} else if (string(argv[i]) == string("help") || string(argv[i]) == string("-h")) {
			cout << "[Help]" << endl;
			cout << "[-i input] [-o output] [-mo mask_output]" << endl;
			cout << "[-l lonely_start lonely_end lonely_times]" << endl;
			cout << "[-bw black_white] [-m margin] [-h | help]" << endl;
			return 0;
		} else {
			return 0;
		}
	}
	Default.m_default.print_tree();

	CMat pic;
	CMat mask = pic.set_in_file_path( Default.INPUT() ).set_out_file_path( Default.OUTPUT() ).open();
	mask.guass().cvt_color().soble().guass().black_white( Default.BLACK_WHITE() );
	for(int i = 0 ; i < Default.LONELY_TIMES() ; i++){
		mask.lonely( Default.LONELY_START() + i, Default.LONELY_END() - i);
	}
	mask.set_out_file_path( Default.MASK_OUTPUT() ).save();

	vector<pair<int, int> > source;
	source.push_back(pair<int, int>(0,0));
	pic.kill_pos_color(mask.flood(source), Default.MARGIN()).save();
	return 0;
}

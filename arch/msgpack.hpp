#ifndef MSGPACK_HPP
#define MSGPACK_HPP

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "endian.h"

struct value {
    private:
    public:
        typedef bool boolean_type;
        typedef std::int64_t integer_type;
        typedef std::string string_type;
        typedef std::vector<value> array_type;
        typedef std::map<string_type, value> map_type;

        value(){tag = tag_t::nil_tag;}
        explicit value(const boolean_type &b){
            tag = tag_t::boolean_tag;
            val.b = b;
        }
        explicit value(const integer_type &i){
            tag = tag_t::integer_tag;
            val.i = i;
        }
        explicit value(const string_type& s){
//            tag = string_tag;
            tag = tag_t::string_tag;
            new(&val.s) string_type(s);
        }
        explicit value(const array_type& a){
            tag = tag_t::array_tag;
            new(&val.a) array_type(a);
        }
        explicit value(const map_type& m){
            tag = tag_t::map_tag;
            new(&val.m) map_type(m);
        }
        
        value(const value& rhs){
            tag = tag_t::nil_tag;
            (*this) = rhs;
        }
        value& operator=(const value& rhs){
            if(this == &rhs) return (*this);
            if(is_string()){ val.s.~string_type();
            } else if(is_array()){ val.a.~array_type();
            } else if(is_map()){ val.m.~map_type();
            }
            if(rhs.is_nil()){
            } else if(rhs.is_boolean()){ val.b = rhs.val.b;
            } else if(rhs.is_integer()){ val.i = rhs.val.i;
            } else if(rhs.is_string()){ new(&val.s) string_type(rhs.val.s);
            } else if(rhs.is_array()){ new(&val.a) array_type(rhs.val.a);
            } else if(rhs.is_map()){ new(&val.m) map_type(rhs.val.m);
            }
            tag = rhs.tag;
            return (*this);
        }
        ~value(){
            if(is_string()){ val.s.~string_type();
            } else if(is_array()){ val.a.~array_type();
            } else if(is_map()){ val.m.~map_type();
            }
        }

        bool is_nil() const { return tag == tag_t::nil_tag; }
        bool is_boolean() const { return tag == tag_t::boolean_tag; }
        bool is_integer() const { return tag == tag_t::integer_tag; }
        bool is_string() const { return tag == tag_t::string_tag; }
        bool is_array() const { return tag == tag_t::array_tag; }
        bool is_map() const { return tag == tag_t::map_tag; }

        boolean_type& get_boolean() {
            if(is_boolean()) return val.b;
            throw std::bad_cast();
        }
        integer_type& get_integer(){
            if(is_integer()) return val.i;
            throw std::bad_cast();
        }
        string_type& get_string(){
            if(is_string()) return val.s;
            throw std::bad_cast();
        }
        array_type& get_array(){
            if(is_array()) return val.a;
            throw std::bad_cast();
        }
        map_type& get_map(){
            if(is_map()) return val.m;
            throw std::bad_cast();
        }

        const boolean_type& get_boolean() const {
            if(is_boolean()) return val.b;
            throw std::bad_cast();
        }
        const integer_type& get_integer() const {
            if(is_integer()) return val.i;
            throw std::bad_cast();
        }
        const string_type& get_string() const {
            if(is_string()) return val.s;
            throw std::bad_cast();
        }
        const array_type& get_array() const{
            if(is_array()) return val.a;
            throw std::bad_cast();
        }
        const map_type& get_map() const{
            if(is_map()) return val.m;
            throw std::bad_cast();
        }

        friend bool operator==(const value& lhs, const value& rhs){
            if(lhs.tag != rhs.tag) return false;
            if(lhs.is_nil()){ return true;
            } else if(lhs.is_boolean()){ return lhs.val.b == rhs.val.b;
            } else if(lhs.is_integer()){ return lhs.val.i == rhs.val.i;
            } else if(lhs.is_string()){ return lhs.val.s == rhs.val.s;
            } else if(lhs.is_array()){ return lhs.val.a == rhs.val.a;
            } else if(lhs.is_map()){ return lhs.val.m == rhs.val.m;
            } return false;
        }
        friend bool operator!=(const value& lhs, const value& rhs){ return !(lhs == rhs); }

        std::ostream& serialize(std::ostream& out) const {
            if(is_nil()){
                out << '\xc0';
            } else if( is_boolean() ){
                out << (val.b ? '\xc3' : '\xc2');
            } else if( is_integer() ){
                out << '\xd3';
                for(int x = 7 ; x >= 0 ; x--)
                    out << char( (val.i >> (8*x)) & (0xff) );
            } else if( is_string() ){
                out << '\xdb';
                unsigned int sz = 0u;
                while(sz < val.s.size() && val.s[sz]) sz++;
                for(int x = 3 ; x >= 0 ; x--)
                    out << char( (sz >> (8u*x)) & (0xff) );
                for(unsigned int i = 0u ; i < sz ; i++)
                    out << val.s[i];
            } else if( is_array() ){
                out << '\xdd';
                for(int x = 3 ; x >= 0 ; x--)
                    out << char( ((unsigned int)val.a.size() >> (8*x)) & (0xff) );
                for(auto x : val.a)
                    x.serialize(out);
            } else if( is_map() ){
                out << '\xdf';
                for(int x = 3 ; x >= 0 ; x--)
                    out << char( ((unsigned int)val.m.size() >> (8*x)) & (0xff) );
                for(auto x : val.m){
                    value(x.first).serialize(out);
                    x.second.serialize(out);
                }
            }
            return out;
        }
        static value deserialize(std::istream& in){
            bool start = in.tellg() == 0;
            char c_tag, tmp_c;
            value re;
            if(in.get(c_tag)){
                if(c_tag == '\xc0'){
                } else if(c_tag == '\xc2' || c_tag == '\xc3'){
                    re = value(c_tag == '\xc3');
                } else if(c_tag == '\xd3'){
                    int64_t tmp = 0;
                    for(int x = 7 ; x >= 0 ; x--){
                        if(in.get(tmp_c)){
                            tmp |= (int64_t(tmp_c) << (8 * x));
                        } else {
                            in.setstate(std::ios::failbit); //unexpected end
                            break;
                        }
                    }
                    re = value(tmp);
                } else if(c_tag == '\xdb' || c_tag == '\xdd' || c_tag == '\xdf'){   //string, array, map
                    unsigned int size = 0;
                    for(int x = 3 ; x >= 0 ; x--){      //calc size
                        if(in.get(tmp_c)){
                            size |= ((unsigned int)(tmp_c) << (8 * x));
                        } else {
                            in.setstate(std::ios::failbit); //unexpected end
                            break;
                        }
                    }
                    if(c_tag == '\xdb'){    //string
                        std::string str;
                        for(unsigned int i = 0u ; i < size ; i++){
                            if(in.get(tmp_c)){
                                str.push_back(tmp_c);
                            } else {
                                in.setstate(std::ios::failbit); //unexpected end
                                break;
                            }
                        }
                        re = value(str);
                    } else if(c_tag == '\xdd'){ //array
                        array_type array;
                        for(unsigned int i = 0u ; i < size ; i++)
                            array.push_back(deserialize(in));
                        re = value(array);
                    } else if(c_tag == '\xdf'){ //map
                        map_type map;
                        for(unsigned int i = 0u ; i < size ; i++){
                            value string = deserialize(in);
                            value data = deserialize(in);
                            if(string.is_string())
                                map[string.get_string()] = data;
                        }
                        re = value(map);
                    }
                } else {
                    in.setstate(std::ios::failbit); //unexpected tag
                }
            } else {    //unexpected end
                in.setstate(std::ios::failbit);
            }
            if(start && (in.peek() != EOF)){    //no read to end
                in.setstate(std::ios::failbit);
            }
            if(in.fail())
                re = value();
            return re;
        }
		void print_tree(int deep = 0, std::string name = ""){
			for(int i = 0 ; i < deep ; i++)
				std::cout << "  ";
			if(name.size())
				std::cout << name << ": ";
			if( is_nil() ){
				std::cout << "nil" << std::endl;
			} else if( is_boolean() ) {
				std::cout << std::boolalpha << get_boolean() << std::endl;
			} else if( is_integer() ) {
				std::cout << get_integer() << std::endl;
			} else if( is_string() ) {
				std::cout << get_string() << std::endl;
			} else if( is_array() ){
				std::cout << "Array: " << std::endl;
				for(auto x : get_array())
					x.print_tree(deep+1);
			} else if( is_map() ){
				std::cout << "Map: " << std::endl;
				for(auto x : get_map()){
					x.second.print_tree(deep+1, x.first);
				}
			}
		}

    private:
        enum class tag_t { nil_tag, boolean_tag, integer_tag, string_tag,
                           array_tag, map_tag };
        union union_t {
            union_t() {}
            ~union_t() {}
            boolean_type b;
            integer_type i;
            string_type s;
            array_type a;
            map_type m;
        };

        tag_t tag;
        union_t val;
};

#endif

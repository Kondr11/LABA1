//Copyright 2019 <Kondr11>
#ifndef HEADER_JSON_HPP
#define HEADER_JSON_HPP

#include <iostream>
#include <any>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <map>
#include <list>

using namespace std;

class Json {

public:
    size_t t = 0;
    map <string, any> data_map;
    vector <any> data_arr;

    Json(const string &s);

    Json(const map<string, any> &map);

    Json(const vector<any> &vector);

    bool is_array() const;
    
    bool is_object() const;

    
    any & operator[](const string &key);

    
    any & operator[](int index); 
    
    static Json parse(const string &s);
    
    static Json parseFile(const string &path_to_file);

    string split_string(const string &str, size_t &j);

    bool split_bool(const string &str, size_t &j);

    double split_double(const string &str, size_t &j);

    vector<any> split_arr(const string &str, size_t &j);

    map<string, any> split_obj(const string &str, size_t &j);

    void print(any _data);

    void print_map();

};


#endif //HEADER_JSON_HPP

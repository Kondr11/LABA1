#include "library.h"
    // Конструктор из строки, содержащей Json-данные.
    Json::Json(const string &s) {
        size_t i = 0;
        if (s[i] == '{') {
            data_map = split_obj(s, i);
        } else {
            if (s[i] == '[') {
                data_arr = split_arr(s, i);
            }
        }
    }

    Json::Json(const map<string, any> &map) {
        data_map = map;
    }

    Json::Json(const vector<any> &vector) {
        data_arr = vector;
    }


    
    bool Json::is_array() const;

    
    bool Json::is_object() const;

    
    any & Json::operator[](const string &key);

    
    any & Json::operator[](int index); 
    
    static Json Json::parse(const string &s) {
        return Json(s);
    }

    
    static Json Json::parseFile(const string &path_to_file);

    Json::string split_string(const string &str, size_t &j);

    bool Json::split_bool(const string &str, size_t &j);

    double Json::split_double(const string &str, size_t &j);

    vector<any> Json::split_arr(const string &str, size_t &j);

    map<string, any> Json::split_obj(const string &str, size_t &j);

    void Json::print(any _data);

    void Json::print_map();

};


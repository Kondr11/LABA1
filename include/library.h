#ifndef HEADER_LIBRARY_H
#define HEADER_LIBRARY_H

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
    // Конструктор из строки, содержащей Json-данные.
    Json(const string &s);

    Json(const map<string, any> &map);

    Json(const vector<any> &vector);

    // Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
    bool is_array() const {
        try {
            any_cast<vector<any>>(data_arr);
            return true;
        }
        catch (const bad_any_cast &e) {
            return false;
        }
    }

    // Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
    bool is_object() const {
        try {
            any_cast<map<string, any>>(data_map);
            return true;
        }
        catch (const bad_any_cast &e) {
            return false;
        }
    }

    // Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-массивом, генерируется исключение.
    any &operator[](const string &key) {
        try {
            if (is_object()) {
                return any_cast<map<string, any>>(data_map)[key];
            }
        }
        catch (exception &e) {
            cout << e.what() << endl;
        }

    }

    // Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-объектом, генерируется исключение.
    any &operator[](int index) {
        try {
            if (is_array()) {
                return any_cast<vector<any>>(data_arr)[index];
            }
        }
        catch (exception &e) {
            cout << e.what() << endl;
        }
    }

    // Метод возвращает объект класса Json из строки, содержащей Json-данные.
    static Json parse(const string &s) {
        return Json(s);
    }

    // Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
    static Json parseFile(const string &path_to_file) {
        ifstream file;
        string s;
        string line;
        while (getline(file, line)) {
            s += line;
        }
        return Json(s);
    }

    string split_string(const string &str, size_t &j) {
        string result;
        size_t next;
        size_t teta;
        j = str.find('"', j);
        if (str[j] == '"') {
            next = str.find('"', j+1);
            teta = next-j-1;
            result = str.substr(j+1, teta);
            //cout<<str.substr(j+1, teta)<<endl;
            this->t = next++;
        }
        return result;
    }

    bool split_bool(const string &str, size_t &j) {
        string string;
        size_t i = j;
        if (isalpha(str[i]) && str[i] == 't') {
            this->t = j + 4;
            return true;
        }
        else if (isalpha(str[i]) && str[i] == 'f') {
            this->t = j + 5;
            return false;
        }
    }

    double split_double(const string &str, size_t &j) {
        string s;
        for (size_t i = j; i < str.size(); ++i) {
            if (isdigit(str[i]) || str[i] == '.') {
                s += str[i];
            } else if (str[i] == ',' || str[i] == ']' || str[i] == '}'
                       || isspace(str[i])) {
                this->t = --i;
                return stof(s);
            }
        }
        //return stof(s);
    }

    vector<any> split_arr(const string &str, size_t &j) {
        vector<any> result;
        j = str.find('[', j);
        this->t = j;
        int k = 1;
        size_t *next = new size_t[10];
        next[0] = j;
        size_t *exit = new size_t[10];
        exit[0] = str.find(']', j+1);

        for (size_t i = exit[0]; i < str.find_last_of(']', j+1); i++) {
            k++;
            next[k] = str.find('[', next[k-1]+1);
            exit[k] = str.find(']', exit[k-1]+1);
            i = exit[k];
            //cout<<exit[k]<<endl;
            if(i == str.find_last_of(']', j+1))
                break;
        }

        if (str[j] == '[') {
            for (size_t i = j; i < exit[k-1]; i++) {
                if (isdigit(str[i])) {
                    result.emplace_back(split_double(str, i));
                    i = this->t;
                } else if (isalpha(str[i])) {
                    result.emplace_back(split_bool(str, i));
                    i = this->t;
                } else if (str[i] == '"') {
                    result.emplace_back(split_string(str, i));
                    i = this->t;
                } /*else if (str[i] == '[') {
                    result.emplace_back(split_arr(str, i));
                    i = this->t;
                } */else if (str[i] == '{') {
                    result.emplace_back(split_obj(str, i));
                    i = this->t;
                } else if (str[i] == ',' || isspace(str[i])) {
                    i = str.find_first_not_of(isspace(str[i]), i+1) - 1;
                    this->t = i;
                } else if (str[i] == ']') {
                    this->t = i;
                    return result;
                }
            }
        }
    }

    map<string, any> split_obj(const string &str, size_t &j) {
        map<string, any> result;
        string key;
        bool iskey = true;
        j = str.find('{', j);
        this->t = j;
        int k = 0;
        size_t *next = new size_t[10];
        next[0] = j;
        size_t *exit = new size_t[10];
        exit[0] = str.find('}', j+1);

        for (size_t i = exit[0]; i < str.find_last_of('}', j+1); i++) {
            k++;
            next[k] = str.find('{', next[k-1]+1);
            exit[k] = str.find('}', exit[k-1]+1);
            i = exit[k];
            //cout<<exit[k]<<endl;
            if(i == str.find_last_of('}', j+1))
                break;
        }


        if (str[j] == '{') {
            for (size_t i = str.find('"', j); i < exit[k]; i++) {
                if (str[i] == '"' && iskey) {
                    key = split_string(str, i);
                    iskey = false;
                    this->t = str.find(':', this->t);
                    i = str.find_first_not_of(isspace(str[this->t]), this->t+1);
                }
                else if (str[i] == '"' && !iskey) {
                    result[key] = split_string(str, i);
                    i = this->t;
                    iskey = true;
                } else if (isdigit(str[i])) {
                    result[key] = split_double(str, i);
                    i = this->t;
                    iskey = true;
                } else if (isalpha(str[i])) {
                    result[key] = split_bool(str, i);
                    i = this->t;
                    iskey = true;
                } else if (str[i] == '[') {
                    result[key] = split_arr(str, i);
                    i = this->t;
                    iskey = true;
                } else if (str[i] == '{') {
                    result[key] = split_obj(str, i);
                    i = this->t;
                    iskey = true;
                } else if (str[i] == '}') {
                    this->t = i;
                    return result;
                }
            }
        }
    }

    void print(any _data) {
        string type = _data.type().name();

        try {
            if (type == "i") {
                cout << any_cast<int> (_data);
            }
            else if (type == "d") {
                cout << any_cast<double> (_data);
            }
            else if (type == "b") {
                if (any_cast<bool> (_data)) std::cout << "true";
                else cout << "false";
            }
            else if (type == "Ss" ||type == "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE") {
                cout << any_cast<string> (_data);
            }
            else if (type.find("St6vector") < type.length()) {
                std::vector <std::any> vec;
                vec = any_cast<vector<any>>(_data);
                unsigned int count = 0;
                cout << "[ ";
                for(const auto& c: vec) {
                    count++;
                    if (count > 1) std::cout << " , ";
                    print(c);
                }
                cout << " ]";
            }
            else if (type.find("St3map") < type.length()) {
                map <string, any> _map;
                _map = std::any_cast<map <string, any>>(_data);
                cout << "{\n" ;
                unsigned int count = 0;
                for(const auto& c: _map) {
                    count++;
                    if (count > 1) cout << " ,\n";
                    cout << "\t" << c.first << " : ";
                    print(c.second);
                }
                cout << "\n\t}";
            }
        }
        catch(const std::bad_any_cast& e) {
            cout << e.what() << '\n';
        }
    }

    void print_map() {
        cout << "{\n";
        int i = 0;
        for(const auto& p : this->data_map) {
            if (i != 0) cout <<",\n";
            cout << "   "<< p.first << " : ";
            print(p.second);
            i++;
        }
        cout<< "}\n";
    }

};


#endif //HEADER_LIBRARY_H

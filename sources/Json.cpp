#include "library.h"
    
    
    // Конструктор из строки, содержащей Json-данные.
    Json::Json(const string &s) {
        //for (size_t i = 0; i < s.size(); i++) {
        size_t i = 0;
        if (s[i] == '{') {
            data_map = split_obj(s, i);
            //cout<<any_cast<map<string, any>>(data)[i];
        } else {
            if (s[i] == '[') {
                data_arr = split_arr(s, i);
            }
            else{
                //throw logic_error("Error");
            }
        }
        //}
    }

    Json::Json(const map<string, any> &map) {
        data_map = map;
    }

    Json::Json(const vector<any> &vector) {
        data_arr = vector;
    }


    // Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
    bool Json::is_array() const;

    // Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
    bool Json::is_object() const;

    // Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-массивом, генерируется исключение.
    any & Json::operator[](const string &key);

    // Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-объектом, генерируется исключение.
    any & Json::operator[](int index); 
    // Метод возвращает объект класса Json из строки, содержащей Json-данные.
    static Json Json::parse(const string &s) {
        return Json(s);
    }

    // Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
    static Json Json::parseFile(const string &path_to_file);

    Json::string split_string(const string &str, size_t &j);

    bool Json::split_bool(const string &str, size_t &j);

    double Json::split_double(const string &str, size_t &j);

    vector<any> Json::split_arr(const string &str, size_t &j);

    map<string, any> Json::split_obj(const string &str, size_t &j);

    void Json::print(any _data);

    void Json::print_map();

};


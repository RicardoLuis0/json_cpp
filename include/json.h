#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

enum JSON_Literal {
    JSON_FALSE,
    JSON_TRUE,
    JSON_NULL,
};

namespace JSON {
    class Element {
        public:
            using data_t = std::variant<int64_t,double,std::string,std::vector<Element>,std::map<std::string,Element>,JSON_Literal>;
            data_t data;
            
            //explicit constructors using std::variant
            explicit inline Element(const data_t & v) : data(v) {}
            explicit inline Element(data_t && v) : data(std::move(v)) {}
            
            //conversion constructors for simple data types
            inline Element(int i) : data(i) {}
            inline Element(int64_t i) : data(i) {}
            inline Element(double d) : data(d) {}
            inline Element(const std::string &s) : data(s) {}
            inline Element(std::string &&s) : data(std::move(s)) {}
            inline Element(bool b) : data(b?JSON_TRUE:JSON_FALSE) {}
            inline Element(std::nullptr_t) : data(JSON_NULL) {}
            inline Element(JSON_Literal l) : data(l) {}
            
            //helper access methods, may throw std::bad_variant_access if trying to access wrong types
            inline int64_t& get_int(){ return std::get<int64_t>(data); }
            inline const int64_t& get_int() const { return std::get<int64_t>(data); }
            
            inline double& get_double(){ return std::get<double>(data); }
            inline const double& get_double() const { return std::get<double>(data); }
            
            inline std::string& get_str(){ return std::get<std::string>(data); }
            inline const std::string& get_str() const { return std::get<std::string>(data); }
            
            inline std::vector<Element>& get_arr(){ return std::get<std::vector<Element>>(data); }
            inline const std::vector<Element>& get_arr() const { return std::get<std::vector<Element>>(data); }
            
            inline std::map<std::string,Element>& get_obj(){ return std::get<std::map<std::string,Element>>(data); }
            inline const std::map<std::string,Element>& get_obj() const { return std::get<std::map<std::string,Element>>(data); }
            
            inline JSON_Literal& get_lit() { return std::get<JSON_Literal>(data); }
            inline const JSON_Literal& get_lit() const { return std::get<JSON_Literal>(data); }
            
            inline bool get_bool() const { return std::get<JSON_Literal>(data)!=JSON_NULL?std::get<JSON_Literal>(data)==JSON_TRUE:throw std::bad_variant_access(); }
            //std::bad_variant_access doesn't _quite_ fit the situation but it's best to keep the exception thrown consistent for all get_* methods
            
            
            
            //helper type check methods
            inline bool is_int() const { return std::holds_alternative<int64_t>(data); }
            
            inline bool is_double() const { return std::holds_alternative<double>(data); }
            
            inline bool is_str() const { return std::holds_alternative<std::string>(data); }
            
            inline bool is_arr() const { return std::holds_alternative<std::vector<Element>>(data); }
            
            inline bool is_obj() const { return std::holds_alternative<std::map<std::string,Element>>(data); }
            
            inline bool is_lit() const { return std::holds_alternative<JSON_Literal>(data); }
            
            inline bool is_bool() const { return std::holds_alternative<JSON_Literal>(data)&&std::get<JSON_Literal>(data)!=JSON_NULL; }
            
            inline bool is_null() const { return std::holds_alternative<JSON_Literal>(data)&&std::get<JSON_Literal>(data)==JSON_NULL; }
            
            //serialize with spaces/newlines
            std::string to_json(bool trailing_quote=true,size_t depth=0) const;
            
            //serialize without spaces/newlines
            std::string to_json_min() const;
            
    };
    
    inline Element Int(int64_t i){ return Element(i); }
    inline Element Boolean(bool b){ return Element(b?JSON_TRUE:JSON_FALSE); }
    inline Element True(){ return Element(JSON_TRUE); }
    inline Element False(){ return Element(JSON_FALSE); }
    inline Element Null(){ return Element(JSON_NULL); }
    inline Element Double(double d){ return Element(d); }
    inline Element String(std::string s){ return Element(s); }
    inline Element Array(const std::vector<Element> & v){ return Element(Element::data_t(v)); }
    inline Element Array(std::vector<Element> && v){ return Element(Element::data_t(std::move(v))); }
    inline Element Object(const std::map<std::string,Element> & m){ return Element(Element::data_t(m)); }
    inline Element Object(std::map<std::string,Element> && m){ return Element(Element::data_t(std::move(m))); }
    
    Element parse(const std::string &data);
    
}

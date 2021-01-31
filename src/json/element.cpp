#include "json.h"
#include <stdexcept>

namespace JSON {
    namespace {
        constexpr char escape(char c){
            switch(c) {
            case '\a':
                return 'a';
            case '\b':
                return 'b';
            case '\e':
                return 'e';
            case '\f':
                return 'f';
            case '\n':
                return 'n';
            case '\r':
                return 'r';
            case '\t':
                return 't';
            case '\v':
                return 'v';
            case '\\':
                return '\\';
            case '"':
                return '"';
            default:
                return c;
            }
        }
        
        inline std::string quote_str(std::string s){
            std::string str;
            str.reserve(s.size()+10);
            str+='"';
            for(char c:s){
                if(c=='\\'||c=='"'||escape(c)!=c){
                    str+='\\';
                    str+=escape(c);
                }else{
                    str+=c;
                }
            }
            str+='"';
            return str;
        }
        
        inline std::string indent(size_t depth){
            return std::string(depth*4,' ');
        }
    }
    
    std::string Element::to_json(bool trailing_quote,size_t depth) const {
        if(std::holds_alternative<int64_t>(data)){//int
            return std::to_string(get_int());
        }else if(std::holds_alternative<double>(data)){//double
            return std::to_string(get_double());
        }else if(std::holds_alternative<std::string>(data)){//string
            return quote_str(get_str());
        }else if(std::holds_alternative<JSON_Literal>(data)){//literal
            return get_lit()==JSON_TRUE?"true":get_lit()==JSON_FALSE?"false":"null";
        }else if(std::holds_alternative<std::vector<Element>>(data)){//array
            return "[\n"+({
                std::string s;
                bool first=true;
                for(auto &e:get_arr()){
                    if(!first)s+=",\n";
                    s+=indent(depth+1);
                    s+=e.to_json(trailing_quote,depth+1);
                    first=false;
                }
                if(!s.empty())s+=trailing_quote?",\n":"\n";
                s;
            })+indent(depth)+"]";
        }else if(std::holds_alternative<std::map<std::string,Element>>(data)){//object
            return "{\n"+({
                std::string s;
                bool first=true;
                for(auto &e:get_obj()){
                    if(!first)s+=",\n";
                    s+=indent(depth+1);
                    s+=quote_str(e.first);
                    s+=" : ";
                    s+=e.second.to_json(trailing_quote,depth+1);
                    first=false;
                }
                if(!s.empty())s+=trailing_quote?",\n":"\n";
                s;
            })+indent(depth)+"}";
        }
        __builtin_unreachable();//all std::variant cases are handled in the if/else, this is absolutely unreachable
    }
    
    std::string Element::to_json_min() const {
        if(std::holds_alternative<int64_t>(data)){//int
            return std::to_string(get_int());
        }else if(std::holds_alternative<double>(data)){//double
            return std::to_string(get_double());
        }else if(std::holds_alternative<std::string>(data)){//string
            return quote_str(get_str());
        }else if(std::holds_alternative<JSON_Literal>(data)){//literal
            return get_lit()==JSON_TRUE?"true":get_lit()==JSON_FALSE?"false":"null";
        }else if(std::holds_alternative<std::vector<Element>>(data)){//array
            return "["+({
                std::string s;
                bool first=true;
                for(auto &e:get_arr()){
                    if(!first)s+=",";
                    s+=e.to_json_min();
                    first=false;
                }
                s;
            })+"]";
        }else if(std::holds_alternative<std::map<std::string,Element>>(data)){//object
            return "{"+({
                std::string s;
                bool first=true;
                for(auto &e:get_obj()){
                    if(!first)s+=",";
                    s+=quote_str(e.first);
                    s+=":";
                    s+=e.second.to_json_min();
                    first=false;
                }
                s;
            })+"}";
        }
        __builtin_unreachable();//all std::variant cases are handled in the if/else, this is absolutely unreachable
    }
}

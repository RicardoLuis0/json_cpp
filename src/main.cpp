#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <windows.h>
#include "json.h"

static std::string readfile(const std::string &filename){
    std::ostringstream ss;
    std::ifstream f(filename);
    if(!f)throw std::runtime_error(strerror(errno));
    ss<<f.rdbuf();
    return ss.str();
}

int main() {
    std::string file=readfile("test.json");
    JSON::Element elem(JSON::parse(file));
    std::cout<<elem.to_json()<<"\n";
    return 0;
}

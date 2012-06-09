#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "BloxorzMap.h"

namespace {
    std::vector<std::string> splitString(const std::string& s, const std::string& delim,
                                         bool keep_empty=false) {
        std::vector<std::string> result;
        if (delim.empty()) {
            result.push_back(s);
            return result;
        }
        std::string::const_iterator substart = s.begin(), subend;
        while (true) {
            subend = std::search(substart, s.end(), delim.begin(), delim.end());
            std::string temp(substart, subend);
            if (keep_empty || !temp.empty()) {
                result.push_back(temp);
            }
            if (subend == s.end()) {
                break;
            }
            substart = subend + delim.size();
        }
        return result;
    }

    std::string trimString(const std::string& str){
        std::string trimChars=" \t\n\r";
        size_t endpos = str.find_last_not_of(trimChars);
        size_t startpos = str.find_first_not_of(trimChars);
        if(std::string::npos == endpos || std::string::npos == startpos)
            return "";
        return str.substr(startpos, endpos-startpos+1);
    }

}


BloxorzMap::BloxorzMap(const std::string& f):mapFile(f){
    std::ifstream file(f.c_str());
    std::stringstream ss;
    ss<<file.rdbuf();
    mapStr = splitString(ss.str(),"\n");
    std::transform(mapStr.begin(), mapStr.end(), mapStr.begin(), trimString);
    
    const std::string VALID_CHARS = " X+";
    
    for (int i=0, len1=mapStr.size(); i<len1; i++){
        std::vector<Cell> row;
        for (int j=0, len2=mapStr[i].length(); j<len2; j++){
            if (VALID_CHARS.find(mapStr[i][j]) == std::string::npos)
                continue;
            row.push_back(Cell(mapStr[i][j], i, j));
        }
        map.push_back(row);
    }
}

Cell BloxorzMap::get(int r, int c) const{
    return map[r][c];
}

void BloxorzMap::getBox(int& r1, int& c1, int& r2, int& c2) const{
    r1 = c1 = r2 = c2 = -1;
    for (int i=0, len1=mapStr.size(); i<len1; i++){
        for (int j=0, len2=mapStr[i].length(); j<len2; j++){
            if (mapStr[i][j] == '+'){
                if (r1 == -1){
                    r1 = i;
                    c1 = j;
                }else{
                    r2 = i;
                    c2 = j;
                    return;
                }
            }
        }
    }
}

void BloxorzMap::init(){
    for (std::vector<std::vector<Cell> >::iterator it1=map.begin(); it1!= map.end(); it1++){
        for (std::vector<Cell>::iterator it2=it1->begin(); it2!=it1->end(); it2++){
            it2->init();
        }
    }
}

void BloxorzMap::paint(){
    for (std::vector<std::vector<Cell> >::iterator it1=map.begin(); it1!= map.end(); it1++){
        for (std::vector<Cell>::iterator it2=it1->begin(); it2!=it1->end(); it2++){
            it2->paint();
        }
    }
}
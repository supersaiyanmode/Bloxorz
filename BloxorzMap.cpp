#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <GL/gl.h>

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


BloxorzMap::BloxorzMap(const std::string& f):loading(1), mapList(0), mapFile(f){
    
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
    map = std::vector<std::vector<Cell> >();
    
    //Read the file and load the map from it.
    std::ifstream file(mapFile.c_str());
    if (file.bad()){
        std::cout<<"Map file does not exist.\n";
        std::exit(1);
    }
    std::stringstream ss;
    ss<<file.rdbuf();
    mapStr = splitString(ss.str(),"\n");
    std::transform(mapStr.begin(), mapStr.end(), mapStr.begin(), trimString);
    
    //pad the map with bad cells.
    for (std::vector<std::string>::iterator it=mapStr.begin(); it!=mapStr.end(); it++){
        *it = ".." + *it + "..";
    }
    std::string border(mapStr[0].size(),'.');        //2 '.' on either side to pad.
    mapStr.insert(mapStr.begin(), 2, border);
    mapStr.insert(mapStr.end(), 2, border);
    
    
    
    for (int i=0, len1=mapStr.size(); i<len1; i++){
        std::vector<Cell> row;
        for (int j=0, len2=mapStr[i].length(); j<len2; j++){
            row.push_back(Cell(mapStr[i][j], i, j));
        }
        map.push_back(row);
    }
    
    //throw 0;
    
    for (std::vector<std::vector<Cell> >::iterator it1=map.begin(); it1!= map.end(); it1++){
        for (std::vector<Cell>::iterator it2=it1->begin(); it2!=it1->end(); it2++){
            it2->init();
        }
    }
    std::cout<<"Map loaded!\n";
    load();
}

bool BloxorzMap::paint(){
    bool stillInvalid = false;
    
    //If not loading, go ahead make a list.
    if (!loading){
        if (!mapList){  //does the list exist?
            //create the list.
            Cell::setUseList(false);
            mapList = glGenLists(1);
            glNewList(mapList, GL_COMPILE_AND_EXECUTE);
                for (std::vector<std::vector<Cell> >::iterator it1=map.begin(); it1!= map.end();
                                it1++){
                    for (std::vector<Cell>::iterator it2=it1->begin(); it2!=it1->end(); it2++){
                        if (it2->paint())
                            stillInvalid = true;
                    }
                }
            glEndList();
            //std::cout<<"List compiled!\n";
        }else{
            //std::cout<<"Calling list..\n";
            glCallList(mapList);
        }
    }else{
        //std::cout<<"Loading ..\n";
        for (std::vector<std::vector<Cell> >::iterator it1=map.begin(); it1!= map.end();
                                it1++){
            for (std::vector<Cell>::iterator it2=it1->begin(); it2!=it1->end(); it2++){
                if (it2->paint())
                    stillInvalid = true;
            }
        }
        //update `loading`
        loading = stillInvalid?1:0;
    }
    return stillInvalid;
}

void BloxorzMap::load(){
    loading = 1;
    Cell::setUseList(true);
}
void BloxorzMap::unload(){
    //std::cout<<"UNLOADING...\n";
    loading = -1;
}

std::vector<std::vector<char> > BloxorzMap::state(){
    std::vector<std::vector<char> > res;
    
    for (int i=0, len1=mapStr.size(); i<len1; i++){
        std::vector<char> row;
        for (int j=0, len2=mapStr[i].length(); j<len2; j++){
            if (mapStr[i][j] == '+'){
                row.push_back('X');
            }else{
                row.push_back(mapStr[i][j]);
            }
        }
        res.push_back(row);
    }
    return res;
}

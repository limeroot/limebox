/*
 *  Created on  05/05/2013 15:40:28
 *
 *  This file is part of the LR command for GNU/Linux LimeRoot
 *
 *  Copyright (c) LimeRoot, http://www.limeroot.org, devel@limeroot.org
 *
 *  Author: Daniel R. Torres Ortiz, daniel@limeroot.org
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "printable.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <json/json.h>

using namespace std; 

Printable::Printable():m_lineIndex(-1){
    //m_lineIndex = -1;
    m_max = 0;
}

Printable::~Printable(){
    
}

void Printable::Print(vector<Printable> &list){

    map<string, unsigned> colsizes;
    
    vector<string> colnames;
    
    unsigned width = 0;
    
    for(auto printable : list){
        
        for(auto key : *printable.keys()){
            
            map<string, unsigned>::iterator it = colsizes.find(key.first);
            
            if(it != colsizes.end()){
                if(key.second > it->second){
                    width -= it->second;
                    width += key.second;
                    it->second = key.second;
                }
            } else {
                width += key.second;
                colsizes[key.first] = key.second;
            }
            
            vector<string>::iterator vit = find(colnames.begin(), colnames.end(), key.first);
            
            if(vit == colnames.end())
                colnames.push_back(key.first);
        }
    }
    
    
    cout << setfill('-') << "+" << setw(width + (colnames.size() * 3 )) << "+" << endl;

    cout << setfill(' ') << "| ";
        
    for(auto name : colnames){
        //if(name == "") continue;
        cout << setw(colsizes[name]) << name << " | ";
    }
        
    cout << endl;
    
    cout << setfill('-') << "+" << setw(width + (colnames.size() * 3 )) << "+" << endl;

    cout << setfill(' ')  ;

    for(auto printable : list){
        
        
        while(printable.netxLine()){
            cout << "| ";
            for(auto name : colnames){
                //cout << name <<  endl;
                cout << setw(colsizes[name]) << printable.get(name) << " | ";
            }
            cout << endl;
        }
        cout << "| " ;
        for(auto name : colnames){
            cout << setw(colsizes[name]) << " " << " | ";
        }
        cout << endl /*<< "| "*/;
    }
    cout << setfill('-') << "+" << setw(width + (colnames.size() * 3 )) << "+" << endl;

}

void Printable::PrintJson(vector<Printable> &list){
    //std::vector<std::pair<std::string, unsigned>> m_keys;
    for(auto printable : list){
    
        
        while(printable.netxLine()){
            Json::Value object;    
        
            for(auto key : *printable.keys()){
                object[key.first] = printable.get(key.first);
            }
        
            cout << object.toStyledString() << endl;   
        }
        
    }
}

void Printable::set(string name, string val, bool canBeRepeated) {
    
    //cout << name << " " << val << endl;
    
    bool found = false;
    
    // Find if name exists
    map<string, vector<string>>::iterator it = m_values.find(name);
    
    if(it != m_values.end()){
        
        found = true;
        
        //If val cannot be reapeated
        if(! canBeRepeated){
            
            vector<string>::iterator vit = find(it->second.begin(), it->second.end(), val);
            
            if(vit == it->second.end())
                it->second.push_back(val);
            
        } else {
            it->second.push_back(val);
            
            if(it->second.size() > m_max) m_max = it->second.size();
        }
    }
    
    
    // If not
    if(! found){
        vector<string> v;
        v.push_back(val);
        m_values[name] = v;
        if(v.size() > m_max) m_max = v.size();
        //cout << v.size() << "b " << val << endl;
    }
    

    
    unsigned size = (name.size() > val.size()) ? name.size() : val.size();

    pair<string, unsigned> p(name, size);

    m_keys.push_back(p);
}

string Printable::get(string name){
    
    if(m_lineIndex < 0) m_lineIndex = 0;
    
    // Find if name exists
    for(auto &value : m_values){
        
        if(name == value.first){            
            if(value.second.size()){
                if(m_lineIndex < (int)value.second.size())
                    return value.second[m_lineIndex];
                else{
                    return "";
                }
            }
        }
    }
    return string();
}

vector<pair<string, unsigned>> *Printable::keys(){
    return &m_keys;    
}

bool Printable::netxLine(){

    m_lineIndex++;
    
    if(m_lineIndex < (int)m_max) return true;
    
    m_lineIndex = -1;
    
    return false;
}


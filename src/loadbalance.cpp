/*
 *  Created on  20/05/2013 18:54:30
 *
 *  This file is part of __________
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

#include "loadbalance.h"
#include "database.h"
#include "system.h"
#include <vector>
//#include <string>
#include <iostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <sstream>
#include <boost/algorithm/string.hpp>


using namespace std;

LoadBalance::LoadBalance(){
    
}

LoadBalance::~LoadBalance(){
    
}


void LoadBalance::load(){
    
    Database::DatabaseValues values;
    
    Database db;
    db.query("SELECT * FROM interface WHERE mode='wan'", &values);
    
    vector<string> hops;
    
    for(auto &wan : values){
        
        //Here calculate the weight: 
        
        hops.push_back("nexthop via " + wan["gateway"] + " dev " + wan["device"] + " weight 1 ");
    }
    
    string command = "ip route replace default scope global ";
    
    for(auto &hop : hops){
        command.append(hop);
    }
    
    System::execute(command);
}

void LoadBalance::addWan(string name){
    
    string filename("/etc/iproute2/rt_tables");
            
    boost::iostreams::stream<boost::iostreams::file_source> file(filename.c_str());
    
    string line;
    
    unsigned count = 0;
    
    stringstream ss;
    
    vector<string> tokens;
    
    while(getline(file, line)){
        
        if(line.size() < 3 || (line.size() && line[0] == '#')) continue;
        
        boost::split(tokens, line, boost::is_any_of("\t"));
        
        if(tokens.size()){
            if(tokens[1] == name) return; 
        }else{
            continue;
        }
        
        ss << line << endl;
        
        count ++;
    }
    //cout << ss.str();
    
    unsigned index = (count - 4) + 1;
    
    ss << index << "\t" << name << endl;
    
    ofstream myfile;
    myfile.open (filename.c_str());
    myfile << ss.str();
    myfile.close();
}
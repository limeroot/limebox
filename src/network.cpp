/*
 *  Created on  04/05/2013 14:03:30
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

#include "network.h"
#include <iostream>
#include "wan.h"
#include <vector>
#include <map>
#include "printable.h"
#include "database.h"
using namespace std;

Network::Network(Options &options){
    Database database;
    database.query("CREATE TABLE IF NOT EXISTS network(name TEXT UNIQUE, status TEXT)");
    
    m_functions["start"] = &Optionable::start;
    m_functions["stop"] = &Optionable::stop;
    m_functions["restart"] = &Optionable::restart;
    m_functions["status"] = &Optionable::status;
    
    parseOption(options);
}
 
Network::~Network(){
    
}

bool Network::up(Options &options){
    
    vector< map<string,string>  > values;
    Database database;
    database.query("SELECT status FROM network", &values);
    
    if(values.size())
        return (values[0]["status"] == "up");
    
    return false;
}

void Network::status(Options &options){
    
    vector< map<string,string>  > values;
    Database database;
    database.query("SELECT status FROM network", &values);
    
    if(values.size()){
        Printable p;
        p.set("status", values[0]["status"]);
        vector<Printable> pl;
        pl.push_back(p);
        Printable::Print(pl);
    }
}

void Network::start(Options &options){
    
    if(up(options)){
        cout << "Error: Network already running" << endl;
        return;
    }
    
    Wan wan;
    wan.start(options);
    Database database;
    database.query("REPLACE INTO network(name,status) VALUES('main','up')");
    //cout << "private" << endl;    
    //vector< map<string,string>  > wans;
    //
    //options.database.query("select * from wan", &wans);
    //
    //for(auto wan : wans){
    //    
    //    for(auto val : wan){
    //        
    //        cout << val.first << ": " << val.second << endl;    
    //    }
    //}
}

void Network::stop(Options &options){
    
    if(! up(options)){
        cout << "Error: Network is not running" << endl;
        return;
    }
    
    //Wan wan;
        
    //wan.start(options);
    Database database;
    database.query("REPLACE INTO network(name,status) VALUES('main','down')");    
}

void Network::restart(Options &options){
    
}

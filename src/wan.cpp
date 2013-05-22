/*
 *  Built on 13/04/2013 14:14:23
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
 
#include "wan.h"
#include <iostream>
#include <string> 
#include "ipaddress.h"
#include "system.h"
#include "interface.h" 
#include "wan_static.h"
#include <algorithm> 
#include "database.h"
#include "loadbalance.h"
using namespace std;

Wan::Wan(Options &options){
    //getList(options);
    //m_functions["start"] = &Optionable::start;

    m_functions["list"] = &Optionable::list;
    m_functions["json_list"] = &Optionable::json_list;    
    m_functions["use"] = &Optionable::use;    
    //Database database;
    //string query = "CREATE TABLE IF NOT EXISTS "
    //                       "wan("
    //                       "device TEXT UNIQUE,"
    //                       "name TEXT,"
    //                       "ip TEXT,"
    //                       "connection TEXT,"
    //                       "gateway TEXT,"
    //                       "bandwidth TEXT)";
    //database.query(query);
    parseOption(options);
}

Wan::Wan(){
    
}

Wan::Wan(string device){
    m_interface = device;
}

Wan::~Wan(){
}

void Wan::getList(vector<Printable> *wanList){
    
    vector< map<string,string>  > values;
    Database database;
    database.query("SELECT * FROM interface WHERE mode='wan'", &values);
    
    for(auto &wan : values){
    
        if(wan["connection"] == "dhcp"){
           string name = wan["interface"];
           Interface nic(name);
           wan["ip"] = nic.ipv4();           
        }
        
        Printable p;
        
        p.set("name",       wan["name"]);
        p.set("device",     wan["device"]);
        p.set("ip",         wan["ip"]);
        p.set("connection", wan["connection"]);
        p.set("gateway",    wan["gateway"]);
        p.set("bandwidth",  wan["bandwidth"]);
        p.set("status",     wan["status"]);
        p.set("link",       wan["link"]);
        
        wanList->push_back(p);
    }
}

void Wan::list(Options &options){
    
    vector<Printable> wanList;
    getList(&wanList);
    Printable::Print(wanList); 
}

void Wan::json_list(Options &options){
    
    vector<Printable> wanList;
    getList(&wanList);
    Printable::PrintJson(wanList);    
}


//bool Wan::isWan(string *name){
//    
//    if(name) *name = string();
//    
//    Database::DatabaseValues values;
//    
//    Database db;
//    
//    db.query("SELECT name FROM interface WHERE mode='wan' AND device='" + m_interface + "'", &values);
//    
//    if(values.size()){
//        
//        map<string,string> val = values[0];
//        
//        *name = val["name"];
//    }
//    
//    return values.size();
//}

void Wan::use(Options &options){
    
    //wan inte8 use eth0 192.168.0.4 gw 192.168.0.1 200/2000
    
    //<use> is used to create a new wan or to alter an existing one
    
    // m_mainArgument contains the wan name and cannot be empty
    string name = m_mainArgument;
    
    if(name == ""){
        cout << "Error: wan name" << endl;
        return;
    }
    
    m_interface = options.next();
    
    if(! Interface::isValid(m_interface)){
        
        cout << "Error: The device " << m_interface << " does not exists" << endl;
        return;
    }
    
    string mode = "wan";
    
    if(Interface::causesConflict(m_interface, mode, name)){
        
        cout << "Error: Conflict with \"" << mode << " " << name << " on " << m_interface << "\"" << endl;
        return;
    }
    
    string wan_mode = options.next(); 
     
    if(wan_mode == "dhcp")
    ;
    else if(wan_mode == "user" || wan_mode == "pass" || wan_mode == "password") //pppoe
    ;
    else{
        WanStatic ws;
        ws.set(m_interface, name, options);
    }
    
    LoadBalance::addWan(name);
}

//void Wan::dhcp(Options & options){
//
//    options.next();
//    
//    string bandwidth = options.next();
//    
//    string query = "REPLACE "
//                   "INTO "
//                   "wan(name,interface,ip,connection,gateway,bandwidth) "
//                   "VALUES('";
//    query.append(m_wanName).append("','");
//    query.append(m_interface).append("','");
//    query.append("','");
//    query.append("dhcp','");
//    query.append("','");
//    query.append(bandwidth).append("')");
//    options.database.query(query); 
//}
//
//void Wan::pppoe(Options & options){
//    
//}

void Wan::start(Options &options){
    
    //getList(options);
    //
    //m_enableLB = (m_wanList.size() > 1);
    //
    //vector<WanConnection*> v;
    //
    //for(auto wan : m_wanList){
    //
    //    if(wan.get("connection") == "dhcp")
    //        ;
    //    if(wan.get("connection") == "pppoe")
    //        ;
    //    if(wan.get("connection") == "static"){
    //        
    //        WanStatic *ws = new WanStatic(wan);
    //        //cout << "Trying to setup " << wan.get("interface") << endl;
    //        //ws->setUp();
    //        v.push_back(ws);
    //    }
    //}
    //
    //
    //while(v.size()){
    //    
    //    for(auto &w : v){
    //        
    //        string interface = w->interface();
    //            
    //        if(w->Up()){
    //            
    //            //cout << interface << " connection success..." << endl;    
    //            vector<WanConnection*>::iterator it;
    //            
    //            it = find(v.begin(), v.end(), w);
    //            
    //            if(it != v.end()){
    //                //cout << "deleted " << interface << endl;
    //                delete *it;
    //                *it = NULL;
    //                v.erase(it);
    //                
    //            }
    //        }// else {
    //        //    cout << interface << " not ready yet" << endl; 
    //        //}
    //    }
    //    
    //    this_thread::sleep_for(chrono::seconds(1));
    //}
}

void Wan::stop(Options &options){
    
}

void Wan::restart(Options &options){
    
}

void Wan::start(){
    
}

void Wan::stop(){
    
}

void Wan::start(string device){
    
    Database::DatabaseValues vals;
    
    Database db;
    
    db.query("SELECT connection FROM interface WHERE device='" + device + "'", &vals);
    
    if(vals.size()){
        
        if(vals[0]["connection"] == "static"){
            
            WanStatic::start(device);
        }
    }
}

void Wan::stop(string device){
    
}




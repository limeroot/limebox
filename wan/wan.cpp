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
#include "interface/interface.h" 
#include "wan_static.h"
#include <algorithm>
using namespace std;

Wan::Wan(Options &options){
    getList(options);
    m_functions["list"] = &Optionable::list;
    m_functions["json_list"] = &Optionable::json_list;    
    m_functions["use"] = &Optionable::use;    

    options.database.query("CREATE TABLE IF NOT EXISTS wan(name TEXT, interface TEXT UNIQUE, ip TEXT, connection TEXT, gateway TEXT, bandwidth TEXT)");
    parseOption(options);
}

Wan::Wan(){
    
}

Wan::~Wan(){
}

void Wan::getList(Options &options){
    
    vector< map<string,string>  > values;
    
    options.database.query("select name,interface,ip,connection,gateway,bandwidth from wan", &values);
    
    for(auto &wan : values){
    
        if(wan["connection"] == "dhcp"){
           string name = wan["interface"];
           Interface nic(name);
           wan["ip"] = nic.ipv4();           
        }
        
        Printable p;
        
        p.set("name", wan["name"]);
        
        p.set("interface", wan["interface"]);
        
        p.set("ip", wan["ip"]);
        
        p.set("connection", wan["connection"]);
        
        p.set("gateway", wan["gateway"]);
        
        p.set("bandwidth", wan["bandwidth"]);
        
        m_wanList.push_back(p);
    }
    
}

void Wan::list(Options &options){
    
    Printable::Print(m_wanList); 
}

void Wan::json_list(Options &options){

    Printable::PrintJson(m_wanList);    
}

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
    
    Interface iface(m_interface);
    
    
    //The interface exists?
    
    if(! iface.exists()){
        cout << "Error: interface \"" << m_interface << "\" does not exists." << endl;
        return;
    }
    
    
    // The interface can be asigned to only one wan
    
    bool interfaceAlreadyAssigned = false;
    
    string nameOfTheCurrentInterfaceWan = string();
    
    for( auto wan : m_wanList){
        if(wan.get("interface") == m_interface){
            interfaceAlreadyAssigned = true;
            nameOfTheCurrentInterfaceWan = wan.get("name");
            break;
        }
    }
    
    
    // If the wan name and the interface name are not the names that we are trying to change
    // then an error is thrown becuase we are trying to assing one interface to multiple wans
    if(interfaceAlreadyAssigned && (nameOfTheCurrentInterfaceWan != name)){
        cout << "An interface can only be assigned to one wan" << endl;
        return;
    }
    
    
    // If the wan name is already registered then that registry will be altered
    
    
    string mode = options.next();
     
    if(mode == "dhcp")
    ;
    else if(next == "user" || next == "pass" || next == "password") //pppoe
    ;
    else{
        WanStatic ws;
        ws.set(m_interface, name, options);    
    }
    
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
    
    getList(options);
    
    m_enableLB = (m_wanList.size() > 1);
    vector<WanStatic*> v;
    unsigned count = 1;
    for(auto wan : m_wanList){
    
        if(wan.get("connection") == "dhcp")
            ;
        if(wan.get("connection") == "pppoe")
            ;
        if(wan.get("connection") == "static"){
            
            WanStatic *ws = new WanStatic(wan);
            cout << "Trying to setup " << wan.get("interface") << endl;
            ws->setUp(count);
            v.push_back(ws);
            count++;
        }
    }
 
    
    while(v.size()){
        
        for(auto &w : v){
            
            string interface = w->interface();
                
            if(w->Up()){
                
                cout << interface << " connection success..." << endl;
                
                vector<WanStatic*>::iterator it;
                it = find(v.begin(), v.end(), w);
                if(it != v.end()){
                    cout << "deleted " << interface << endl;
                    delete *it;
                    *it = NULL;
                    v.erase(it);
                    
                }
            } else {
                cout << interface << " not ready yet" << endl; 
            }
        }
        
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void Wan::stop(Options &options){
    
}

void Wan::restart(Options &options){
    
}




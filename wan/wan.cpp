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
#include <boost/algorithm/string.hpp>
#include "interface.h"
#include <boost/algorithm/string.hpp>
#include "wan_static.h"

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
    
    if(m_mainArgument == ""){
        cout << "Error: wan name" << endl;
        return;
    }
    
    m_interface = m_mainArgument;
    
    string next = options.next();
    
    if(next == "dhcp"){ 
        dhcp(options);
    }
    else if(next == "user" || next == "pass" || next == "password"){
        pppoe(options);
    }
    else{
    
        IPAddress ip(next);
    
        if(ip.is_valid()){
            
            m_ipaddress = ip.address() + "/" + ip.prefix(); 
            _static(options);
        }
        else
            cout << "error: \"" << next << "\" is not a valid wan option" << endl; 
        
    }
}

void Wan::dhcp(Options & options){

    options.next();
    
    string bandwidth = options.next();
    
    string query = "REPLACE "
                   "INTO "
                   "wan(name,interface,ip,connection,gateway,bandwidth) "
                   "VALUES('";
    query.append(m_wanName).append("','");
    query.append(m_interface).append("','");
    query.append("','");
    query.append("dhcp','");
    query.append("','");
    query.append(bandwidth).append("')");
    options.database.query(query); 
}

void Wan::pppoe(Options & options){
    
}

void Wan::_static(Options & options){
    
    //wan inte8 use eth0 192.168.0.4 gw 192.168.0.1 200/2000
    options.next();
    
    string gw = options.next();
    
    if(! IPAddress::is_valid(gw)){
        cout << "error: \"" << gw << "\" is not a valid gateway" << endl; 
        return;
    }
    
    string bandwidth = options.next();
    
    if(! isValidBandwidthString(bandwidth)){
        cout << "error: \"" << bandwidth << "\" is not a valid bandwidth" << endl; 
        return;
    }
    
    string query = "REPLACE "
                   "INTO "
                   "wan(name,interface,ip,connection,gateway,bandwidth) "
                   "VALUES('";
    query.append(m_wanName).append("','");
    query.append(m_interface).append("','");
    query.append(m_ipaddress).append("','");
    query.append("static','");
    query.append(gw).append("','");
    query.append(bandwidth).append("')");

    options.database.query(query);
    
    string connection = "/sbin/ip link set dev " + m_interface + " up";
}

void Wan::start(Options &options){
    
    getList(options);
    
    m_enableLB = (m_wanList.size() > 1);
    
    for(auto wan : m_wanList){
    
        if(wan.get("connection") == "dhcp")
            ;
        if(wan.get("connection") == "pppoe")
            ;
        if(wan.get("connection") == "static"){
            
            WanStatic ws(wan);
            ws.connect();
        }
    }  
}

void Wan::stop(Options &options){
    
}

void Wan::restart(Options &options){
    
}

void Wan::start_dhcp(){
    
}

void Wan::start_pppoe(){
    
}

void Wan::start_static(Printable &wan){
    
    cout << "wnainx" << wan.get("interface") << endl;
    //command = "/sbin/ip link set dev" + interface + " up";
    //System::execute(command.c_str());
    
    //command = "/sbin/ip addr add " + ip + " dev " + interface;
    //System::execute(command.c_str());
    //
    //command = "/sbin/ip route add default via 192.168.0.1
 
}

bool Wan::isValidBandwidthString(std::string &bw){
    
    vector<string> words;
        
    boost::split(words, bw, boost::is_any_of("/"));
    
    if(words.size() < 2){
        return false;
    }
    
    bool ret = true;
    
    for(auto s : words){
    
        ret = (!s.empty() && std::find_if(s.begin(), 
            s.end(), [](char c) { return !std::isdigit(c); }) == s.end());
        
        if(! ret) break;
    }
    
    return ret;
}

void Wan::connectCallback(string interface){
    cout << interface << endl;
}


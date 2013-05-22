/*
 *  Built on 09/04/2013 12:07:51
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

#include <sstream>
#include <iostream>
#include "interface.h"
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <utility> 
#include "system.h"
#include <boost/filesystem.hpp>
#include "wan.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "loadbalance.h"

using boost::property_tree::ptree; using boost::property_tree::read_json; using boost::property_tree::write_json;
using namespace boost::filesystem;

using namespace std;

Interface::Interface(Options &options){
    
    m_functions["list"] = &Optionable::list;
    m_functions["json_list"] = &Optionable::json_list;
    m_functions["jsonevent"] = &Optionable::jsonevent;
   
    Interface::createDatabase();
    
    parseOption(options);
}

Interface::Interface(string device){

    m_device = device;
    
    Interface::createDatabase();
    
    Database database;
     
    Database::DatabaseValues vals;
    
    database.query("SELECT mode FROM interface WHERE device='" + device + "'", &vals);
    
    if(vals.size()){
        m_mode = vals[0]["mode"];
    } else {
        m_mode = string();
    }
    //get_list();
}

Interface::~Interface(){
    
}

bool Interface::causesConflict(string &device, string &mode, string &name){
    
    Interface::createDatabase();
    
    Database::DatabaseValues values;
    
    Database db;
    
    db.query("SELECT * FROM interface", &values);
    
    
    for(auto &interface : values){
        
        string idevice = interface["device"];
        string imode   = interface["mode"];
        string iname   = interface["name"];
        
        // If device, mode and name match then there is no problem,
        // we are editing the interface
        if(idevice == device && imode == mode && iname == name)
            return false;
        
        if(idevice == device){
            device = idevice;
            mode = imode;
            name = iname;
            return true;
        }
        
        if(iname == name){
            device = idevice;
            mode = imode;
            name = iname;
            return true;
        }
    }
    
    return false;   
}

void Interface::createDatabase(){
    
    Database::DatabaseValues values;
    
    Database db;
    
    db.query("CREATE TABLE IF NOT EXISTS "
                           "interface("
                           "device TEXT UNIQUE,"
                           "mode TEXT,"
                           "name TEXT,"
                           "ip TEXT,"
                           "connection TEXT,"
                           "gateway TEXT,"
                           "bandwidth TEXT,"
                           "link TEXT DEFAULT 'unplugged',"
                           "status TEXT DEFAULT 'down',"
                           "weight TEXT DEFAULT '0')");
    
    db.query("SELECT * FROM interface", &values);
    
}

bool Interface::isValid(string device){
    
    path someDir("/sys/class/net");
    
    directory_iterator end_iter;
    
    if(exists(someDir) && is_directory(someDir)){
        
        for( directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter){
            
                const directory_entry& entry = *dir_iter; 
            
                string ldevice = entry.path().filename().string();;
            
                if(device == ldevice) return true;
        }
    }
    
    return false;
}

void Interface::list(Options &options){

    vector<Printable> printables;

    for(auto val : m_nic_list){
        printables.push_back(val.second);
    }
    
    Printable::Print(printables);   
}

void Interface::json_list(Options &options){

    vector<Printable> printables;

    for(auto val : m_nic_list){
        printables.push_back(val.second);
    }
    
    Printable::PrintJson(printables);   
}

//void Interface::get_list(){
//    
//    vector<string> lines;
//    
//    // GET IPV4 && IPV6
//    System::execute("ip -o addr", &lines);
//
//    for(auto line : lines){
//        
//        vector<string> words;
//        
//        boost::split(words, line, boost::is_any_of(" "));
//            
//        int count = 0;
//        
//        string next_info = "";
//        
//        Printable* nic = NULL;
//        
//        for(auto word : words){
//            
//            if(next_info == "ipv4"){
//                next_info = "";
//                nic->set("ipv4", word);
//                continue;
//            }
//            else if(next_info == "ipv6"){
//                next_info = "";
//                nic->set("ipv6", word); 
//                continue;
//            }
//            //cout << next_info << endl;
//            // IF IS THE INTERFACE NAME 
//            if(count == 1){
//                if(word.size()){
//                    if(word[word.size()-1] == ':')
//                        word = word.substr(0, word.size()-1);
//                }
//                
//                nic = &m_nic_list[word];
//                
//                nic->set("name", word, false);
//                
//                //cout << "setting name: " << line << endl;
//                                
//            }
//            else if(word == "inet"){
//                
//                next_info = "ipv4";
//                continue;   
//            }
//            else if(word == "inet6"){
//                
//                next_info = "ipv6";
//                continue;   
//            }
//            else{
//                next_info = "";      
//            }
//            count++;
//        }
//    }
//    
//    // GET MAC && STATE 
//    System::execute("ip -o link", &lines);
//    
//    for(auto line : lines){
//        
//        vector<string> words;
//        
//        boost::split(words, line, boost::is_any_of(" "));
//            
//        int count = 0;
//        
//        string next_info = "";
//        
//        Printable* nic = NULL;
//        
//        for(auto word : words){
//            
//            if(next_info == "mac"){
//                next_info = "";
//                nic->set("mac", word);
//                continue;
//            }
//            else if(next_info == "state"){
//                next_info = "";
//                nic->set("state", word);
//                continue;
//            }
//            
//            // IF IS THE INTERFACE NAME 
//            if(count == 1){
//                if(word.size()){
//                    if(word[word.size()-1] == ':')
//                        word = word.substr(0, word.size()-1);
//                }
//                
//                nic = &m_nic_list[word];                
//            }
//            else if(word == "link/ether"){
//                
//                next_info = "mac";   
//                continue;   
//            }
//            else if(word == "state"){
//                
//                next_info = "state";
//                continue;   
//            } else {
//                
//                next_info = "";      
//            }
//                            
//            count++;
//        }
//    }
//}

string Interface::ipv4(){ 
    
    map<string, Printable>::iterator it;
    
    it = m_nic_list.find(m_device);
    
    if(it != m_nic_list.end()){
        
        return it->second.get("ipv4");
    }
    
    return "";
}

void Interface::jsonevent(Options &options){
    ptree pt2;
    istringstream is(options.next());
    //cout << options.current() << endl;
    read_json (is, pt2);
    string event = pt2.get<string> ("event");
    string interface = pt2.get<string> ("interface");
    
    if(event == "nic_status"){
        
        string state = pt2.get<string>("state");
        string link = pt2.get<string>("link");
        
        Database db;
        db.query("UPDATE interface SET link='" + link + "', state='" + state + "'");
        
        Interface iface(interface);
        
        // UP AD RUNNNING
        if(state == "up" && link =="plugged"){
            iface.start();
        } else{
            //DOWN
            iface.stop();
        }
    }
    

}

void Interface::start(){
    if(m_mode == "wan"){
        Wan::start(m_device);
        LoadBalance::load();
    }
}

void Interface::stop(){
   
    if(m_mode == "wan")
        Wan::stop(m_device); 
}

bool Interface::isPlugged(){
    Database::DatabaseValues vals;
    Database db;
    db.query("SELECT link FROM interface WHERE device='" + m_device + "'", &vals);

    if(vals.size()){
        map<string,string> iface = vals[0];
        return (iface["link"] == "on");
    }
    
    return false;
}








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

using namespace std;

Interface::Interface(Options &options){
    
    m_functions["list"] = &Optionable::list;
    m_functions["json_list"] = &Optionable::json_list;
    
    get_list();
    
    parseOption(options);
    //parseAction(options);
}

Interface::Interface(string name){
    
    m_name = name;
    
    get_list();
}

Interface::~Interface(){
    
}

bool Interface::exists(){
    get_list();
    map<string, Printable>::iterator it;
    it = m_nic_list.find(m_name);
    return (it != m_nic_list.end());
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



void Interface::get_list(){
    
    vector<string> lines;
    
    // GET IPV4 && IPV6
    System::execute("ip -o addr", &lines);

    for(auto line : lines){
        
        vector<string> words;
        
        boost::split(words, line, boost::is_any_of(" "));
            
        int count = 0;
        
        string next_info = "";
        
        Printable* nic = NULL;
        
        for(auto word : words){
            
            if(next_info == "ipv4"){
                next_info = "";
                nic->set("ipv4", word);
                continue;
            }
            else if(next_info == "ipv6"){
                next_info = "";
                nic->set("ipv6", word); 
                continue;
            }
            //cout << next_info << endl;
            // IF IS THE INTERFACE NAME 
            if(count == 1){
                if(word.size()){
                    if(word[word.size()-1] == ':')
                        word = word.substr(0, word.size()-1);
                }
                
                nic = &m_nic_list[word];
                
                nic->set("name", word, false);
                
                //cout << "setting name: " << line << endl;
                                
            }
            else if(word == "inet"){
                
                next_info = "ipv4";
                continue;   
            }
            else if(word == "inet6"){
                
                next_info = "ipv6";
                continue;   
            }
            else{
                next_info = "";      
            }
            count++;
        }
    }
    
    // GET MAC && STATE 
    System::execute("ip -o link", &lines);
    
    for(auto line : lines){
        
        vector<string> words;
        
        boost::split(words, line, boost::is_any_of(" "));
            
        int count = 0;
        
        string next_info = "";
        
        Printable* nic = NULL;
        
        for(auto word : words){
            
            if(next_info == "mac"){
                next_info = "";
                nic->set("mac", word);
                continue;
            }
            else if(next_info == "state"){
                next_info = "";
                nic->set("state", word);
                continue;
            }
            
            // IF IS THE INTERFACE NAME 
            if(count == 1){
                if(word.size()){
                    if(word[word.size()-1] == ':')
                        word = word.substr(0, word.size()-1);
                }
                
                nic = &m_nic_list[word];                
            }
            else if(word == "link/ether"){
                
                next_info = "mac";   
                continue;   
            }
            else if(word == "state"){
                
                next_info = "state";
                continue;   
            } else {
                
                next_info = "";      
            }
                            
            count++;
        }
    }
}

string Interface::ipv4(){
    
    map<string, Printable>::iterator it;
    
    it = m_nic_list.find(m_name);
    
    if(it != m_nic_list.end()){
        
        return it->second.get("ipv4");
    }
    
    return "";
}









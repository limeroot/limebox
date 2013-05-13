/*
 *  Created on  09/05/2013 11:56:35
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

#include "wan_connection.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include "database.h"
#include "system.h"
#include "interface.h"

using namespace std;

WanConnection::WanConnection(){
    
}

WanConnection::~WanConnection(){ 
    
}

bool WanConnection::isValidBandwidthString(std::string &bw){ 
    
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

void WanConnection::setStatus(std::string status){
    Database db;
    db.query("UPDATE wan SET status='"+ status +"' WHERE interface='" + m_interface + "'");
}

void WanConnection::devFlush(){
    m_amIUP = false;
    System::execute("/sbin/ip addr flush dev " + m_interface);
    System::execute("/sbin/ip route flush dev " + m_interface);
}

void WanConnection::devUP(){
    System::execute("/sbin/ip link set dev " + m_interface + " up");
}

string WanConnection::interfaceIPV4(){
    Interface iface(m_interface);
    return iface.ipv4();
}

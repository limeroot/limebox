/*
 *  Created on  09/05/2013 11:45:16
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

#include "wan_static.h"
#include <iostream>
#include "ipaddress.h"
#include "system.h"
#include "database.h"

using namespace std;

WanStatic::WanStatic(Printable &p): m_connectionThread(NULL){
    m_interface = p.get("interface");
    m_ipaddress = p.get("ip");
    m_amIUP = false;
} 

WanStatic::WanStatic(): m_connectionThread(NULL){
    
}

WanStatic::~WanStatic(){
    
    if(m_connectionThread){
        delete m_connectionThread;
    }
    
    m_connectionThread = NULL; 
}

void WanStatic::setUp(){
    
    devUP();
    
    devFlush();
    
    m_connectionThread = new thread(&WanStatic::tryTosetUp , this); 
    
    m_connectionThread->detach();
    
}

void WanStatic::tryTosetUp(){
    
    //this_thread::sleep_for(chrono::seconds(m_dummysecs * 5));

    System::execute("ip addr add " + m_ipaddress + " dev " + m_interface);
    
    m_setUpMutex.lock();
    
    // Determine if the change was done
    m_amIUP = (m_ipaddress == interfaceIPV4());
    
    m_setUpMutex.unlock();
}

bool WanStatic::Up(){ 
    m_setUpMutex.lock();
    bool ret = m_amIUP;
    m_setUpMutex.unlock();
    return ret;
}

string WanStatic::interface(){
    
    return m_interface;
}

void WanStatic::set(string interface, string name, Options &options){
    //wan inte8 use eth0 192.168.0.4 gw 192.168.0.1 200/2000
    
    string ipaddress = options.current();
    
    if(! IPAddress::is_valid(ipaddress)){
        cout << "error: \"" << ipaddress << "\" is not a valid ipaddress" << endl; 
        return;
    }
    
    IPAddress::getFullAddress(ipaddress);
    
    //pop otu the "gw word"
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
    
    query.append(name).append("','");
    query.append(interface).append("','");
    query.append(ipaddress).append("','");
    query.append("static','");
    query.append(gw).append("','");
    query.append(bandwidth).append("')");
    Database database;
    database.query(query);
    
    //cout << query << endl;
    //string connection = "/sbin/ip link set dev " + interface + " up";
}


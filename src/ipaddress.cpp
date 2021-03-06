/*
    Built on 12/04/2013 14:27:02
    
    This file is part of _____
    Copyright (c) 2013 LimeRoot
    www.limeroot.org, devel@limeroot.org

    Author: Daniel Torres, daniel@limeroot.org     

*/

#include "ipaddress.h"
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <iomanip>
#include "json.h"
#include "system.h"

using namespace std;

IPAddress::IPAddress(std::string ip):
    m_address(ip), m_print_cols_sizes(10, 0),m_valid(false){
    parse_ip();
}

IPAddress::IPAddress(Options &options):
    m_print_cols_sizes(10, 0){
    
    m_valid = false;
    
    m_address = options.next();
    
    parse_ip();
    
    string action = options.next();
    
    if(action == "info")
        info();
    else if(action == "json_info")
        json_info();
}

IPAddress::~IPAddress(){}

bool IPAddress::is_valid(){
    return m_valid;
}

bool IPAddress::is_valid(string sip){
    IPAddress ip(sip);
    return ip.is_valid();
}

void IPAddress::getFullAddress(std::string &sip){
    IPAddress ip(sip);
    if(ip.is_valid()){
	sip = ip.address() + "/" + ip.prefix();
    }
}

void IPAddress::info(){
    
    //    0       1        2       3               4                  5          6
    //address, prefix, netmask, hex netmask, wildcardbits_netmask, network, broadcast
    
    unsigned int div_size = 0;
    
    for(unsigned int size : m_print_cols_sizes){
        div_size += size;    
    }
    
    stringstream hdiv;
    
    hdiv << setfill('-')
         << '+'
         << setw(div_size + 21)
         << '+';
         
    cout << hdiv.str() << endl;
    
    stringstream header;
        
    header  << left << "| "
            << setw(m_print_cols_sizes[0]) << "address" << " | "
            << setw(m_print_cols_sizes[1]) << "prefix" << " | "
            << setw(m_print_cols_sizes[2]) << "netmask" << " | "
            << setw(m_print_cols_sizes[3]) << "hex netmask" << " | "
            << setw(m_print_cols_sizes[4]) << "wildcard bits" << " | "
            << setw(m_print_cols_sizes[5]) << "network" << " | "
            << setw(m_print_cols_sizes[6]) << "broadcast" << " |";

    cout << header.str() << endl;
    
    cout << hdiv.str() << endl;
        
    stringstream row;
    
    row << left << "| "
        << setw(m_print_cols_sizes[0]) << m_address<< " | "
        << setw(m_print_cols_sizes[1]) << m_prefix << " | " 
        << setw(m_print_cols_sizes[2]) << m_netmask << " | "
        << setw(m_print_cols_sizes[3]) << m_hex_netmask<< " | "
        << setw(m_print_cols_sizes[4]) << m_wildcardbits_netmask << " | "
        << setw(m_print_cols_sizes[5]) << m_network << " | "
        << setw(m_print_cols_sizes[6]) << m_broadcast << " |";
       
    cout << row.str() << endl;
    
    stringstream div;
    
    div << setfill('-')
         << '+'
         << setw(div_size + 21)
         << '+';
         
    cout << div.str() << endl;
}

void IPAddress::json_info(){
        
    Json::Value ip;    

    ip["address"] = m_address;
    
    ip["prefix"] = m_prefix;
    
    ip["netmask"] = m_netmask;
    
    ip["hex_netmask"] = m_hex_netmask;
    
    ip["wildcardbits"] = m_wildcardbits_netmask;
    
    ip["network"] = m_network;
    
    ip["broadcast"] = m_broadcast;
    
    cout << ip.toStyledString() << endl;
}

void IPAddress::parse_ip(){
    
    //m_print_cols_sizes[0] = (m_address.size() > string("address").size()) ? m_address.size() : string("address").size();   
    if(m_address == "") return;
    
    vector<string> lines;
    
    System::execute("busybox ipcalc -bnmp " + m_address, &lines);
    
    
    for(auto line : lines){
        
        if(line == "") continue;
        
        if(line.size() && line[0] == 'i'){
            return;
        }
        
        vector<string> words;
        
        boost::split(words, line, boost::is_any_of("="));
	
	if(words.size()){
	    string key = words[0];
	    string val = words[1];
	    
	    if(key == "PREFIX") m_prefix = val;
	    else if(key == "NETMASK") m_netmask = val;
	    else if(key == "BROADCAST") m_broadcast = val;
	    else if(key == "NETWORK") m_network = val; 
	}
	
	//cout << words[0] << " == " << m_prefix << endl;
	        
        //int count = 0;
        //
        //for(auto word : words){
        //    
        //    if(count == 1){
        //        
        //        m_prefix = word;
        //    }
        //    count++;
        //}
    }
    
    m_valid = true;
    
    vector<string> address;
    
    boost::split(address,m_address,boost::is_any_of("/"));
    
    m_full_address = m_address;
    
    if(address.size()){	
	m_address = address[0];
    }
    
    //m_print_cols_sizes[1] = (m_prefix.size() > string("prefix").size()) ? m_prefix.size() : string("prefix").size();   
    //            
    //command = "whatmask " + m_address + "/" + m_prefix;
    //
    //command.append(" 2>&1");
    //
    //pipe = popen(command.c_str(), "r");
    //
    //if (!pipe) cout << "ERROR";
    //
    //result = "";
    //
    //while(!feof(pipe)) {
    //	if(fgets(buffer, 1024, pipe) != NULL)
    //		result += buffer;
    //}
    //
    //pclose(pipe);
    //
    //lines.clear();
    //
    //boost::split(lines,result,boost::is_any_of("\n"));
    //
    //
    //for(auto line : lines){
    //    
    //    if(line.size() && line[0] == 'w' && line[1] == 'a'){
    //        cout << line << endl;
    //        return;
    //    }
    //    
    //    vector<string> words;
    //    
    //    boost::split(words, line, boost::is_any_of("="));
    //
    //    string next_info = "";
    //
    //    for(auto word : words){
    //        if(word == "address"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        if(word == "netmask"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        else if(word == "hex_netmask"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        else if(word == "wildcardbits_netmask"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        else if(word == "network"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        else if(word == "broadcast"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        else if(word == "usable_ips"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        else if(word == "first_usableip"){
    //            
    //            next_info = word;   
    //            continue;   
    //        }
    //        else if(word == "last_usableip"){
    //            
    //            next_info = word;   
    //            continue;   
    //        } 
    //        
    //        // NETX INFO
    //        //    0       1        2       3               4                  5          6
    //        //address, prefix, netmask, hex netmask, wildcardbits_netmask, network, broadcast
    //        if(next_info == "address"){
    //            
    //            m_address = word;   
    //            m_print_cols_sizes[0] = (next_info.size() > word.size()) ? next_info.size() : word.size();
    //            continue;   
    //        }
    //        if(next_info == "netmask"){
    //            
    //            m_netmask = word;   
    //            m_print_cols_sizes[2] = (next_info.size() > word.size()) ? next_info.size() : word.size();
    //            continue;   
    //        }
    //        else if(next_info == "hex_netmask"){
    //            
    //            m_hex_netmask = word;   
    //            m_print_cols_sizes[3] = (next_info.size() > word.size()) ? next_info.size() : word.size();
    //            continue;   
    //        }
    //        else if(next_info == "wildcardbits_netmask"){
    //            
    //            m_wildcardbits_netmask = word;
    //            m_print_cols_sizes[4] = (next_info.size() > word.size()) ? next_info.size() : word.size();
    //            continue;   
    //        }
    //        else if(next_info == "network"){
    //            
    //            m_network = word;   
    //            m_print_cols_sizes[5] = (next_info.size() > word.size()) ? next_info.size() : word.size();
    //            continue;   
    //        }
    //        else if(next_info == "broadcast"){
    //            
    //            m_broadcast = word;   
    //            m_print_cols_sizes[6] = (next_info.size() > word.size()) ? next_info.size() : word.size();
    //            continue;   
    //        }
    //        else if(next_info == "usable_ips"){
    //            
    //            m_usable_ips = word;   
    //            continue;   
    //        }
    //        else if(next_info == "first_usableip"){
    //            
    //            m_first_usableip = word;   
    //            continue;   
    //        }
    //        else if(next_info == "last_usableip"){
    //            
    //            m_last_usableip = word;   
    //            continue;   
    //        }
    //    }
    //}
    //
    //m_valid = true;
}


string IPAddress::address(){
    return m_address;
}

string IPAddress::prefix(){
    return m_prefix;
}

string IPAddress::netmask(){
    return m_netmask;
}

string IPAddress::hex_netmask(){
    return m_hex_netmask;
}

string IPAddress::wildcardbits_netmask(){
    return m_netmask;
}

string IPAddress::network(){
    return m_network;
}

string IPAddress::broadcast(){
    return m_broadcast;
}

string IPAddress::usable_ips(){
    return m_usable_ips;
}

string IPAddress::first_usableip(){
    return m_first_usableip;
}

string IPAddress::last_usableip(){
    return m_last_usableip;
}

string IPAddress::fullAddress(){
    return m_full_address;
}

/*
    Built on 12/04/2013 14:26:57
    
    This file is part of _____
    Copyright (c) 2013 LimeRoot
    www.limeroot.org, devel@limeroot.org

    Author: Daniel Torres, daniel@limeroot.org
     
*/


#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

#include <string>
#include "options.h"
#include <vector>
class IPAddress{

    public:
        IPAddress(std::string ip);
        IPAddress(Options &options);
	~IPAddress();
        
        void info();
        void json_info();
        bool is_valid();
        static bool is_valid(std::string ip);
        static void getFullAddress(std::string &ip);
        std::string     address();
        std::string     fullAddress();
        std::string     prefix();
        std::string     netmask();
        std::string     hex_netmask();
        std::string     wildcardbits_netmask();
        std::string     network();
        std::string     broadcast();
        std::string     usable_ips();
        std::string     first_usableip();
        std::string     last_usableip();
    
    private:
        std::string     m_address;
        std::string     m_full_address;
        std::string     m_prefix;
        std::string     m_netmask;
        std::string     m_hex_netmask;
        std::string     m_wildcardbits_netmask;
        std::string     m_network;
        std::string     m_broadcast;
        std::string     m_usable_ips;
        std::string     m_first_usableip;
        std::string     m_last_usableip;
        void parse_ip();
        std::vector<unsigned int> m_print_cols_sizes;
        bool m_valid;
        

};

#endif //IP_ADDRESS_H
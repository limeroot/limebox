/*
 *  Built on 13/04/2013 14:14:19
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


#ifndef WAN_H
#define WAN_H

#include "optionable.h"
#include <vector>
#include "printable.h"


class Wan : public Optionable{

    public:
        Wan(Options &options);
        Wan(); 
	~Wan();
        void start(Options &options);
        void stop(Options &options);
        void restart(Options &options);        
        //void use(Options &options);

    
    private:
        
        std::vector<Printable> m_wanList;
        bool m_enableLB;
        
        void getList(Options &options);
        void list(Options &options);
        void json_list(Options &options);
        //void parseOptions(Options &options);
        std::string m_wanName;
        std::string m_user;
        std::string m_password;
        std::string m_interface;
        std::string m_ipaddress;
      
        void use(Options &options);
        void dhcp(Options & options);
        void pppoe(Options & options);
        //void _static(Options & options);
        
        //void start_dhcp();
        //void start_pppoe();
        //void start_static(Printable &wan);
        //
        
        

};

#endif //WAN_H
/*
 *  Built on 09/04/2013 11:59:24
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


#ifndef INTERFACE_H
#define INTERFACE_H

#include "options.h"  
#include "printable.h"
#include "optionable.h"

class Interface : public Optionable{

    public:
        Interface(Options &options);
        Interface(std::string device);
        ~Interface();
        void list(Options &options);
        void json_list(Options &options);
        void start();
        void stop();
        bool isPlugged();
        
        std::string ipv4();
        static bool isValid(std::string device);
        static bool causesConflict(std::string &device, std::string &mode, std::string &name);
        
    private:
        static void createDatabase();
        void jsonevent(Options &options);
        void get_list();
        std::map<std::string, Printable> m_nic_list;
        std::string m_device;
        std::string m_mode;
        
};

#endif //INTERFACE_H
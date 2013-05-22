/*
 *  Created on  09/05/2013 11:55:46
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

#ifndef WANCONNECTION_H
#define WANCONNECTION_H

#include <string>
#include "options.h"
#include <thread>
#include <mutex>

class WanConnection{
    
    public:
        WanConnection();
        //WanConnection(std::string device);
        virtual ~WanConnection() = 0;
        bool Up();
        std::string interface();
        //static void start(std::string device);
    private:
    
    protected:
        std::mutex m_setUpMutex;
        bool m_amIUP;
        std::string m_interface;
        std::string m_ipaddress;
        bool isValidBandwidthString(std::string &bw);
        static void devFlush(std::string device);
        static void devUP(std::string device);
        void setStatus(std::string status);
        std::string interfaceIPV4();
        //std::string m_device;
};

#endif //WANCONNECTION_H

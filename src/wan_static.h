/*
 *  Created on  09/05/2013 11:44:02
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

#ifndef WANSTATIC_H
#define WANSTATIC_H

#include "printable.h"
#include <string>
#include "wan_connection.h"

class WanStatic : public WanConnection{
    
    public:
        WanStatic(Printable &p);
        WanStatic();
    
        ~WanStatic();
        void setUp();
        void set(std::string interface, std::string name, Options &options);
        static void start(std::string device);
    private:
        std::thread *m_connectionThread;
        void tryTosetUp();
};

#endif //WANSTATIC_H

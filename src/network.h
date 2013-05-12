/*
 *  Created on  04/05/2013 14:11:45
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

#ifndef NETWORK_H
#define NETWORK_H

#include "optionable.h"

class Network : public Optionable{
    
    public:
        Network(Options &options);
        ~Network();
    
    private:
        bool up(Options &options);
        void start(Options &options);
        void stop(Options &options);
        void restart(Options &options);
        void status(Options &options);
        
};

#endif //NETWORK_H

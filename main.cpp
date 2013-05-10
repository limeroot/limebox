/*
 *  Created on 09/04/2013 11:27:51
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
#include <string>
#include "options.h"
#include "interface/interface.h"
#include "wan/wan.h"
#include "network.h"

using namespace std;

int main(int argc, char** argv){
    
    Options options(argc, argv);
         
    string command = options.next();
    
    if(command == "interface") Interface interface(options);
    
    else if(command == "wan") Wan wan(options);
    
    else if(command == "network") Network network(options);
    
    else cout << "Error: Command " << command <<" not exists " << endl;

    return 0;
}


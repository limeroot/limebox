/*
 *  Created on 09/04/2013 14:31:11
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


#ifndef OPTIONS_H 
#define OPTIONS_H

#include <string>
#include <deque>
#include "database.h"

class Options{

    public:
        Options(int argc, char **argv);
	~Options();
        
        std::string next();
        std::string current();
        
        //Database database;
        
    private:
        std::deque<std::string> m_options;

};

#endif //OPTIONS_H

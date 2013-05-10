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

#include "options.h"

using namespace std;

Options::Options(int argc, char **argv){
    
    for(int i = 0; i < argc; i++){
        m_options.push_back(argv[i]);
    }
}

Options::~Options(){ 
} 

string Options::current(){
    
    if(m_options.size())
        return m_options[0];
    
    return string();
}

string Options::next(){
    
    if(m_options.size()){
        m_options.pop_front();
    } else {
        return string();
    }
    
    if(m_options.size())
        return m_options[0];
    
    return string();
}

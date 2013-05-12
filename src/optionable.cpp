/*
 *  Created on  06/05/2013 17:03:17
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

#include "optionable.h"
#include <iostream>

using namespace std;

Optionable::Optionable(){
    m_mainArgument = "";
}

Optionable::~Optionable(){
}

void Optionable::executeOption(string option, Options &options){
    
    map<string, OptionableFunction>::iterator it = m_functions.find(option);
    
    if(it != m_functions.end()){
        OptionableFunction f = it->second;
        (this->*f)(options);
    } else {
        
        // May be "option" was the main argument for the command
        m_mainArgument = option;
        
        string newoption = options.next();
        
        it = m_functions.find(newoption);
        
        
        if(it != m_functions.end()){
            
            OptionableFunction f = it->second;
        
            (this->*f)(options);
        
        } else {
            // Nope, nothing more to do
            cout << "Error: option \"" << option << "\" not implemented" << endl;
        }
    }
}

void Optionable::parseOption(Options &options){
    
    string action = options.next();
    
    executeOption(action, options);
}

void Optionable::start(Options &options){}
void Optionable::stop(Options &options){}
void Optionable::restart(Options &options){}
void Optionable::list(Options &options){}
void Optionable::json_list(Options &options){}
void Optionable::status(Options &options){}
void Optionable::use(Options &options){}


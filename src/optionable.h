/*
 *  Created on  06/05/2013 17:03:00
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

#ifndef OPTIONABLE_H
#define OPTIONABLE_H

#include <string>
#include "options.h"
#include <map>

class Optionable{
    
    public:
        Optionable();
        ~Optionable();        
        typedef void (Optionable::*OptionableFunction)(Options &);
        virtual void start(Options &options);
        virtual void stop(Options &options);
        virtual void restart(Options &options);
        virtual void list(Options &options);
        virtual void json_list(Options &options);
        virtual void status(Options &options);
        virtual void use(Options &options);
        virtual void jsoncommand(Options &options);
        virtual void jsonevent(Options &options);
 
    private:
        
    protected:
        std::string m_mainArgument;
        void parseOption(Options &options);
        virtual void executeOption(std::string option, Options &options);
        std::map<std::string, OptionableFunction> m_functions;
};

#endif //OPTIONABLE_H

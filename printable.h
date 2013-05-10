/*
 *  Created on  05/05/2013 15:39:19
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

#ifndef PRINTABLE_H
#define PRINTABLE_H

#include <string>
#include <vector>
#include <utility>
#include <map>

class Printable{
    
    public:
        Printable();
        ~Printable();
        
        void set(std::string name, std::string val, bool canBeRepeated = true);
        std::string  get(std::string name);
        bool netxLine();
        
        static void Print(std::vector<Printable> &list);
        static void PrintJson(std::vector<Printable> &list);
        
        friend bool operator> (Printable &p1, Printable &p2);
        friend bool operator< (Printable &p1, Printable &p2);
        
        std::vector<std::pair<std::string, unsigned>> *keys();
        
    private:
        unsigned m_max;
        int m_lineIndex;
        std::map<std::string, std::vector<std::string>> m_values;
        std::vector<std::pair<std::string, unsigned>> m_keys;
        
        
};

#endif //PRINTABLE_H

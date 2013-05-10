/*
 *  Created on  09/05/2013 11:45:16
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

#include "wan_static.h"

using namespace std;

WanStatic::WanStatic(Printable &p): m_connectionThread(NULL){
    
}

WanStatic::~WanStatic(){
    if(m_connectionThread)
        delete m_connectionThread;
        
    m_connectionThread = NULL;
}

void WanStatic::connect(){
    //this_thread::sleep_for(chrono::seconds(i * 5));
    //m_connectCallback = callback;
    //m_connectCallback();
    
    m_connectionThread = new thread();
}

void WanStatic::tryConnection(){
    
}

/*
    Built on 13/04/2013 09:53:25
    
    This file is part of _____
    Copyright (c) 2013 LimeRoot
    www.limeroot.org, devel@limeroot.org

    Author: Daniel Torres, daniel@limeroot.org
     
*/


#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

class System{

    public:
        System();
	~System();
        
        static void execute(std::string command, std::vector<std::string> *output_lines = NULL);
    private:

};

#endif //SYSTEM_H
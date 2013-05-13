/*
    Built on 13/04/2013 09:53:30
    
    This file is part of _____
    Copyright (c) 2013 LimeRoot
    www.limeroot.org, devel@limeroot.org

    Author: Daniel Torres, daniel@limeroot.org      

*/

#include "system.h"  
#include <algorithm>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

System::System(){
}

System::~System(){
}

void System::execute(string command, vector<string> *output_lines){
    
    if(output_lines)
	output_lines->clear();
    
    command.append(" 2>&1"); 
    
    FILE* pipe = popen(command.c_str(), "r");
    
    if (!pipe) return;// "error: cannot open pipe";
    
    char buffer[1024];
    
    string result = string();
     
    while(!feof(pipe)) {
    	if(fgets(buffer, 1024, pipe) != NULL){
            if(string(buffer) == "") continue;
            result += buffer;
    	}
    }
    
    pclose(pipe);
    
    //boost::algorithm::trim(result); 
    
    if(output_lines)
        boost::split(*output_lines,result,boost::is_any_of("\n"));
    
}



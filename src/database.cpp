/*
 * Built on 13/04/2013 17:15:14
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

#include "database.h"
#include <iostream>

using namespace std;

static int callback( void *slpm, int argc, char **argv, char **azColName );

Database::Database(){
    
    int rc = sqlite3_open( "limebox.db", &database );
       
    if( rc ){
        m_error = "error: database cannot be initialized";
        return;
    } 
}

Database::~Database(){
    sqlite3_close( database ); 
}

bool Database::query( string squery, DatabaseValues *values, bool debug){
    
    if(debug)
        cout << squery << endl;
    
    char *sqlError = 0;
    
    int rc = sqlite3_exec(database, squery.c_str(), callback, (void*)values, &sqlError );
    
    if( rc != SQLITE_OK )
    {
        m_error = sqlError;
        
        sqlite3_free( sqlError );
        
        return false;
    }
        
    return true;
}

static int callback( void *pvalues, int argc, char **argv, char **azColName )
{
    map< string, string > row;        
    
    vector< string > colNames;
    
    vector< map<string, string>  > *pv = (vector< map<string, string>  > *)pvalues;
    
    for( int i=0; i<argc; i++ )
    {
        string cn;
        
        if( azColName[i] ) cn = azColName[ i ];
        
        string vl;
        
        if( argv[ i ] ) vl = argv[ i ];

        row[ cn ] = vl;
        
        //cout << cn << endl;
        //pm->addColSize( cn, vl );
        
        //colNames.push_back( cn );
    }
    pv->push_back(row);
    //pm->addRow( row );
    //pm->addColNames( colNames );

    return 0;        
}

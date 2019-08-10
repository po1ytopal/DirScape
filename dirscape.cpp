/*
   (c) 2019 po1ytopal and A. Hernandez Jr.
   This project and all code contained is licensed under the MIT license (see LICENSE.txt for details)
*/

/**
 * @file dirscape.cpp
 *
 * @brief Main driver for dirscape project
 *
 * @author po1ytopal
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "xxhash64.h"

namespace fs = std::filesystem;

using std::cout; using std::cerr; using std::cin; using std::endl;
using std::string; using fs::directory_iterator;

void usage(char *);

int main(int argc, char* argv[]){

   string rootDir;
   fs::path rootPath;

   if (argc == 2){
      //for (int i=0; i<argc; i++) cerr << argv[i] << endl;
      rootDir = argv[1];
      rootPath = rootDir;
      if( !fs::exists(rootPath) ){
         cout << "noexist\n";
         usage( argv[0] );
         return 1;
      } else if( rootPath.has_filename() ){
         cout << "filename not dir\n";
            usage( argv[0] );
            return 1;
      }
   } else {
      usage( argv[0] );
      return 1;
   }

   std::vector<char> buffer (1024,0);        // process by 1k chunks

   uint64_t myseed = 0;

   for (const auto & file : directory_iterator(rootPath)) {
      cout << file.path().string() << ":";
      std::ifstream fin;
      if( file.exists() &&
          ( (  (file.status().permissions()&fs::perms::owner_read) != fs::perms::none ) &&
            (  (file.status().permissions()&fs::perms::group_read) != fs::perms::none ) &&
            ( (file.status().permissions()&fs::perms::others_read) != fs::perms::none ) )
         ) fin.open(file.path(), std::ifstream::binary);
      else{
         cout << "I can't access " << file.path() <<
            "\nCheck for typos, e.g. slash direction (/ for *nix \\ for Win, file permissions and try again!\n";
         return 1;
      }

      XXHash64 myhash(myseed);

      //cout << "read:";
      while(!fin.eof()) {
         fin.read(buffer.data(), buffer.size());
         auto s=fin.gcount();

         //cout << s ;        // bytes processed
         //if(!fin.eof()) cout << ",";

         myhash.add(buffer.data(), s);
      }
      cout << myhash.hash() << endl;   // 
      //cout << ";";
   }

   return 0;
}

void usage( char * arg ){

   cout << "Usage: " << arg << " path\nWhere \"path\" exists and permits read and execute by " << arg << endl;
}


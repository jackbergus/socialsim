/*
 * script_runner.cpp
 * This file is part of script_runner
 *
 * Copyright (C) 2014 - Giacomo Bergami
 *
 * script_runner is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * script_runner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with script_runner. If not, see <http://www.gnu.org/licenses/>.
 */

 

#include <fstream>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <sys/wait.h>

extern "C" {
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <signal.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <string.h>
	#include <dirent.h>
       #include <fcntl.h>

}

//Hardcoding all
#include "json.h"
#include "json.cpp"

static inline  std::string get_file_contents(const char *filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  std::string s{};
  return s;
}

char check_extension(const char* file, const char* end) {
	if (strlen(end)>strlen(file)) return (char)0;
	char toret;
	int len = strlen(file)-strlen(end);
	toret = (char)(strcmp(&file[len],end)==0);
	return toret;
}


int select_log(const struct dirent *entry)
{
   if ((strcmp(entry->d_name, ".") == 0) || 
       (strcmp(entry->d_name, "..") == 0)||
       (entry->d_type!=DT_REG) || (check_extension(entry->d_name,".log")==0))
     return (0);
   else
     return (1);
}

char *convert(const std::string & s)
{
   return (char*)s.c_str();
}

std::vector<std::string> split(const std::string& str, int delimiter(int) = ::isspace){
  std::vector<std::string> result;
  auto e=str.end();
  auto i=str.begin();
  while(i!=e){
    i=find_if_not(i,e, delimiter);
    if(i==e) break;
    auto j=find_if(i,e, delimiter);
    result.push_back(std::string(i,j));
    i=j;
  }
  return result;
}

int main(int argc, char* argv[]) {
	int pid;
	
	std::string content;
	content = get_file_contents(argv[1]);
	json::Object configuration = json::Deserialize(content);
	for (auto g : configuration) {
		pid = fork();
		if (!pid) {
			//The child executes the program
			std::cout << g.first << std::endl;
			//Splits the string by command arguments
			std::vector<std::string> vs = split(g.first);
			std::vector<char*>  vc;
			//Push the data inside a vector
			std::transform(vs.begin(), vs.end(), std::back_inserter(vc), convert);
			
			int bak, nw;
			fflush(stdout);
			fflush(stderr);
			
			//Closing stdout
			bak = dup(1);
			nw = open("/dev/null", O_WRONLY);
			dup2(nw, 1);
			close(nw);
			//Closing stderr
			bak = dup(2);
			nw = open("/dev/null", O_WRONLY);
			dup2(nw,2);
			close(nw);
			
			//Execute the command with the parsed data
			execvp(vc[0], vc.data());
		} else {
			//Father

#if 0

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			//Waits for an hour
			sleep(3600);
			std::cout << "Exit from sleep" << std::endl;
			//terminates the child
			if (kill(pid, SIGTERM)==-1) {
				std::cout << "Error" << std::endl;
			}
			//
#endif
			int status;
			if ((pid = wait(&status)) == -1) {
				std::cout << "Error" << std::endl;
			}

			struct dirent **namelist;
			int n;
			std::string second = g.second;
			n = scandir( ".", &namelist, select_log, alphasort);
			if (n>0) {
				std::cout << second << std::endl;
				mkdir(second.c_str(), 0777);
				int i;
			       	for (i=0; i<n; i++)  {
				   std::string tmpfile{namelist[i]->d_name};
				   std::string newpath = second + "/" + tmpfile;
				   rename(namelist[i]->d_name,newpath.c_str());
				   free(namelist[i]);
			       	}
			       	free(namelist);
			}
			
		}
	}
	return(0);

}


/*
 * script_maker.cpp
 * This file is part of script_maker
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
#include <list>

extern "C" {
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <signal.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <string.h>
	#include <dirent.h>
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

static inline void cp(std::string source, std::string dest) {
	FILE *exein, *exeout;
	exein = fopen(source.c_str(), "rb");
	if (exein == NULL) {
	    /* handle error */
	    perror("file open for reading");
	    exit(EXIT_FAILURE);
	}
	exeout = fopen(dest.c_str(), "wb");
	if (exeout == NULL) {
	    /* handle error */
	    perror("file open for writing");
	    exit(EXIT_FAILURE);
	}
	size_t n, m;
	unsigned char buff[8192];
	do {
	    n = fread(buff, 1, sizeof buff, exein);
	    if (n) m = fwrite(buff, 1, n, exeout);
	    else   m = 0;
	} while ((n > 0) && (n == m));
	if (m) perror("copy");
	if (fclose(exeout)) perror("close output file");
	if (fclose(exein)) perror("close input file");
}


int main(int argc, char* argv[]) {
	std::string CONF{"conf"};
	std::string GRAPH{"graph"};
	std::string OPSEED{"opinionSeed"};
	std::string SET{"setting"};
	std::string SIGMA{"SIGMA"};
	std::string SAME{"sameEvent"};
	std::string EVENT{"stopAtEvent"};
	
	std::string content;
	//Loading the base configuration
	content = get_file_contents(argv[1]);
	json::Object configuration = json::Deserialize(content);
	std::string  base_name{argv[1]};
	
	//generating all the possible graphs
	std::list<std::string> graphs;
	graphs.push_back("lattice_5x6.xml");
	//graphs.push_back("lattice_10x10.xml");
	
	//Generating all the possible opinion seed
	std::list<int> opSeed;
	for (int i=0; i<10; i++)
		opSeed.push_back(i);
	
	//Generating all the possible SIGMAS
	std::list<double> SIGMAS;
	SIGMAS.push_back(0.8);
	
	//Generating all the possible Settings
	std::list<int> Settings;
	for (int i=1; i<=8; i++) {
		Settings.push_back(i);
	}
	
	std::cout << "Generating " << 2*graphs.size()*Settings.size()*opSeed.size() << "events for each Sigma" << std::endl;
	int j = 0;
	//Generating all the other configurations
	for (double sigma : SIGMAS) {
		for (int same_event = 0; same_event<=0; same_event++) {
		
								//Creating a orchestration configuration file
					
		
			for (std::string g : graphs) {
				int i;
				for (i=0; i<4; i++) {
				
				json::Object obj;
					std::string orchfilen = "sigma_" + std::to_string(sigma) + "-same_" + std::to_string(same_event) + "group=" + std::to_string(i);
					mkdir(orchfilen.c_str(),0777);
				
					int j;
					for (j=1; j<3; j++) 
					for (int seed : opSeed) {
						int set = i*2+j;
						json::Object def = configuration[CONF];
						def[GRAPH] = (std::string)g;
						def[OPSEED] = (int)seed;
						def[SET] = std::to_string(set);
						def[SIGMA] = std::to_string(sigma);
						def[SAME] = same_event;
						configuration[CONF] = def;
						configuration[EVENT] = (set <= 3 ? 10000 : 50000);
					
						//Naming the configuration file
						std::string filename = SIGMA + "-" + std::to_string(sigma) + "=";
						filename = filename + GRAPH + "-" + g + "=";
						filename = filename + OPSEED + "-" + std::to_string(seed) + "=";
						filename = filename + SET + "-" +std::to_string(set)+ "=";
						filename = filename + "SAME-" + (same_event ? "true" : "false");
						filename = filename + base_name;
				
						//Saving the single configuration file
						std::ofstream out(orchfilen + "/" + filename +".json");
						out << json::Serialize(configuration);
						out.close();
						obj["../src/socialsim " + filename + ".json 10"] = filename;
					}
					
				//Saving the configuration
					std::ofstream orch(orchfilen + "/orchestration");
					orch << json::Serialize(obj);
					orch.close();
					std::string slink = orchfilen + "/graphview";
					symlink("../src/graphview",slink.c_str());
					slink = orchfilen + "/socialsim";
					cp("script_runner",orchfilen + "/script_runner");
					cp("../src/socialsim",orchfilen + "/socialsim");
					
				}
				
				

				
				
			}
			
					

			

		}
		
	}
	
	
	
	return(0);

}


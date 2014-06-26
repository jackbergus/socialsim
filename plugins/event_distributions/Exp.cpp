/*
 * Exp.cpp
 * This file is part of socialsim
 *
 * Copyright (C) 2013 - Giacomo Bergami
 *
 * socialsim is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * socialsim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with socialsim; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */
 
#include <iostream>
#include "event_distributions.h"

class Exp : public Distribution {
private: 
		private:
		std::default_random_engine e;
		std::uniform_real_distribution<double> distrib; 
		double lam;
		double getRand() { return distrib(e); } 		//Generatore di numeri casuale
		
	public:
		Exp() :  distrib{}, lam{1} {};				//Inizializzazione necessaria per inserirlo in liste
		Exp(int seed, double lambda) : e{seed}, lam{lambda} {};		//Inizializzazione con parametro
		double next(Vertex* src, Vertex* dst) { return -(1/lam) * log(1-this->getRand()); }; //Definizione della funzione calcolata
		double start(double none) { return 0; }
};



class ExpCreator : public DistrCreator {	//Il plugin è il costruttore della distribuzione esponenziale, accettante un oggetto json
	public:
		Exp* creator(json::Object o) {	//Accesso ad un oggetto json come parametro
			double lam = o["lambda"];//Ottengo il parametro lambda
			int seed = o["seed"];   //Ottengo il seed da parametro
			std::cout << "INITIALIZED WiTH args." << std::endl;
			return new Exp(seed, lam);	//Inizializzazione dell'oggetto
		}
};

MAKE_PLUGIN(ExpCreator,DistrCreator);	//Definisce che il plugin è ExpCreator, particolare istanza di DistrCreator


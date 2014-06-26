/*
 * Plan1.cpp
 * This file is part of socialsim
 *
 * Copyright (C) 2014 - Giacomo Bergami, Elena Tea Russo
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
 
 

#include "agent_plan.h"


#include <math.h>
#include <string>
#include <iostream>
#include <memory>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>




class Plan1 : public Plan {
		
		


		
	public:
		Plan1() {}; //Costruttore base
		std::string getName() { return "Plan1"; } //Nome del piano

	
		//int process(json::Object* state, Message msg) <<--- XXX questa era la versione vecchia
		int process(json::Object* state, Message msg,double NormalDistributionValue) { //Nome di *ogni* azione
			std::cout << "In Plan1" << std::endl; 
			std::cout << "horror" << NormalDistributionValue << std::endl;
			
			//Se nessuno ha inviato il messaggio, ma è quello di "inizio computazione"
			if (msg.getAgentSender() == nullptr) {
				std::cout << "NULL" <<std::endl;
				//Semplicemente inoltro il mio stato
				doSend(state,0);
				//Esco dal ragionamento
				return 50;
			}
			
			//Ottiene l'opinione su di un determinato tipo di argomento, sul tipo del messaggio
			double opinion = getOpinionType(state,msg.getType());
			//Ottengo l'opinione del mittente sull'argomento
			double op_msg  = msg.getOpinion();
			//Ottengo il valore della plagiabilità
			double plagiability = getPlagiability(state);
			
			// Introduco l'errore per la valutazione
			std::cout << "valerror=" << NormalDistributionValue << std::endl; // Già corretto prima
			
			
			//Valutazione
			double valutation;
			double reputation = getSenderReputation(state,msg);
			double rad=0;
			
			std::cout << "RADICANDO: " << 0.5 << "*(1-" << abs(op_msg-opinion) << "+"<< reputation <<") +" << NormalDistributionValue << std::endl;
			
			rad=0.5*(1 - abs(op_msg-opinion) + reputation) + NormalDistributionValue ;
			if(rad>=1.) rad=1.;
			if(rad<=0) rad=0;
			
			valutation= sqrt( rad );
			
			std::cout << "VALUTATION : " <<  valutation << std::endl;

			//modifica dell'opinione nello stato interno
			
			std::cout << "OPINION +=:(( " <<  op_msg << "-" << opinion << ")*" << plagiability << "*" << "*" << valutation << std::endl;
			
			opinion += ((op_msg - opinion)* plagiability *valutation);
			std::cout << "New Opinion: " << opinion << std::endl;
			std::cout << "Reputation: " << reputation << std::endl;
			setOpinionType(state,msg.getType(),opinion);
			
			//mando un messaggio di tipo prefissato
			//Con questo comando però non invio ancora il messaggio,
			//questo verrà veramente inviato solamente alla fine del
			//processo di ragionamento
			doSend(state,msg.getType());
			//Per non inviare un messaggio:     	notSend(state)
			
			return 50; //Salto al piano in posizione 50 dalla corrente (andando in avanti o indietro)
		};
};

MAKE_PLUGIN(Plan1,Plan); //Creazione di un plugin

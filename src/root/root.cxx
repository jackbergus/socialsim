/*
 * root.cxx
 * This file is part of socialsim
 *
 * Copyright (C) 2014 - Giacomo Bergami
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
 * along with socialsim. If not, see <http://www.gnu.org/licenses/>.
 */


#include "CernOWFile.h"
#include "CernLists.h"


int main(void) {

#if 0
	//Intestazione della tabella (è un pro-forma. Ti è utile?)
	CernHeader ch;
	ch << "e";
	ch << "f";
	ch << "g";
	ch << "h";
	//Salvataggio della tabella sul file
	CernOWFile cw("output",ch);
	//Creazione del record
	CernRecord crc{"elem"};
	crc << 80;
	crc << 2;
	crc << 2;
	crc << 2.56;
	//Replico per due volte il record sul file
	cw << crc;
	cw << crc;
	//Lo salvo
	cw.close();
	//std::cout << cw.nlines() << " " << cw.ncolumns() << std::endl;
#endif
	
	//Ora apro il file
	CernOWFile cr{"../Ising.json_lattice_5x6.xml_1_12.log"};
	//Lo converto in matrice - intanto fa una veloce letta dei valori di riga e colonna
	TMatrixD td = cr.toDoubleMatrix();
	//Test sul valore
	std::cout << cr.nlines() << " " << cr.ncolumns() << std::endl;
	std::cout << td(0,0) << std::endl;
	
	return 0;
	
}

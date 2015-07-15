/*
 * libfile-stringarray
 *
 * Copyright 2014-2015 Matteo Alessio Carrara <sw.matteoac@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

/*Funzione main() di esempio*/
#include "libfsa.c" /*Attenzione che deve essere il primo include*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char **arrayprova=FileStringArray("/home/matteo/prova.txt", ';');
    long long unsigned i=0, nelem;
    switch(erroriFSA)
        {
        case FILE_VUOTO:
			printf("Il file è vuoto!\n");
            exit(EXIT_FAILURE);
		case CARATTERI_MANCANTI:
			printf("File troppo grande, alcuni caratteri finali saranno mancanti\n");
			break;
		case SEPARATORE_FINALE_MANCANTE:
			printf("Separatore dopo l'ultimo elemento mancante, l'ultimo elemento verrà ignorato\n");
			break;
        case ERRORE_FOPEN:
            printf("Problema con l'apertura del file!\n");
            exit(EXIT_FAILURE);
        case ERRORE_MALLOC:
            printf("Problema con malloc()!\n");
            exit(EXIT_FAILURE);
		case ERRORE_FREAD:
			printf("Errore in fread()\n");
			exit(EXIT_FAILURE);
		case ERRORE_FTELLO:
			printf("Errore in ftello()\n");
		case NESSUN_ERRORE:
            printf("Tutto ok, nessun errore\n");
			break;
        default:
            printf("In teoria non dovresti leggere qui, se lo stai facendo allora la funzione è buggata lol\nInformazioni sull'errore: erroriFSA vale %d e non era previsto.\n", erroriFSA);
        }
	nelem=NumeroElementiFSA(arrayprova);
	printf("%llu elementi\n", nelem);
    for (i=0; i<nelem; i++) printf("Elemento %llu: %s\n", i+1, arrayprova[i]);
	EliminaFSArray(arrayprova);
    return 0;
}

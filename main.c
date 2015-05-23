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
#include <stdio.h>
#include <stdlib.h>
#include "libfsa.c"

int main(void)
{
    char **arrayprova=FileStringArray("/home/matteo/prova.txt", ';');
    long long unsigned i=0, nelem;
    switch(erroriFSA)
        {
        case NESSUN_ELEMENTO:
			printf("Il file è vuoto!\n");
            exit(EXIT_FAILURE);
        case PROBLEMA_APERTURA_FILE:
            printf("Problema con l'apertura del file!\n");
            exit(EXIT_FAILURE);
        case PROBLEMA_MALLOC:
            printf("Problema con malloc()!\n");
            exit(EXIT_FAILURE);
        case PROBLEMA_FILE:
            printf("Problema con il file!\n");
            exit(EXIT_FAILURE);
		case ERRORE_FREAD:
			printf("Errore in fread()\n");
			exit(EXIT_FAILURE);
		case NESSUN_ERRORE:
            printf("Tutto ok, nessun errore\n");
			break;
        default:
            printf("In teoria non dovresti leggere qui, se lo stai facendo allora la funzione è buggata lol\nInformazioni sull'errore: erroriFSA vale %d e non era previsto.\n", erroriFSA);
        }
	nelem=NumeroElementiFSA(arrayprova);
	printf("%llu elementi\n", nelem);
    for (i=0; i<nelem; i++) printf("Elemento %llu: %s\n", i+1, arrayprova[i]);
    return 0;
}

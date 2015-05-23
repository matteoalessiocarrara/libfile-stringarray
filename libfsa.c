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

/*Questo file contiene le varie funzioni*/

#ifndef LIBFSA_C
#define LIBFSA_C

#include "def.h"
#include "modificare.h"

#if FSA_OS==FSA_OS_LINUX

#include <malloc.h>
#include <stdbool.h>

typedef enum {NESSUN_ERRORE=0, NESSUN_ELEMENTO, PROBLEMA_MALLOC, PROBLEMA_APERTURA_FILE, PROBLEMA_FILE, ERRORE_FREAD} ENUM_ERRORI_FSA;
ENUM_ERRORI_FSA erroriFSA=NESSUN_ERRORE; /*usare per controllare gli errori, viene modificato dalle funzioni in questo file*/

long long unsigned NumeroElementiFSA(char **array)
{
	register long long unsigned i=0;
    for(; array[i]!=NULL; i++); /*NULL è l'ultimo elemento dell'array*/
    return i;
}

void EliminaFSArray(char **array)
{
    register long long unsigned i=0, numeroElementi=NumeroElementiFSA(array);
    for(; i<numeroElementi; i++) free(array[i]); /*TODO Aggiungere opzione per copiare NULL in tutti i puntatori, o solo in array, per ricordare che non è più utilizzabile*/
    free(array);
}

char** FileStringArray(const char _file[], const char separatoreElementi)
/*return: NULL in caso di errore, e la variabile erroriFSA contiene il codice dell'errore (vedere l'enum ENUM_ERRORI_FSA)*/
{
	#define FSAEsciErrore(codiceErrore)\
	{\
        erroriFSA=codiceErrore;\
        return NULL;\
	}
	#define NByteFile(file, numeroCaratteri)\
	/*restituisce la dimensione di file in byte (verrà inserita in numeroCaratteri)*/\
	/*ATTENZIONE!! file deve essere il puntatore restituito da fopen(), e con questa funzione si perderà la posizione nel file*/\
	{\
		fseek(file, 0, SEEK_END);\
		numeroCaratteri=ftell(file);\
	}

    FILE *f; /*il file _file*/
    long long unsigned ncaratteri=0, nseparatori=0, inizioElemento=0, i, ii;
    int tmp;
    char *ramfile, **puntatoreElementi;

    if ((f=fopen(_file, "r"))==NULL) FSAEsciErrore(PROBLEMA_APERTURA_FILE);
    NByteFile(f, ncaratteri); /*conta i caratteri*/
    if(!ncaratteri) FSAEsciErrore(NESSUN_ELEMENTO);
    ramfile=(char*)malloc(sizeof(char)*ncaratteri); /*crea una copia del file nella ram, prendo la ram necessaria*/
	/*ATTENZIONE: SOPRA NON HO MESSO ncaratteri+1 (1 per NULL) PERCHÉ È PREVISTO UN SEPARATORE ANCHE DOPO L'ULTIMO ELEMENTO, CHE DIVENTERÀ NULL*/
    if(ramfile==NULL) FSAEsciErrore(PROBLEMA_MALLOC);
    rewind(f);
	if (fread(ramfile, 1, ncaratteri, f)<ncaratteri) FSAEsciErrore(ERRORE_FREAD); /*copia il file nella ram*/
	fclose(f);
	for(i=0; i<ncaratteri; i++) /*sostituisce i separatori con NULL*/
	{
		if(ramfile[i]==separatoreElementi)
			{
				ramfile[i]=0; 
				nseparatori++;
			}
	}
    if((puntatoreElementi=(char**)malloc(sizeof(char*)*(nseparatori+1)))==NULL) /*array che verrà restituito, terminato da puntatore a NULL*/
    {
        free(ramfile);
        FSAEsciErrore(PROBLEMA_MALLOC);
    }
    puntatoreElementi[nseparatori]=NULL;
    for(ii=i=0; i<ncaratteri; i++) /*inserisce gli indirizzi nell'array di puntatori*/
    {
        if (ramfile[i]==0)
        {
            puntatoreElementi[ii++]=ramfile+inizioElemento;
            if((i+1)<ncaratteri) inizioElemento=i+1;
        }
    }
    return puntatoreElementi;
	#undef FSAEsciErrore /*non serve fuori da questa funzione*/
	#undef NByteFile /*come sopra*/
}

#else /*#if FSA_OS==FSA_OS_LINUX*/
#error Queste funzioni non sono ancora state scritte per il s.o. scelto
#endif /*#if FSA_OS==FSA_OS_LINUX*/
#endif /*#ifndef LIBFSA_C*/
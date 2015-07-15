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

#define SIZE_T_MAX ((1<<(sizeof(size_t)<<3))-1)

#include "def.h"
#include "modificare.h"
#include "autodef.h"

#if COMPILATORE==COMPILATORI_GCC
/*ATTENZIONE!! Questi define devono essere prima di ogni #include con header di sistema!*/
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#endif

/*C standard library*/
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <errno.h>
#include <inttypes.h>
#ifdef POSIX
/*C POSIX library*/
#include <sys/types.h>
#endif

typedef enum 
{
	NESSUN_ERRORE=0,
	FILE_VUOTO,
	CARATTERI_MANCANTI, /*alcuni caratteri finali non sono stati considerati, perché il numero di caratteri era troppo grande e non stava in una variabile (vedere dove viene utilizzato questo errore nel file, per una migliore spiegazione del problema)*/
	SEPARATORE_FINALE_MANCANTE, /*dopo l'ultimo elemento mancava il separatore, quindi non è stato considerato. Piccolo bug (facilmente risolvibile): questo errore non si attiva, se nel file ci sono 0 separatori e (ovviamente) manca il separatore finale. Questo errore può essere una conseguenza dell'errore CARATTERI_MANCANTI*/	
	ERRORE_MALLOC, 
	ERRORE_FOPEN, 
	ERRORE_FREAD, 
	ERRORE_FTELLO
} ENUM_ERRORI_FSA;

/*Viene modificata dalle funzioni in questo file, utilizzarla in caso di errore per capire quale è l'errore precisamente*/
ENUM_ERRORI_FSA erroriFSA=NESSUN_ERRORE; /*0, false*/

#if FSA_OS==FSA_OS_LINUX
/*POSIX autodefinito*/

off_t NumeroElementiFSA(char **array)
{
	register off_t i=0;
    while (array[i]!=NULL) i++; /*NULL è l'ultimo elemento dell'array*/
    return i;
}

void EliminaFSArray(char **array)
{
	array[0]=NULL; /*per ricordare che è inutilizzabile ora*/    
    for(register off_t i=0, numeroElementi=NumeroElementiFSA(array); i<numeroElementi; i++)
		free(array[i]);
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
	#define NByteFile(_file1, numeroCaratteri)\
	/*restituisce la dimensione di "file" in byte (verrà inserita in "numeroCaratteri")*/\
	/*ATTENZIONE!! "file" deve essere il puntatore restituito da fopen(), e con questa funzione si perderà la posizione nel file*/\
	{\
		fseek(_file1, 0, SEEK_END);\
		if ((numeroCaratteri=ftello(_file1))==-1)\
			FSAEsciErrore(ERRORE_FTELLO);\
	}

    FILE *f; /*il file _file*/
    register off_t i, nseparatori=0;
	off_t ncaratteri=0, inizioElemento=0;
    char *ramfile, **puntatoreElementi;
	
    if ((f=fopen(_file, "r"))==NULL) FSAEsciErrore(ERRORE_FOPEN);
    NByteFile(f, ncaratteri); /*conta i caratteri*/
    if(ncaratteri==0) FSAEsciErrore(FILE_VUOTO);
	if (ncaratteri>(size_t)SIZE_T_MAX)
	/*una funzione sotto, fread(), richiede una variabile di tipo size_t ma l'originale è off_t; controllo se la variabile off_t contiene un numero troppo grande*/
	{		
		erroriFSA=CARATTERI_MANCANTI;
		ncaratteri=SIZE_T_MAX;
	}
    if((ramfile=malloc(sizeof(char)*ncaratteri))==NULL) /*crea una copia del file nella ram, prendo la ram necessaria*/
	/*ATTENZIONE: SOPRA NON HO MESSO ncaratteri+1 (1 per '\0' alla fine) PERCHÉ È PREVISTO UN SEPARATORE ANCHE DOPO L'ULTIMO ELEMENTO, CHE DIVENTERÀ '\0'*/		
		FSAEsciErrore(ERRORE_MALLOC);
    rewind(f);
	if (fread(ramfile, 1, ncaratteri, f)<ncaratteri) FSAEsciErrore(ERRORE_FREAD); /*copia il file nella ram*/ 
	fclose(f);
	for(i=0; i<ncaratteri; i++) /*sostituisce i separatori con '\0' e conta i separatori*/
	{
		if(ramfile[i]==separatoreElementi)
			{
				ramfile[i]=0; 
				nseparatori++;
			}
	}
    if((puntatoreElementi=(char**)malloc(sizeof(char*)*(nseparatori+1)))==NULL) /*array che verrà restituito, terminato da puntatore NULL*/
    {
        free(ramfile);
        FSAEsciErrore(ERRORE_MALLOC);
    }
    puntatoreElementi[nseparatori]=NULL;
    for(nseparatori=i=0; i<ncaratteri; i++) /*inserisce gli indirizzi delle stringhe nell'array di puntatori*/
    {
        if (ramfile[i]==0)
        {
            puntatoreElementi[nseparatori++]=ramfile+inizioElemento;
            if((i+1)<ncaratteri) inizioElemento=i+1;
			else inizioElemento=0;
        }
    }
	if (inizioElemento) erroriFSA=SEPARATORE_FINALE_MANCANTE;
    return puntatoreElementi;
	#undef FSAEsciErrore /*non serve fuori da questa funzione*/
	#undef NByteFile /*come sopra*/
}

#else /*#if FSA_OS==FSA_OS_LINUX*/
#error Queste funzioni non sono ancora state scritte per il s.o. scelto
#endif /*#if FSA_OS==FSA_OS_LINUX*/
#endif /*#ifndef LIBFSA_C*/
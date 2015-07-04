24/5/2015
---------
 - Aggiunto nuovo file autodef.h
    
**Cambiamenti nei file**
    
def.h:
    - Aggiunto define per utilizzare codice diverso con diversi compilatori
libfsa.c:
    - Ora FileStringArray() dovrebbe poter gestire file più grandi
    - Codice migliorato
    - Modificati gli errori
    - Adesso FileStringArray() avvisa quando manca il separatore dopo l'ultimo elemento
    - Cambiati "vari long long unsigned" in "off_t" (il tipo del return di ftello(), la funzione utilizzata per ottenere la dimensione del file)
    - La funzione EliminaFSArray() fa puntare il primo elemento dell'array a NULL
    - Eliminate due variabili, meno memoria utilizzata! :D
main.c
    - Modificata per funzionare con la nuova versione di libfsa.c
    - Ora utilizza tutte le funzioni disponibili in libfsa.c
    modificare.c
    - Ora è possibile specificare il compilatore
README.md
    - Eliminato un piccolo problema grafico
    - Aggiornato

23/5/2015
---------
 - Prima versione
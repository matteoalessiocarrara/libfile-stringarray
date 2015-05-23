# libfile-stringarray
Funzioni in c per leggere e dividere un file di testo in un array di stringhe

### Sinossi

#include "libfsa.c"

char** FileStringArray(const char _file[], const char separatoreElementi);
long long unsigned NumeroElementiFSA(const char **array);
void EliminaFSArray(char **array);

### Descrizione

** Prima di tutto, dovreste modificare il file "modificare.h". (In realtà per ora non c'è niente da modificare, ma in futuro forse si) **
* FileStringArray() * trasforma il contenuto di un file di testo ASCII (una stringa) in un array di stringhe. Il file viene diviso utilizzando un carattere (chiamato appunto separatoreElementi).
** Attenzione **: il carattere che divide gli elementi deve essere presente ** dopo ogni elemento, anche dopo l'ultimo **.
Altrimenti, l'ultimo non verrà considerato.
* EliminaFSArray() * esegue un free() su ogni elemento dell'array, e alla fine sull'array stesso, ma non mette a NULL i puntatori.

### Return

FileStringArray() restituisce un puntatore a **char se tutto va bene, altrimenti NULL.

### Errori

La variabile globale erroriFSA se non ci sono errori è uguale a 0 (NESSUN_ERRORE) e quindi false, altrimenti è true e il valore contenuto può essere confrontato con gli elementi dell'enum ENUM_ERRORI_FSA per capire cosa non ha funzionato.
Avviso: un errore, NESSUN_ELEMENTO, ha un nome leggermente sbagliato; significa infatti che nel file non ci sono proprio caratteri, non che nell'array non ci sono elementi.
 
### Bug e suggerimenti

La mia email è scritta all'inizio di ogni file; considerate però che non sono un esperto e programmo per divertimento, alcune volte potrei non riuscire a correggere dei bug.
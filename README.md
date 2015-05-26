# libfile-stringarray
Funzioni in c per leggere e dividere un file di testo in un array di stringhe

### Sinossi

\#include "libfsa.c"

char** FileStringArray(const char _file[], const char separatoreElementi);  
off_t NumeroElementiFSA(char \*\*array);  
void EliminaFSArray(char \*\*array);

### Descrizione

**Prima di tutto, dovreste modificare il file "modificare.h". (In realtà per ora non c'è niente da modificare, ma in futuro forse si)**
* FileStringArray() trasforma il contenuto di un file di testo ASCII (una stringa) in un array di stringhe. Il file viene diviso utilizzando un carattere (chiamato appunto separatoreElementi).
**Attenzione**: il carattere che divide gli elementi deve essere presente **dopo ogni elemento, anche dopo l'ultimo**.
Altrimenti, l'ultimo non verrà considerato (errore SEPARATORE_FINALE_MANCANTE).
* EliminaFSArray() esegue un free() su ogni elemento dell'array, e alla fine sull'array stesso. Inoltre il primo puntatore dell'array diventa NULL.

### Limiti

FileStringArray() può gestire file grandi fino a 2^nbit(size_t)-1 o 2^(nbit(off_t)-1)-1 byte, il valore più piccolo fra i due. Nel caso il file fosse più grande, i caratteri dopo l'ultimo carattere utilizzabile non verranno considerati, e verrà attivato l'errore CARATTERI_MANCANTI (probabilmente sovrascritto da SEPARATORE_FINALE_MANCANTE, prima dell'uscita dalla funzione).

### File

* libfsa.c: contiene le funzioni, è l'unico da utilizzare con \#include
* modificare.h: tutto ciò che dovreste modificare prima di usare le funzioni, si trova qui
* main.c: funzione main() di esempio, può essere utilizzata per fare delle prove
* def.h: file con alcuni define **da non modificare**.
* autodef.h: file con alcune istruzioni per attivare e disattivare dei define, non modificare 

Libfsa.c richiede i file def.h, autodef.h e modificare.h (dovrebbero essere nella stessa directory, ma questo è modificabile); main.c può essere eliminato.

### Return

FileStringArray() restituisce un puntatore a \*char se tutto va bene, altrimenti NULL.

### Errori

La variabile globale erroriFSA se non ci sono errori è uguale a 0 (NESSUN_ERRORE) e quindi false, altrimenti è true e il valore contenuto può essere confrontato con gli elementi dell'enum ENUM_ERRORI_FSA (contenuto in libfsa.c) per capire cosa non ha funzionato. Per alcuni errori, come ERRORE_FTELLO, ERRORE_FREAD o comunque relativi ad altre funzioni, potrebbero essere disponibili informazioni aggiuntive sull'errore.
 
### Bug e suggerimenti

La mia email è scritta all'inizio di ogni file; considerate però che non sono un esperto e programmo per divertimento, alcune volte potrei non riuscire a correggere dei bug.

// gcc strrev.c -o strrev -pthread
/*
   Firele de executie(ale aceluiasi proces) impart resursele si vad modificarile facute in spatiul procesului de oricare dintre ele, 
   fara a fi nevoie de o structura intermediara (ca la fork).
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void *Str_Rev(void *sir) // Functie care inverseaza sirul
{
    char *str = (char *)sir;

    int lung = strlen(str);

    char *rez = (char*) malloc(lung + 1);

	for(int i = 0; i < lung; i++) 
        rez[lung - 1 - i] = str[i];

	return rez;
}

int main(int argc, char *argv[])
{
    if (argc != 2) // daca este nr necorespunzator de argumente
    {
        perror("Eroare -> argumentul string lipseste\n");
        return errno;
    }

    char *initial = argv[1];
    pthread_t thr;  // Creez si lansez noul fir de executie = thread

    if( pthread_create(&thr, NULL, Str_Rev, initial) ) 
    //  thr primeste noul fir de executie = thread
    //  ce a fost lansat de fct Str_Rev prin argumentul initial == argv[1]
    {
        perror("Eroare -> la crearea thread-ului\n");
        return errno;
    }

    //Pentru a astepta finalizarea executiei unui thread
    char *result = NULL;
    if( pthread_join(thr, (void**)&result) )
    {
		perror("Eroare -> la executia thread-ului\n");
		return errno;
	}

	printf("StrRev: %s\n", result);
    
	free(result); //eliberez memoria

    return 0;
}

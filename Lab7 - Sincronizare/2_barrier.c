// gcc barrier.c -o barrier -pthread
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

sem_t sem; // pentru a astepta la bariera
pthread_mutex_t mtx; // pentru contorizarea firelor ajunse la bariera
int procese ; // numarul de procese

void barrier_point()
{
    static int vizitate = 0; // procesele care au vizitat deja bariera 

    pthread_mutex_lock( &mtx ); // blochez mutexul pentru a lucra cu nr de procese
    vizitate ++;
    pthread_mutex_unlock( &mtx );  // deblockez mutexul

    if (procese == vizitate)  // au trecut toate procesele 
    {
        sem_post(&sem); // verifica daca sunt thread-uri blocate la semafor
                        // eliberandu-l pe cel care asteapta de cel mai mult timp
        return;
    }

    sem_wait( &sem );
    //scade valoarea cu o unitate; daca este 0, asteapta sa fie mai intai incrementata

    sem_post( &sem );  // am iesit din semafor
    // creste valoarea cu 1 si verifica daca sunt thread-uri blocate la semafor
    // eliberandu-l pe cel care asteapta de cel mai mult timp
}

void *tfun (void *v) //functia executata de fiecare fir
{
	int *tid = (int *) v; // tid este numarul threadului pornit

	printf("%d reached the barrier \n",*tid);

	barrier_point();

	printf("%d passed the barrier \n", *tid);

	free(tid);

	return NULL;
}


int main()
{
	sem_init(&sem, 0, 0); // creez semaforul, iar initial va avea valoarea 0

	if( pthread_mutex_init( &mtx, NULL) ) // creez mutex-ul
    {
          perror("Eroare -> Creare Mutex.\n");
          return errno ;
    }

    printf("Nr procese: \n $ ");
    scanf("%d", &procese);

    pthread_t* threads = malloc(sizeof(threads) * procese);

	for(int i = 0; i < procese; i++)
	{
        int* add = malloc(sizeof add);
        *add = i;

		if( pthread_create( threads + i, NULL, tfun, add ) )//se creaza un thread nou 
        //initializeaza thread-ul cu noul fir de executie lansat de functia tfun oferind argumentul add
        {
            perror("Eroare -> Nu s-a putut crea thread-ul.\n");
            return errno;
        }
	}

    for(int i = 0; i < procese; i++)
    {
        if( pthread_join( threads[i], NULL ) ) // asteapta finalizarea executiei unui thread
        {
            perror("Eroare.\n");
            return errno;
        }
    }

    sem_destroy(&sem); //eliberez resursele ocupate

    pthread_mutex_destroy( &mtx ); // distrug mutexul

    free(threads); // eliberez memorie

    return 0;
}

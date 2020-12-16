// gcc count.c -o count -pthread
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;

pthread_mutex_t mtx; // pentru a fi accesibila tuturor firelor de executie

int decrease(int count) // Thread-ul foloseste un numar de resurse
{
    pthread_mutex_lock( &mtx ); // blochez zona ca sa intre un singur thread
    
    if(available_resources < count)
    //daca threadul foloseste mai multe resurse decat avem disponibile
    {
        pthread_mutex_unlock( &mtx ); // deblochez zona ca sa obtin mai multe resurse
        return -1; //iesim din functie, ca sa stim ca nu aveam destule resurse disponibile
    }
    else
    {
        available_resources -= count; //vedem cate resurse raman disponibile 
        //dupa ce threadul foloseste count resurse
    
        printf("Got %d resources, remaining %d resources\n", count, available_resources);
    }
    
    pthread_mutex_unlock( &mtx ); // deblochez zona daca am terminat

    return 0;
}

int increase(int count) // Thread-ul a eliberat resurse
{
    pthread_mutex_lock( &mtx ); // blochez Zona

    available_resources += count ; //adaugam nr de resurse eliberate la nr de resurse disponibile

    printf("Release %d resources, remaining %d resources\n", count, available_resources);
    
    pthread_mutex_unlock( &mtx ); //  deblochez Zona

    return 0;
}

void *functie(void *arg) // functie ca sa pot folosi thread-uri
{
    int count = *((int*) arg); 
    if (count > 0)
        increase(count); //eliberam resurse
    else
        decrease(-count); //ocupam resurse

    free(arg);

    return NULL;
}

int main()
{
    if( pthread_mutex_init(&mtx, NULL) ) // creez mutex-ul
    {
          perror("Eroare -> Creare Mutex.\n");
          return errno ;
    }

    printf("MAX_RESOURCES = %d\n", available_resources);

    pthread_t* tid = malloc(sizeof(tid) * 10); //initializam tid pt 10 threaduri

    int i; 

    for(i = 0; i < 10; i++)
    {
        int* add = malloc(sizeof add);
        *add = rand() % 10 - 5;

        if( pthread_create(tid + i, NULL, functie, add) ) //se creaza un thread nou 
        //initializeaza thread-ul cu noul fir de executie lansat de functie oferind argumentul add
        {
            perror("Eroare -> Nu s-a putut crea thread-ul.\n");
            return errno;
        }
    }

    for(i = 0; i < 10; i++)
    {
        if( pthread_join(tid[i], NULL) ) // asteapta finalizarea executiei unui thread
        {
            perror("Eroare.\n");
            return errno;
        }
    }

    free(tid); //eliberam memoria

    pthread_mutex_destroy( &mtx ); // distruge mutex-ul pt ca nu mai avem nevoie de el; eliberam resursele ocupate

    return 0;
}

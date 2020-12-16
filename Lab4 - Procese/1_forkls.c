#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/errno.h>

int main()
{
    pid_t pid = fork(); //creez un proces nou
    
    if(pid < 0) 
    {
        perror("Problema la PID.\n"); //in caz ca exista o prbl cu procesul/pidul
        return errno;
    }
    else 
    if(pid == 0) //Proces Fiu - Copil
    {
        char *argv[] = {"ls", NULL}; //la fel ca pt main - pe prima pozitie, numele
        
        execve("/bin/ls", argv, NULL); 
        // pe prima pozitie argv[0] este calea absoluta, urmat de argumente -> lista se termina cu NULL
        //envp, este ultimul argument, similar cu argv mai putin primul element
        
        //executa un alt program existent si suprascrie complet procesul apelant cu un nou proces conform programului dat ca parametru
        perror(NULL);
        //execve nu mai revine in programul initial decat in cazul in care a aparut o eroare

        //printf("Child %d finished.\n", getpid());
        // -> nu se executa pt ca am suprascris cu execve
    }
    else //Proces Parinte
    {
        printf("My PID=%d, Child PID=%d.\n", getppid(), getpid());

        wait(NULL); //parintele isi suspenda activitatea pt a astepta terminarea executiei unui proces fiu; 
        //daca sunt mai multi fii, wait asteapta doar unul
        
        printf("Child %d finished.\n",  getpid());
    }

    return 0;
}

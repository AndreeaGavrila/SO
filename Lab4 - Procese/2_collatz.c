#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork(); //creez un proces nou

    if(pid < 0) 
    {
        perror("Problema cu PID-ul.\n"); //in caz ca exista o prbl cu procesul/pidul
        return errno;
    }
    else 
    if(pid == 0) //Proces Fiu
    {
        if(argc != 2) //nr necorespunzator de argumente
        {
            printf("Numar insuficient de argumente.\n"); 
            perror(NULL); 
        }
        else
        {
            int n = atoi(argv[1]); //n este nr pt care testam cu functia (string to int)
            printf("%d: %d ", n, n);// al doilea n = capat de ipoteza

            while(n != 1) //Ipoteza Collatz
            {
                if(n % 2 == 0) 
                    n /= 2;
                else 
                    n = 3* n + 1;
                    
                printf("%d ", n);
            }
            printf("\n");
        }
    }
    else //Proces Parinte
    {
        wait(NULL);
        //parintele isi suspenda activitatea pt a astepta terminarea executiei unui proces fiu; 
        //daca sunt mai multi fii, wait asteapta doar unul
        
        printf("Child %d finished.\n", getpid());
    }

    return 0;
}

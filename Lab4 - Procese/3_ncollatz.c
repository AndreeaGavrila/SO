#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>

int main(int argc, char *argv[])
{
    int i;
    printf("Starting Parent %d\n", getpid());

    for(i = 1; i <= argc; i++)
    {
        //se repeta programul de la collatz, pentru fiecare nr al listei
        pid_t pid = fork(); //creez un proces nou
        
        if(pid < 0) 
        {
            perror("Problema cu PID-ul.\n"); 
            return errno;
        }
        else 
        if( pid == 0) //Proces Fiu
        {
            int n = atoi(argv[i]);

            printf("%d: %d ", n, n);

            while(n != 1)
            {
                if(n % 2 == 0) 
                    n /= 2;
                else 
                    n = 3 * n + 1;

                printf("%d ", n);
            }
            printf("\nDone Parent %d Me %d\n",  getppid(), getpid());
            //marcam fiecare proces care se termina
            return 1;
        }
    }
    
    //Proces Parinte
    for(i = 1; i <= argc; i++)
        wait(NULL); //pt fiecare parinte se asteapta executia procesului fiu

    printf("Done Parent %d Me %d\n", getppid(), getpid());

    return 0;
}

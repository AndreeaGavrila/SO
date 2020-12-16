// gcc matrice.c -o matrice -pthread
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int **matriceA, **matriceB;
struct mat
{
	// pozitia curenta in matrice: i, j, dimensiunea 
	int i, j, dimens;
};

void *produs(void *v)
{
    struct mat *arg = v;

    int *sum = (int *) malloc( sizeof(int) );
    *sum = 0; 

    for(int k = 0; k < arg->dimens; k++ )
        *sum += matriceA [arg->i] [k] * matriceB [k] [arg->j];

    free(v);
    v = NULL;
    
	return sum;
}

int main()
{
	int m, n, dimens, i, j, k;
    k = 0;
	void *result;

	//citesc prima matrice,  Matricea A
	printf("Dimensiune matrice A: ");
	scanf("%d %d", &m, &dimens);

	matriceA=(int**) malloc ( sizeof (int*) *m );
	for( i = 0; i < m; i++ )
    {
		matriceA[i] = (int *) malloc ( sizeof(int) *dimens );

		for(j = 0; j < dimens; j++)
		{
			int x;
			scanf("%d", &x);
            matriceA [i] [j] = x; // citesc pe rand fiecare element
		}
	}

	//citesc a doua matrice,  Matricea B
	printf("Dimensiune matrice B \nEste Obligatoriu ca:   (nr linii la matricea B == nr coloane matricea A) : ");
	scanf("%d %d", &dimens, &n);

    matriceB = (int**) malloc ( sizeof (int*) *dimens );
    for( i = 0; i < dimens; i++ )
    {
        matriceB[i] = (int *) malloc ( sizeof (int) *n) ;
        for( j = 0; j < n; j++)
        {
            int x;
            scanf("%d", &x);
            matriceB [i] [j] = x; // citesc pe rand fiecare element
        }
    }

    pthread_t thr[n * m]; // Creez si lansez noul fir de executie = thread

	int rezultat[m][n];
    
	for(i = 0; i < m; i++)
    {
		for(j = 0; j < n; j++)
		{
		    struct mat *arg = (struct mat*) malloc (sizeof (struct mat) );

            arg->i = i;
            arg->j = j;
            arg->dimens = dimens;

			if( pthread_create(&thr[k++], NULL, produs, arg) )
			//  thr primeste noul fir de executie = thread
            //  ce a fost lansat de fct Str_Rev prin argumentul result==argv[1]
            {
				perror("Eroare -> la crearea thread-ului\n");     
                return errno;
			}
		}
	}
    
	k = 0;

	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
        {
            //Pentru a astepta finalizarea executiei unui thread
            if( pthread_join(thr[k++], &result) )
            {
				perror("Eroare -> la executia thread-ului\n");
				return errno;
			}
			rezultat[i][j] = *( (int*) result);
        }

    free(result); // eliberez memoria
    result = NULL;


	// afisez maricea obtinuta in final :
    printf("\n\nRezultat:\n");
    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++) 
            printf("%d ", rezultat[i][j]);
        printf("\n");
    }

    for(i = 0; i < dimens; i++) 
        free(matriceA[i]);  // eliberez memoria
    free(matriceA);

    for(i = 0; i < dimens; i++) 
        free(matriceB[i]);  // eliberez memoria
    free(matriceB);
    
    return 0;
}
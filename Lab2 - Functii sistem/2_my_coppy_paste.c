#include <unistd.h>

#include <stdio.h>
#include <string.h>

#include <fcntl.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

int Scrie_Mesaj(char* mesaj)
{
    int lungime, nrcaractere, x;

    lungime = strlen(mesaj);
    nrcaractere = 0;
    
    while ( (x = write(1, mesaj + nrcaractere, lungime - nrcaractere)) > 0) 
    {
        nrcaractere += x;
    }

    //Daca apare o eroare, o scriu cu ajutorul stderr
    int var;
    if (x < 0) 
    {
        var = errno;
        char eroare[] = "Eroare la scriere...\n";
        write(2, eroare, strlen(eroare));
        return var;
    }
    return 0;
}


int Copiere(char* from, char* to)
{
    //Open cu dreptul de Read pe un fisier existent
    int reading = open(from, O_RDONLY);  

    //Daca apare o eroare, afisez acest lucru
    int var;
    if (reading < 0) 
    {
        var = errno;
        Scrie_Mesaj("Eroare la deschiderea fisierului de intrare...\n");
        return var;
    }

    //Open cu dreptul de Write si cu flagul O_CREAT
    int writing = open(to, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);  

    //Daca apare o eroare, afisez acest lucru
    if (writing < 0) 
    {
        var = errno;
        Scrie_Mesaj("Eroare la deschiderea fisierului de iesire...\n");
        return var;
    }

    const int dimens = 1000;
    char buffer[dimens];

    //intr-o bucla, citesc cu read intr-un buffer de dimens fixa 
    //si scriu in celalalt fisier bufferul
    int nrbytes; 
    while ( (nrbytes = read(reading, buffer, dimens))  > 0) 
        //bucla se opreste deoarece READ returneaza nr de bytes cititi la acel read
    {
        int nrcaractere = 0 , c;
        while ( (c = write(writing, buffer + nrcaractere, nrbytes - nrcaractere)) > 0)
        {
            nrcaractere += c;
        }

        //Daca apare o eroare, afisez acest lucru
        int var;
        if (c < 0) 
        {
            var = errno;
            Scrie_Mesaj("Eroare la scrierea in fisier...\n");
            return var;
        }
    }
    //Daca apare o eroare, afisez acest lucru
    if (nrbytes < 0) 
    {
        var = errno;
        Scrie_Mesaj("Eroare la citirea din fisier...\n");
        return var;
    }
    //Nu uit sa inchid fisierele
    close(reading);
    close(writing);
}

//cand atribui unei variabile functia Scrie_Mesaj, 
//aceasta poate fi !=0 , deci vom avea o eroare

//In concluzie, voi face o functie pentru a afisa acest tip de eroare
int Scrie_Eroare(char* mesaj, int nr)
{
    char eroare[1000];
    int lung = 0;

    write(2, mesaj, strlen(mesaj));

    while (nr) 
    {
        eroare[lung++] = nr % 10 + '0';
        nr /= 10;
    }

    write(2, "Eroare: ", strlen("Eroare: "));
    
    while (lung > 0) 
    {
        lung--;
        write(2, eroare + lung, 1);
    }
    write(2, "\n\n", 2);
}

int main(int argc, char* arg[])
{
    //pentru cazul de suprascriere a fisierelor
    int eroare;
    if (argc == 3) //fisierele sunt transmise catre parametri
        eroare = Copiere(arg[1], arg[2]);

    else 
    {
        const int dimens = 1000;
        char input [dimens] , output [dimens] ;

        memset(input, 0, sizeof(input) );
        memset(input, 0, sizeof(output) );

        Scrie_Mesaj("Fisierul de intrare: ");
        read(0, input, dimens);

        Scrie_Mesaj("Fisierul de iesire ");
        read(0, output, dimens);
        
        // Stergem \n de la sfarsit
        input[ strlen(input) - 1 ] = output [strlen(output) - 1 ] = 0;

        eroare = Copiere(input, output);
    }
    if (eroare)
        Scrie_Eroare("Eroare la copierea fisierelor...\n", eroare);

    return 0;
}
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

int kopiec[1001][3]; // kopiec

// odwrotne sortowanie kopca
// służy do tego, aby sprawdzić czy dziecko jest na pewno
// mniejsze od rodzica, jeśli tak to zamieniamy
// patrzymy po czasie dostępności zadania
void kopcowanie(int *tab, int rozmiar_kopca)
{
    for(int i=rozmiar_kopca; i>1; i--)
    {
        if(kopiec[tab[i]][0]<kopiec[tab[i/2]][0])
        {
            int a;
            a=tab[i];
            tab[i]=tab[i/2];
            tab[i/2]=a;
        }

    }
}

void kopcowanie_zadan(int *tab, int rozmiar_kopca)
{
    for(int i=rozmiar_kopca; i>1; i--)
    {
        if(kopiec[tab[i]][2]>kopiec[tab[i/2]][2])
        {
            int a;
            a=tab[i];
            tab[i]=tab[i/2];
            tab[i/2]=a;
        }
    }
}

int main()
{
    char wej[3][30]= {"in50.txt","in100.txt","in200.txt"};
    char wyj[3][30]= {"out50.txt","out100.txt","out200.txt"};
    for(int y=0; y<3; y++)
    {
        int nr_zadania; // liczba zadań
        int czas_aktualny=0; // czas aktualny
        int k=0; // indeks tablicy pi
        int Cmax=0; // funkcja celu
        int p=0;
        fstream plik;
        plik.open (wej[y]); // zmienianie nazwy pliku

        ofstream wynik;

        wynik.open(wyj[y]);
        if (plik.is_open())
        {
            plik >> nr_zadania;
            plik>> p;
        }
        else
        {
            cout << "Error opening file\n";
            return 0;
        }
        int * kolejnosc=new int[2*nr_zadania]; // wektor, w którym jest zapisywana kolejność zadań
        int * cmaxy=new int[2*nr_zadania]; // wektor, w którym są przechowywane kolejne wartości funkcji celu
        int rozmiar_kopca=nr_zadania; // rozmiar kopca
        int ile_gotowych=0; // ilość zadań gotowych w danym momencie
        int * G=new int[nr_zadania+1]; // zbiór zadań gotowych do realizacji w danym momencie
        int * N=new int[nr_zadania+1]; // podstawowa konfiguracja
        for(int i=1; i<=nr_zadania; i++) // wczytywanie wartości z pliku
        {
            N[i]=i;
            for(int m=0; m<3; m++)
            {
                plik >> kopiec[i][m];
            }
        }

        cmaxy[1]=0;
        kopcowanie(N,rozmiar_kopca);
        while((rozmiar_kopca>0) || (ile_gotowych>0)) // dopóki w którejkolwiek tablicy coś jest
        {
            while(rozmiar_kopca>0 && kopiec[N[1]][0]<=czas_aktualny)
                // jeśli rozmiar kopca jest większy od zera
                // i czas dostarczenia pierwszego zadania z kopca
                // jest poniżej aktualnego czasu
            {
                ile_gotowych++; // zwiększamy liczbę zadań
                G[ile_gotowych]=N[1]; //wrzucamy pierwsze zadanie z kopca
                {
                     // robimy to lokalnie
                    int temp;
                    temp=N[1];
                    N[1]=N[rozmiar_kopca]; // wrzucamy pierwsze zadanie na koniec "listy"
                    N[rozmiar_kopca]=temp;
                } // tutaj już temp nie istnieje
                rozmiar_kopca--; // zmniejszamy rozmiar stosu, aby nie brać pod uwagę wyrzuconego zadania
                kopcowanie(N,rozmiar_kopca); //znów sortujemy stos
            }
            if(ile_gotowych==0) // jeśli nie ma żadnego zadania w G
            {
                czas_aktualny=kopiec[N[1]][0]; // przesuwamy aktualny czas do pierwszego zadania z kopca
            }
            else
            {
                kopcowanie_zadan(G,ile_gotowych); // sortujemy dostępne zadania
                k++; // zwiększamy indeks kolejności zadan
                kolejnosc[k]=G[1]; // zapisujemy indeks zadania do tablicy pi
                czas_aktualny=czas_aktualny+kopiec[G[1]][1]; // zwiększamy czas

                Cmax=max(Cmax,czas_aktualny+kopiec[G[1]][2]); // zwiększamy funkcję celu
                {
                    // to samo co wyżej
                    int temp;
                    temp=G[1];
                    G[1]=G[ile_gotowych];
                    G[ile_gotowych]=temp;
                }
                ile_gotowych--; // zmniejszamy liczbę zadań
            }
        }
        for(int i=1; i<=nr_zadania; i++)
            wynik << kolejnosc[i] << endl; // wyświetlamy konfigurację
        cout<<Cmax<<endl;
        delete [] kolejnosc;
        delete [] cmaxy;
        plik.close();
    }
    system("PAUSE");
    return 0;
}


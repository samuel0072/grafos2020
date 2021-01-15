#include <stdio.h>
#include <stdlib.h>

/*
* O grafo foi representado como uma matriz de adjacência por praticidade.
* Uma estrutura auxiliar foi usada para ordenar as arestas.
*/

#define INFINITO 9999 // usado para definir o custo de uma aresta como infinito
#define N_VERTICE -1 // usado para definir um vertice como invalido
#define CUSTO_N 0 // usado para inicializar a matriz de adjacência

typedef struct aresta aresta;

/*
* Estrutura auxiliar para ordenar as arestas pelo seu custo.
*/
struct aresta {
    int u;//indice do vertice
    int v;//indice do vertice
    int custo;// custo da aresta
};

/*
* funções que implementam a estrutura de dados union find
*/
void make_set(int* pai, int* rank, int x);

int find(int* pai, int x);

void union_(int* pai, int* rank, int x, int y);

int find(int* pai, int x);

/*
* Código adaptado de https://www.ime.usp.br/~pf/algoritmos/aulas/mrgsrt.html
*/
static void  intercala (int p, int q, int r, aresta** v);

void mergesort (int p, int r, aresta** v);

/* 
* fim do código adaptado
*/

/*
* Retorna uma matriz de adjacência representando um grafo de dimensões tamanho X tamanho
*/
int** criar_grafo( int tamanho);

/*
* Retorna um vetor de ponteiros da estrutura aresta de dimensão tamanho X tamanho
* Já iniciliza os ponteiros com dados de uma aresta invalida
*/
aresta** criar_arestas(int tamanho);

/*
* Adiciona uma aresta no grafo.
* Atribui o custo na matriz e modifica os campos no array arestas para representarem uam aresta válida.
*/
void adicionar_aresta(int** grafo, aresta** arestas, int tamanho, int u, int v, int custo);

int** kruskal(int** grafo, aresta** e, int tamanho);

int main(){
    int tamanho = 4;
    int i, j;
    int** grafo = criar_grafo(tamanho);
    int** T;
    aresta** arestas = criar_arestas(tamanho);

    adicionar_aresta(grafo, arestas, tamanho, 0, 1, 1);
    adicionar_aresta(grafo, arestas, tamanho, 0, 2, 1);
    adicionar_aresta(grafo, arestas, tamanho, 0, 3, 5);
    adicionar_aresta(grafo, arestas, tamanho, 1, 2, 1);
    adicionar_aresta(grafo, arestas, tamanho, 1, 3, 3);
    adicionar_aresta(grafo, arestas, tamanho, 3, 2, 2);
    

    /*for(i = 0; i < tamanho*tamanho; i++) {
        printf("%d -> %d : %d\n", arestas[i]->u, arestas[i]->v, arestas[i]->custo);
    }*/
    T = kruskal(grafo, arestas, tamanho);
    printf("Matriz de adjacência da arvore minima:\n");
    for(i = 0; i < tamanho; i++) {
        for(j = 0; j < tamanho; j++){
            printf("%d ", T[i][j]);
        }
        printf("\n");
    }
    return 0;
}


void make_set(int* pai, int* rank, int x) {
    pai[x] = x;
    rank[x] = 0;
}

int find(int* pai, int x){
    int r = x;
    while(pai[r] != r ) {
        r = pai[r];
    }
    return r;
}

void union_(int* pai, int* rank, int x, int y) {
    if (rank[x] >= rank[y]) {
        
        pai[y] = x;
        
        if(rank[x] == rank[y]) {
            rank[x]++;
        }
    }
    else {
        pai[x] = y;
    }
}


/*Código adaptado de https://www.ime.usp.br/~pf/algoritmos/aulas/mrgsrt.html*/
static void  intercala (int p, int q, int r, aresta** v) 
{
   aresta** w;                              //  1
   w = (aresta**) malloc ((r-p) * sizeof (aresta*));   //  2
   int i = p, j = q;                       //  3
   int k = 0;                              //  4

   while (i < q && j < r) {                //  5
      if (v[i]->custo <= v[j]->custo)  w[k++] = v[i++];  //  6
      else  w[k++] = v[j++];               //  7
   }                                       //  8
   while (i < q)  w[k++] = v[i++];         //  9
   while (j < r)  w[k++] = v[j++];         // 10
   for (i = p; i < r; ++i)  v[i] = w[i-p]; // 11
   free (w);                               // 12
}

void mergesort (int p, int r, aresta** v)
{
   if (p < r-1) {                 // 1
      int q = (p + r)/2;          // 2
      mergesort (p, q, v);        // 3
      mergesort (q, r, v);        // 4
      intercala (p, q, r, v);     // 5
   }
}

/* fim do código adaptado*/

int** criar_grafo( int tamanho) {
    int** g = (int**) malloc(tamanho * sizeof(int*));
    int i, j;

    for (i = 0; i < tamanho; i++) {
        g[i] = (int*)malloc(tamanho * sizeof(int));
    }

    for (i = 0; i < tamanho; i++) {
        for(j = 0; j < tamanho; j++) {
            g[i][j] = CUSTO_N;
        }
    }

    return g;
}

aresta** criar_arestas(int tamanho) {
    aresta** a = (aresta**) malloc(tamanho*tamanho*sizeof(aresta*));
    int t = tamanho*tamanho;
    int i;
    //as arestas já são alocadas para não atrapalhar o mergesort
    for (i = 0; i < t; i++) {
        a[i] = (aresta*) malloc(sizeof(aresta));
        a[i]->u = N_VERTICE;
        a[i]->v = N_VERTICE;
        a[i]->custo = INFINITO;
    }
    return a;
}

void adicionar_aresta(int** grafo, aresta** arestas, int tamanho, int u, int v, int custo) {
    grafo[u][v] = custo;
    int indice = u*tamanho + v;
    arestas[indice]->u = u ;
    arestas[indice]->v = v;
    arestas[indice]->custo = custo;
}

int** kruskal(int** grafo, aresta** e, int tamanho) {
    int pai[tamanho];
    int rank[tamanho];
    int i;
    int t = tamanho * tamanho;
    int** T =  criar_grafo(tamanho);

    mergesort(0, tamanho*tamanho, e);

    for(i = 0; i < tamanho; i++) {
        make_set(pai, rank, i);
    }
    for(i = 0; i < t; i++) {
        if (e[i]->custo == INFINITO) {
            break;
        }
        int u = e[i]->u;
        int v = e[i]->v;
        int custo = e[i]->custo;
        int pai_u = find(pai, u);
        int pai_v = find(pai, v);
        if( pai_u != pai_v) {
            T[u][v] = custo;
            union_(pai, rank, pai_u, pai_v);
        }
    }
    return T;
}

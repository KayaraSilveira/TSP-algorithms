#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define INT_MAX 9999;

int *caminho_final;
int count_caminho = 0;

int **lerArquivo(int qtd_vertices);
void imprimirMatriz(int **mat, int qtd_vertices);
void freeMatriz (int **mat, int qtd_vertices);
int **primMST(int **mat, int qtd_vertices);
int minKey(int *key, int *mstSet, int qtd_vertices);
int **MST(int *parent, int qtd_vertices);
void DFS(int **matadj, int qtd_vertices, int starting_vertex, int *visitados);
float time_diff(struct timeval *start, struct timeval *end);


int main (){

    int i, j, qtd_vertices;
    int **mat, **matprim, **matadj;
    int *visitados;
    struct timeval begin, end;

    /*LENDO O ARQUIVO E CRIANDO O GRAFO*/

    printf("Qual a quantidade de vertices?\n");
    scanf("%d", &qtd_vertices);
    
    caminho_final = malloc((qtd_vertices + 1) * sizeof(int)); 

    mat = lerArquivo(qtd_vertices);
    gettimeofday(&begin, NULL);

    printf("---Grafo Original---\n");
    imprimirMatriz(mat, qtd_vertices);

    /*EXECUTANDO O PRIM*/

    matprim = (int**)malloc((qtd_vertices - 1) * sizeof(int*));
    for(i = 0; i < (qtd_vertices - 1); i++) {
        matprim[i] = (int*)malloc(2 * sizeof(int));
    }

    matprim = primMST(mat, qtd_vertices);

    printf("--Matriz obtida através do Prim--\n");
    for(i = 0; i < (qtd_vertices - 1); i++) {
        for(j = 0; j < 2; j++) {
            printf("%d ", matprim[i][j]);
        }
        printf("\n");
    }

    /*CRIANDO UMA MATRIZ DE ADJASCENCIA A PARTIR DA MATRIZ RETORNADA PELO PRIM*/
    matadj = (int**)malloc(qtd_vertices * sizeof(int*));
    for(i = 0; i < qtd_vertices; i++) {
        matadj[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }

    for(i = 0; i < qtd_vertices; i++) {
        for(j = 0; j < qtd_vertices; j++) {
            matadj[i][j] = 0;
        }
    }
    int v1, v2;
    for(i = 0; i < (qtd_vertices - 1); i++) {
        v1 = matprim[i][0];
        v2 = matprim[i][1];
        matadj[v1][v2] = 1;
        matadj[v2][v1] = 1;

    }

    printf("--Matriz de adjascencia após o prim--\n");
    imprimirMatriz(matadj, qtd_vertices);


    /*EXECUTANDO O DFS*/
    visitados = malloc(qtd_vertices * sizeof(int));

    for(i = 0; i < qtd_vertices; i++) {
        visitados[i] = 0;
    }

    DFS(matadj, qtd_vertices, 0, visitados);

    caminho_final[qtd_vertices] = caminho_final[0];

    int soma_caminho = 0;
    int max = qtd_vertices + 1;

    /*IMPRIMINDO AS INFORMAÇÕES ÚTEIS*/

    printf("--------------------------------------\n");
    printf("Ciclo: \n");

    for(i = 0; i < max; i++) {
        printf(" %d ->", caminho_final[i]);
    }

    for(i = 0; i < max; i++) {      
        if(i == qtd_vertices) {
            soma_caminho = soma_caminho + 0;    
        }
        else {
            soma_caminho = soma_caminho + mat[caminho_final[i]][caminho_final[i+1]];
        }
    }

    printf("\n");
    printf("\nCusto total: %d\n", soma_caminho);
    gettimeofday (&end, NULL);
    printf("\ntempo de execucao: %0.8f sec\n\n", time_diff(&begin, &end));

    freeMatriz(matadj, qtd_vertices);  
    freeMatriz(mat, qtd_vertices);


    return 0;
}

int **lerArquivo(int qtd_vertices) {

    int **mat;
    int i, j;
    char name_file[15];
    FILE *file;

    printf("Qual o nome do arquivo?\n");
    scanf("%s", name_file);
    file = fopen(name_file, "r");

    mat = (int**)malloc(qtd_vertices * sizeof(int*));
    for(i = 0; i < qtd_vertices; i++) {
        mat[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }

    for(i = 0; i < qtd_vertices; i++) {
        for(j = 0; j < qtd_vertices; j++) {
            fscanf(file, "%d", &mat[i][j]);
        }
    }

    fclose(file);

    return mat;
}

void imprimirMatriz(int **mat, int qtd_vertices) {

    int i, j;

    for(i = 0; i < qtd_vertices; i++) {
        for(j = 0; j < qtd_vertices; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

void freeMatriz (int **mat, int qtd_vertices) {

    int i;

    for(i = 0; i < qtd_vertices; i++) {
        free(mat[i]);
    }
    free(mat);

}

int **primMST(int **mat, int qtd_vertices) {

    int parent[qtd_vertices], key[qtd_vertices], mstSet[qtd_vertices];
    int count, i;

    for (i = 0; i < qtd_vertices; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    for (count = 0; count < qtd_vertices - 1; count++) {

        int u = minKey(key, mstSet, qtd_vertices);
        mstSet[u] = 1;
        for (int v = 0; v < qtd_vertices; v++)
            if (mat[u][v] && mstSet[v] == 0 && mat[u][v] < key[v])
            {
                parent[v] = u;
                key[v] = mat[u][v];
            }
    }

    int  **v;    
    v = MST(parent, qtd_vertices);

    return v;
}

int minKey(int *key, int *mstSet, int qtd_vertices) {
    int min, min_index, v;
    min = INT_MAX;

    for (v = 0; v < qtd_vertices; v++) {
        if (mstSet[v] == 0 && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}

int **MST(int *parent, int qtd_vertices) {

    int **v;
    
    int j, i;

    v = (int**)malloc((qtd_vertices - 1) * sizeof(int*));
    for(i = 0; i < (qtd_vertices - 1); i++) {
        v[i] = (int*)malloc(2 * sizeof(int));
    }
    j = 0;
    for(int i = 1; i < qtd_vertices; i++) {
        v[j][0] = parent[i];
        v[j][1] = i;
        j++;
    }
    return v;
}

void DFS(int **matadj, int qtd_vertices, int starting_vertex, int *visitados) {

    int i;

    caminho_final[count_caminho] = starting_vertex;
    count_caminho++;

    visitados[starting_vertex] = 1;


    //recursão
    for(i = 0; i < qtd_vertices; i++) {
        if(i == starting_vertex) {
        }
        else {
            if(matadj[starting_vertex][i] == 1) {
                if(visitados[i] == 1) {
                }
                else {
                DFS(matadj, qtd_vertices, i, visitados);
                }
            }
        }
    }

}

float time_diff(struct timeval *start, struct timeval *end) {

    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void TSP(int *C, int *A, int *path, int *fpath, int *sum, int *fsum, int flag, int qv, int b, int a, int *sc);
float time_diff(struct timeval *start, struct timeval *end);

int main() {

    srand(time(NULL));
    int qv, i, j, a, sc = 0;
    char name_file[15];
    struct timeval begin, end;

    /*LENDO O ARQUIVO E CRIANDO O GRAFO*/
    FILE *file;

    printf("Qual o nome do arquivo?\n");
    scanf("%s", name_file);

    file = fopen(name_file, "r");

    printf("Qual a quantidade de vertices?\n");
    scanf("%d", &qv);

    int A[qv][qv], C[qv][qv], sum = 0, fsum = 99999, path[qv - 1], fpath[1000][qv - 1], vertice, bestpath[1][qv - 1], bestsum = 99999;

    gettimeofday(&begin, NULL);    
    for (i = 0; i < qv; i++) {
        for (j = 0; j < qv; j++) {
            fscanf(file, "%d ", &A[i][j]);
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    fclose(file);

    /*RESOLUÇÃO DO TSP*/

    for (a = 1; a <= qv; a++)
    {
        for (i = 0; i < qv; i++) {
            for (j = 0; j < qv; j++) {
                C[i][j] = 0;
            }
        }
        for (i = 0; i < qv; i++) {
            C[i][a - 1] = a;
        }

        TSP(C, A, path, fpath, &sum, &fsum, 0, qv, a - 1, a - 1, &sc);

        if (bestsum > fsum) {
            vertice = a;
            bestsum = fsum;
            for (j = 0; j < qv - 1; j++) {
                bestpath[0][j] = fpath[0][j];
            }
        }
    }

    printf("\nCusto total = %d", bestsum);
    printf("\nArestas: %d ->", vertice);
    for (j = 0; j < qv - 1; j++) {
        printf(" %d ->", bestpath[0][j] + 1);
    }
    printf(" %d\n", vertice);
    gettimeofday (&end, NULL);
    printf("\ntempo de execucao: %0.8f sec\n", time_diff(&begin, &end));

}

void TSP(int *C, int *A, int *path, int *fpath, int *sum, int *fsum, int flag, int qv, int b, int a, int *sc) {
    int i, k;
    flag++;

    for (k = 0; k < qv; k++) {
        if (*(C + qv * flag + k) == 0) {

            *(C + qv * flag + k) = k + 1;    
            *sum = *sum + *(A + qv * b + k); 
            *(path + flag - 1) = k;         
 
            if (flag < qv) {
                for (i = flag + 1; i < qv; i++)
                    *(C + i * qv + k) = k + 1;
            }

            if (flag < qv - 1) {
                TSP(C, A, path, fpath, sum, fsum, flag, qv, k, a, sc); 
            }

            if (flag == qv - 1) {
                *sum = *sum + *(A + qv * k + a); 
                if (*sum == *fsum) {
                    *sc = *sc + 1; 
                    for (i = 0; i < qv - 1; i++) {
                        *(fpath + (*sc) * (qv - 1) + i) = *(path + i); 
                    }
                }
                else if (*sum < *fsum) {
                    *fsum = *sum; 
                    *sc = 0;
                    for (i = 0; i < qv - 1; i++) {
                        *(fpath + i) = *(path + i); 
                    }
                }
                *sum = *sum - *(A + qv * k + a); 
            }

            for (i = flag; i < qv; i++) {
                *(C + qv * i + k) = 0;
            }
            *sum = *sum - *(A + qv * b + k); 
        }
    }
}

float time_diff(struct timeval *start, struct timeval *end) {

    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

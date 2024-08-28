//  Crie um programa em C que implemente os algoritmos de escalonamento RM (Rate Monotonic)
// e EDF (Earliest Deadline First) para sistemas de tempo real. O programa também deve fazer os testes
// de escalonabilidade e mostrar uma escala de tempo de execução (Semelhantes aos Slides de Aula).

// O programa deve ler um arquivo .txt que possui todas as tarefas de um sistema com os respectivos
// períodos, tempos de execução no pior caso e deadlines. A seguir temos o exemplo de um arquivo 
// que mostra um sistema com três tarefas.

// P C D
// 4 1 4
// 5 2 5
// 20 4 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Processo{
    int periodo, capacidade, deadline;
}Processo;


float testeEscalabilidade(Processo processos[], int n){
    float resultado = 0;
    for(int i = 0; i < n; i++){
        resultado += (float)(processos[i].capacidade/processos[i].periodo);
    }
    return resultado;
}

int main (int argc, char *argv[]){
    FILE *file = fopen(argv[1], "r");

    char line[50];
    int n = -1;
    while (fgets(line, sizeof(line), file) != NULL){
        n++;
    }
    rewind(file);
    
    Processo *processos = (Processo *)malloc(n * sizeof(Processo));

    fgets(line, sizeof(line), file);
    char *num;
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL){
        Processo novo;
        num = strtok(line,"\t");
        int j = 1;
        while (num != NULL)
        {
            int valor = atoi(num);
            if (j == 1){novo.periodo = valor;}
            else if (j == 2){novo.capacidade = valor;}
            else if (j == 3){novo.deadline = valor;}
            
            j++;
            num = strtok(NULL, "\t");
        }
        processos[i] = novo;
        i++;
    }

    for (int k = 0; k < n; k++) {
        printf("Processo %d: Periodo=%d, Capacidade=%d, Deadline=%d\n", k + 1, processos[k].periodo, processos[k].capacidade, processos[k].deadline);
    }


    return 0;
}
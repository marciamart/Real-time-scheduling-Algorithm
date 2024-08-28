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
#include <limits.h>

typedef struct Processo{
    int periodo, capacidade, deadline;
}Processo;

int maiorDeadline(Processo processos[], int n){
    int maior = processos[0].deadline;
    for(int i = 0; i < n; i++){
        if(processos[i].deadline > maior){
            maior = processos[i].deadline;
        }
    }
    return maior;
}

float testeEscalabilidade(Processo processos[], int n){
    float resultado = 0;
    for(int i = 0; i < n; i++){
        resultado += ((float)processos[i].capacidade/(float)processos[i].periodo);
    }
    return resultado;
}

void plotar(int print[], int n){
    printf("T1 #    *   *   #");
}

Processo* clonarVetores(Processo* vet, int n) {
    Processo* novo_vetor = (Processo*)malloc(n * sizeof(Processo));
    if (novo_vetor == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }
    memcpy(novo_vetor, vet, n * sizeof(Processo));
    return novo_vetor;
}

void rateMonotic(Processo Processos[], int n, int print[]){
    int tempo_total = maiorDeadline(Processos, n); 
    int tempos_executando[n];
    int tempo_prox_execucao[n];
    
    for(int i = 0; i < n; i++){
        tempos_executando[i] = 0;
        tempo_prox_execucao[i] = 0;
    }

    for(int tempo = 0; tempo < tempo_total; tempo++){
        int index_do_selecionado = -1;
        int menor_periodo = INT_MAX;
        
        for(int i = 0; i < n; i++){
            if(tempo >= tempo_prox_execucao[i]){
                if(Processos[i].periodo < menor_periodo){
                    menor_periodo = Processos[i].periodo;
                    index_do_selecionado = i;
                }
            }
        }

        if(index_do_selecionado != -1){
            printf("Tempo: %d: Processo %d\n", tempo, index_do_selecionado + 1);
            print[tempo] = index_do_selecionado+1;
            tempos_executando[index_do_selecionado]++;

            if(tempos_executando[index_do_selecionado] == Processos[index_do_selecionado].capacidade){
                tempo_prox_execucao[index_do_selecionado] = tempo + Processos[index_do_selecionado].periodo;
                tempos_executando[index_do_selecionado] = 0;
            }
            
        }else{
            printf("Tempo %d: ocioso\n", tempo);
            print[tempo] = 0;
        }
    }
    ///printar-------------------------------------------
}

int edf(Processo processos[], int n, float scalabilityTest) {
    // if (scalabilityTest > 1) return 0;
    Processo *processosAux = clonarVetores(processos, n);
    int tempo_total = maiorDeadline(processos, n);
    int tempo_total_aux = tempo_total;
    int shortestDeadline = tempo_total_aux + 1;
    // int a[] = {0, 0, 0};
    int *periodSpace = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        periodSpace[i] = 0;
    }

    while (tempo_total > 0) {
        int actualTask = -1;
        for (int k = 0; k < n; k++) {
            if (tempo_total_aux - tempo_total >= periodSpace[k]) {
                if (processosAux[k].deadline < shortestDeadline) {
                    shortestDeadline = processosAux[k].deadline;
                    actualTask = k;
                } 
            }
        }
        if (actualTask < 0) {
            printf("Tempo %d: ocioso\n", tempo_total);
            tempo_total = tempo_total - 1;
            shortestDeadline = tempo_total_aux + 1;
            continue;
        }
        // subtrai 1 do c do atual
        processosAux[actualTask].capacidade -= 1;
        printf("Tempo: %d: Processo %d\n", tempo_total, actualTask + 1);
        // se c do atual for 0: 
        if (processosAux[actualTask].capacidade == 0) {
            //      Acrescenta o D do atual
            //      ptime[actualTask] += 1;
            periodSpace[actualTask] += processos[actualTask].periodo;
            processosAux[actualTask].deadline += processos[actualTask].deadline;
            // ptime[actualTask] += 1;
            processosAux[actualTask].capacidade = processos[actualTask].capacidade;
        }

        shortestDeadline = tempo_total_aux + 1;
        tempo_total = tempo_total - 1;
    }
}

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "r");

    char line[50];
    int n = -1;
    while (fgets(line, sizeof(line), file) != NULL){
        n++;
    }
    rewind(file);
    
    Processo *processos = (Processo *)malloc(n * sizeof(Processo));
    int print[n][2];

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

    int *print = (int *)malloc(n * sizeof(int));

    rateMonotic(processos, n, print);
    
    float teste = testeEscalabilidade(processos, n);
    printf("teste: %f", teste);
    
    edf(processos, n, teste);

    return 0;
}
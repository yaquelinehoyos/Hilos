#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>

extern int errno;

struct vectorStruct{  
    int *vectorFile1;
    int *vectorFile2;
    int numberTasks;
    int idHilo;
    int *resultado;
    int *p1;
    int *p2;
};

void saveNumbers(FILE *file, int *vector, int elements){    //Este método se encarga de leer
    char * line = NULL;                                     //los regitros línea por línea
    size_t len = 0;                                         //y almacenarlos en un vector
    ssize_t read;
    int counter = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        vector[counter] = atoi(line);
        counter++;
    }
}

int countElementsVector(FILE *file){    //Este método se encarga de contar
    int counter = 0;                    //los elementos de los vectores
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
            counter++;
    }
    return counter;
}

void divTask(int threads, int elements, int *vector){   //Este método es el encargado
    switch(threads){                                    //de la divión de tareas
        case 2:                                         //para cada uno de los hilos
            if(elements % 2 == 0){
                vector[0] = elements / 2;
                vector[1] = elements / 2;
            }else{
                vector[0] = elements/2;
                vector[1] = elements - vector[0];
            }
            break;
        case 4:
            if(elements % 4 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/4;
                }
            }else{
                int auxi1 = elements / 2;
                int auxi2 = elements - auxi1;
                vector[0] = auxi1 / 2;
                vector[1] = auxi1 - vector[0];
                vector[2] = auxi2 / 2;
                vector[3] = auxi2 - vector[2];
            }
            break;
        case 8:
            if(elements % 8 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/8;
                }
            }else{
                int auxi1 = elements / 2;
                int auxi2 = elements - auxi1;
                int auxi3 = auxi1 / 2;
                int auxi4 = auxi1 - auxi3;
                int auxi5 = auxi2 / 2;
                int auxi6 = auxi2 - auxi5;
                vector[0] = auxi3 / 2;
                vector[1] = auxi3 - vector[0];
                vector[2] = auxi4 / 2;
                vector[3] = auxi4 - vector[2];
                vector[4] = auxi5 / 2;
                vector[5] = auxi5 - vector[4];
                vector[6] = auxi6 / 2;
                vector[7] = auxi6 - vector[6];
            }
            break;
        case 16:
            if(elements % 16 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/16;
                }
            }else{
                int auxi1 = elements / 2;
                int auxi2 = elements - auxi1;
                int auxi3 = auxi1 / 2;
                int auxi4 = auxi1 - auxi3;
                int auxi5 = auxi2 / 2;
                int auxi6 = auxi2 - auxi5;
                int auxi7 = auxi3 / 2;
                int auxi8 = auxi3 - auxi7;
                int auxi9 = auxi4 / 2;
                int auxi10 = auxi4 - auxi9;
                int auxi11 = auxi5 / 2;
                int auxi12 = auxi5- auxi11;
                int auxi13 = auxi6 / 2;
                int auxi14 = auxi6 - auxi13;
                vector[0] = auxi7 / 2;
                vector[1] = auxi7 - vector[0];
                vector[2] = auxi8 / 2;
                vector[3] = auxi8 - vector[2];
                vector[4] = auxi9 / 2;
                vector[5] = auxi9 - vector[4];
                vector[6] = auxi10 / 2;
                vector[7] = auxi10 - vector[6];
                vector[8] = auxi11 / 2;
                vector[9] = auxi11 - vector[8];
                vector[10] = auxi12 / 2;
                vector[11] = auxi12 - vector[10];
                vector[12] = auxi13 / 2;
                vector[13] = auxi13 - vector[12];
                vector[14] = auxi14 / 2;
                vector[15] = auxi14 - vector[14];
            }
            break;
    }
}

void putPointer(int *tareas, int numHilos, int *pointers){
    pointers[0] = 0;
    for(int i = 1; i < numHilos; i++){
        pointers[i] = pointers[i - 1] + tareas[i - 1];
    }
}

void *threadFunction(void *args){                                       
    struct vectorStruct *actualArgs = args;                                                                 
    for(int i = 0; i < actualArgs->numberTasks; i++){                               
        actualArgs->resultado[actualArgs->idHilo] += (*actualArgs->p1) * (*actualArgs->p2);
        actualArgs->p1++;
        actualArgs->p2++;
    }
    return NULL;
}

int main (int argc, char *argv[]){
    if(argc != 4){      
        printf("Señor usuario, no ingreso los argumentos requeridos \n");
        printf("Error en el llamado: %s \n", strerror(errno));
        exit(errno);
    }else{
        FILE *file1, *file2;
        int numeroHilos = atoi(argv[3]);
        if(numeroHilos != 2 && numeroHilos != 4 && numeroHilos != 8 && numeroHilos != 16){  
            printf("Error en el llamado: %s \n", strerror(errno));                     
            exit(errno);
        }else{
            if( ((file1 = fopen(argv[1], "r") ) == NULL) || ((file2 = fopen(argv[2], "r") ) == NULL)){  
            printf("Error en el llamado: %s \n", strerror(errno));                                     
            exit(errno);
            }else{
                int elementsFile1 = countElementsVector(file1);
                rewind(file1);
                int elementsFile2 = countElementsVector(file2);
                rewind(file2);
                if(elementsFile1 != elementsFile2){     
                    printf("El numero de elementos de los vectores no es el mismo \n");
                    printf("Error en el llamado: %s \n", strerror(errno));
                    exit(errno);
                }else{
                    if(numeroHilos > elementsFile1){    
                        printf("El numero de hilos supera el numero de elementos del vector \n");
                        printf("Error en el llamado: %s \n", strerror(errno));
                        exit(errno);
                    }else{
                        int *numbersFile1 = malloc(elementsFile1*sizeof(int));
                        int *numbersFile2 = malloc(elementsFile2*sizeof(int));
                        saveNumbers(file1, numbersFile1, elementsFile1);
                        saveNumbers(file2, numbersFile2, elementsFile2);
                        struct vectorStruct args[numeroHilos]; 
                        int tasks[numeroHilos];
                        divTask(numeroHilos, elementsFile1, tasks);
                        int punteros[numeroHilos];
                        putPointer(tasks, numeroHilos, punteros);
                        int *resultadoPorHilo = malloc(numeroHilos*sizeof(int));
                        for(int i = 0; i < numeroHilos; i++){
                            args[i].vectorFile1 = numbersFile1;
                            args[i].vectorFile2 = numbersFile2;
                            args[i].numberTasks = tasks[i];
                            args[i].idHilo = i;
                            args[i].resultado = resultadoPorHilo;
                            args[i].p1 = &args[i].vectorFile1[punteros[i]];
                            args[i].p2 = &args[i].vectorFile2[punteros[i]];
                        }
                        pthread_t threads[numeroHilos];
                        clock_t begin = clock();    
                        for(int i = 0; i < numeroHilos; i++){
                            pthread_create(&threads[i], NULL, threadFunction, &args[i]);  
                        }
                        for(int i = 0; i < numeroHilos; i++){
                            pthread_join(threads[i], NULL);
                        }
                        int resultadoFinal = 0;
                        for(int i = 0; i < numeroHilos; i++){
                            resultadoFinal = resultadoFinal + args[i].resultado[i];
                        }
                        clock_t end = clock();   
                        free(args->vectorFile1);
                        free(args->vectorFile2);
                        free(resultadoPorHilo);
                        double time = 0.0;
                        time += (double)(end - begin) / CLOCKS_PER_SEC; 
                        printf("Resultado: %d \n", resultadoFinal);
                        printf("El tiempo de ejecución de la multiplicación es: %f segundos \n", time);
                    }
                    fclose(file1);  
                    fclose(file2);  
                }
            }
        }
    }
    return 0;
}
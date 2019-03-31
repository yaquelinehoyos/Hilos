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

void divTaskFourThreads(int numberElements, int *vectorToFull){     //Método de apoyo para 
    int auxi1 = numberElements / 2;                                 //la división de tareas
    int auxi2 = numberElements - auxi1;                             //para 4 hilos
    vectorToFull[0] = auxi1 / 2;
    vectorToFull[1] = auxi1 - vectorToFull[0];
    vectorToFull[2] = auxi2 / 2;
    vectorToFull[3] = auxi2 - vectorToFull[2];
}

void divTaskEightThreads(int numberElements, int *vectorToFull){    //Método de apoyo para
    int auxi[5];                                                    // la divisón de tareas
    auxi[0] = numberElements / 2;                                   //para 8 hilos
    auxi[1] = numberElements - auxi[0];
    auxi[2] = auxi[0] / 2;
    auxi[3] = auxi[0] - auxi[2];
    auxi[4] = auxi[1] / 2;
    auxi[5] = auxi[1] - auxi[4];
    int counter = 2;
    vectorToFull[0] = auxi[2] /2;
    for(int i = 1; i < 8; i++){
        if(i % 2 == 0){
            vectorToFull[i] = auxi[counter] / 2;
            counter ++;
        }else{
            vectorToFull[i] = auxi[counter] - vectorToFull[i - 1];
        }
    }
}

void divTaskSixteenThreads(int numberElements, int *vectorToFull){  //Método de apoyo para
    int auxi[13];                                                   //la divisón de tareas
    auxi[0] = numberElements / 2;                                   //para 16 hilos
    auxi[1] = numberElements - auxi[0];
    int counter = 0;
    for(int i = 2; i < 14; i++){
        if(i % 2 == 0){
            auxi[i] = auxi[counter] / 2;
        }else{
            auxi[i] = auxi[counter] - auxi[i - 1];
            counter++;
        }
    }
    vectorToFull[0] = auxi[counter] / 2;
    for(int i = 1; i < 16; i++){
        if(i % 2 == 0){
            vectorToFull[i] = auxi[counter] / 2;
        }else{
            vectorToFull[i] = auxi[counter] - vectorToFull[i - 1];
            counter++;
        }
    }
}

void divTask(int threads, int elements, int *vector){   //Este método es el encargado
    switch(threads){                                    //de la divión de tareas
        case 2:                                         //para cada uno de los hilos
            vector[0] = elements/2;
            vector[1] = elements - vector[0];
            break;
        case 4:
            if(elements % 4 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/4;
                }
            }else{
                divTaskFourThreads(elements, vector);
            }
            break;
        case 8:
            if(elements % 8 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/8;
                }
            }else{
                divTaskEightThreads(elements, vector);
            }
            break;
        case 16:
            if(elements % 16 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/16;
                }
            }else{
                divTaskSixteenThreads(elements, vector);
            }
            break;
    }
}

void putPointer(int *tareas, int numHilos, int *pointers){  //Este método se encarga
    pointers[0] = 0;                                        //de darle posición a los
    for(int i = 1; i < numHilos; i++){                      //punteros de cada hilo 
        pointers[i] = pointers[i - 1] + tareas[i - 1];
    }
}

void *threadFunction(void *args){         //Método para la multiplicación                              
    struct vectorStruct *actualArgs = args;                                                                 
    for(int i = 0; i < actualArgs->numberTasks; i++){                               
        actualArgs->resultado[actualArgs->idHilo] += (*actualArgs->p1) * (*actualArgs->p2);
        actualArgs->p1++;
        actualArgs->p2++;
    }
    return NULL;
}

int main (int argc, char *argv[]){
    if(argc != 4){      //Verificamos que no hayan más o menos argumentos de los necesarios
        printf("Señor usuario, no ingreso los argumentos requeridos \n");
        printf("Error en el llamado: %s \n", strerror(errno));
        exit(errno);
    }else{
        FILE *file1, *file2;
        int numeroHilos = atoi(argv[3]);
        if(numeroHilos != 2 && numeroHilos != 4 && numeroHilos != 8 && numeroHilos != 16){  
            printf("El número de hilos debe ser 2, 4, 8 o 16 \n");  //verificamos que el número 
            printf("Error en el llamado: %s \n", strerror(errno));  //de hilos sea el correcto
            exit(errno);
        }else{
            if( ((file1 = fopen(argv[1], "r") ) == NULL) || ((file2 = fopen(argv[2], "r") ) == NULL)){  //verificamos que los
                printf("Error en el llamado: %s \n", strerror(errno));                                  //archivos existan
                exit(errno);
            }else{
                int elementsFile1 = countElementsVector(file1);
                rewind(file1);
                int elementsFile2 = countElementsVector(file2);
                rewind(file2);
                if(elementsFile1 != elementsFile2){     
                    printf("El numero de elementos de los vectores no es el mismo \n"); //verificamos que el número de elementos
                    printf("Error en el llamado: %s \n", strerror(errno));              //de los vectores sean iguales
                    exit(errno);
                }else{
                    if(numeroHilos > elementsFile1){    
                        printf("El numero de hilos supera el numero de elementos del vector \n");   //Verificamos que se pueda realizar
                        printf("Error en el llamado: %s \n", strerror(errno));                      //la operación con el número de hilos
                        exit(errno);                                                                //requeridos por el usuario
                    }else{
                        int *numbersFile1 = malloc(elementsFile1*sizeof(int));
                        int *numbersFile2 = malloc(elementsFile2*sizeof(int));
                        saveNumbers(file1, numbersFile1, elementsFile1);
                        saveNumbers(file2, numbersFile2, elementsFile2);
                        struct vectorStruct args[numeroHilos];  //Creamos un struct por hilo
                        int tasks[numeroHilos];
                        divTask(numeroHilos, elementsFile1, tasks);
                        int punteros[numeroHilos];
                        putPointer(tasks, numeroHilos, punteros);
                        int *resultadoPorHilo = malloc(numeroHilos*sizeof(int));
                        for(int i = 0; i < numeroHilos; i++){   //Asignamos valores a cada struct de cada hilo
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
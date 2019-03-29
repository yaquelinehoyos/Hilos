#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>

extern int errno;
int result = 0;
int indice = 0;
int *p1;
int *p2;

struct vectorStruct{  
    int *vectorFile1;
    int *vectorFile2;
    int *vectorTasks;
    int idHilo;
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

void *threadFunction(void *args){                                   //Este método es el encargado de
    struct vectorStruct *actualArgs = args;                         //realizar las multiplicaciones
    int numberTasks = actualArgs->vectorTasks[actualArgs->idHilo];  //y sumas de los vectores
    for(int i = 0; i < numberTasks; i++){                           //para hallar el producto punto
        p1 = &actualArgs->vectorFile1[indice];
        p2 = &actualArgs->vectorFile2[indice];
        result += (*p1) * (*p2);
        p1++;
        p2++;
        indice++;
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
        if(numeroHilos != 2 && numeroHilos != 4 && numeroHilos != 8 && numeroHilos != 16){  //verificamos que el número de
            printf("Error en el llamado: %s \n", strerror(errno));                          //hilos sea el permitido
            exit(errno);
        }else{
            if( ((file1 = fopen(argv[1], "r") ) == NULL) || ((file2 = fopen(argv[2], "r") ) == NULL)){  //verificamos que los
            printf("Error en el llamado: %s \n", strerror(errno));                                      //archivos existan
            exit(errno);
            }else{
                int elementsFile1 = countElementsVector(file1);
                rewind(file1);
                int elementsFile2 = countElementsVector(file2);
                rewind(file2);
                if(elementsFile1 != elementsFile2){     //Verificamos que los dos vectores tengan el mismo número de elementos
                    printf("El numero de elementos de los vectores no es el mismo \n");
                    printf("Error en el llamado: %s \n", strerror(errno));
                    exit(errno);
                }else{
                    if(numeroHilos > elementsFile1){    //Verificamos que el número de elementos no supere el número de hilos
                        printf("El numero de hilos supera el numero de elementos del vector \n");
                        printf("Error en el llamado: %s \n", strerror(errno));
                        exit(errno);
                    }else{
                        int *numbersFile1 = malloc(elementsFile1*sizeof(int));
                        int *numbersFile2 = malloc(elementsFile2*sizeof(int));
                        saveNumbers(file1, numbersFile1, elementsFile1);
                        saveNumbers(file2, numbersFile2, elementsFile2);
                        struct vectorStruct *args = malloc(sizeof *args); 
                        args->vectorFile1 = numbersFile1;
                        args->vectorFile2 = numbersFile2;
                        int tasks[numeroHilos];
                        int length = sizeof(tasks)/sizeof(tasks[0]);
                        divTask(numeroHilos, elementsFile1, tasks);
                        args->vectorTasks = &tasks;
                        pthread_t threads[numeroHilos];
                        clock_t begin = clock();    //Tomamos el tiempo de inicio de la multiplicación
                        for(int i = 0; i < numeroHilos; i++){
                            args->idHilo = i;
                            pthread_create(&threads[i], NULL, threadFunction, args);
                            pthread_join(threads[i], NULL);
                        }
                        clock_t end = clock();      //Tomamos el tiempo de finalización de la multiplicación
                        free(args); //Liberamos memoria
                        double time = 0.0;
                        time += (double)(end - begin) / CLOCKS_PER_SEC; //Hallamos el tiempo de ejecución de la multiplicación
                        printf("Resultado: %d \n", result);
                        printf("El tiempo de ejecución de la multiplicación es: %f segundos \n", time);
                    }
                    fclose(file1);  //Cerramos el archivo
                    fclose(file2);  //Cerramos el archivo
                }
            }
        }
    }
    return 0;
}
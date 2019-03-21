#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>

extern int errno;

void saveNumbers(FILE *file, int *vector, int elements){
    char c;
    int counter = 0;
    char number[10];
    number[0] = '\0'; 
    char copy[1];

    while ((c = fgetc(file)) != EOF){
        copy[0] = c;
        if(c == '\n'){
            vector[counter] = atoi(number);
            counter++;
            strcpy(number, "");
        }else{
            strcat(number, copy);
        }
        if(counter == elements - 1){
            vector[counter] = atoi(number);
        }
    }
}

int countElementsVector(FILE *file){
    int counter = 1;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
            counter++;
    }
    return counter;
}

/*int multiplyVectors(int *vector1, int *vector2, int elements){
    int result = 0;
    for (int i = 0; i < elements; i++){
        result += vector1[i] * vector2[i];
    }
    return result;
}*/

void divTask(int threads, int elements, int *vector){
    switch(threads){
        case 2:
            if(elements % 2 == 0){
                vector[0] = elements / 2;
                vector[1] = elements / 2;
            }else{
                vector[0] = floor(elements/2);
                vector[1] = elements - vector[0];
            }
            break;
        case 4:
            if(elements % 4 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/4;
                }
            }else{
                int auxi1 = floor(elements / 2);
                int auxi2 = elements - auxi1;
                vector[0] = floor(auxi1 / 2);
                vector[1] = auxi1 - vector[0];
                vector[2] = floor(auxi2 / 2);
                vector[3] = auxi2 - vector[2];
            }
            break;
        case 8:
            if(elements % 8 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/8;
                }
            }else{
                int auxi1 = floor(elements / 2);
                int auxi2 = elements - auxi1;
                int auxi3 = floor(auxi1 / 2);
                int auxi4 = auxi1 - auxi3;
                int auxi5 = floor(auxi2 / 2);
                int auxi6 = auxi2 - auxi5;
                vector[0] = floor(auxi3 / 2);
                vector[1] = auxi3 - vector[0];
                vector[2] = floor(auxi4 / 2);
                vector[3] = auxi4 - vector[2];
                vector[4] = floor(auxi5 / 2);
                vector[5] = auxi5 - vector[4];
                vector[6] = floor(auxi6 / 2);
                vector[7] = auxi6 - vector[6];
            }
            break;
        case 16:
            if(elements % 16 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/16;
                }
            }else{
                int auxi1 = floor(elements / 2);
                int auxi2 = elements - auxi1;
                int auxi3 = floor(auxi1 / 2);
                int auxi4 = auxi1 - auxi3;
                int auxi5 = floor(auxi2 / 2);
                int auxi6 = auxi2 - auxi5;
                int auxi7 = floor(auxi3 / 2);
                int auxi8 = auxi3 - auxi7;
                int auxi9 = floor(auxi4 / 2);
                int auxi10 = auxi4 - auxi9;
                int auxi11 = floor(auxi5 / 2);
                int auxi12 = auxi5- auxi11;
                int auxi13 = floor(auxi6 / 2);
                int auxi14 = auxi6 - auxi13;
                vector[0] = floor(auxi7 / 2);
                vector[1] = auxi7 - vector[0];
                vector[2] = floor(auxi8 / 2);
                vector[3] = auxi8 - vector[2];
                vector[4] = floor(auxi9 / 2);
                vector[5] = auxi9 - vector[4];
                vector[6] = floor(auxi10 / 2);
                vector[7] = auxi10 - vector[6];
                vector[8] = floor(auxi11 / 2);
                vector[9] = auxi11 - vector[8];
                vector[10] = floor(auxi12 / 2);
                vector[11] = auxi12 - vector[10];
                vector[12] = floor(auxi13 / 2);
                vector[13] = auxi13 - vector[12];
                vector[14] = floor(auxi14 / 2);
                vector[15] = auxi14 - vector[14];
            }
            break;
    }
}

int main (int argc, char *argv[]){
    if(argc != 4){
        printf("Señor usuario, no ingreso los argumentos requeridos \n");
        printf("Error en el llamado: %s \n", strerror(errno));
        exit(errno);
    }else{
        FILE *file1, *file2;
        int aux = atoi(argv[3]);
        if(aux != 2 && aux != 4 && aux != 8 && aux != 16){
            printf("Error en el llamado: %s \n", strerror(errno));
            exit(errno);
        }else{
            if( ((file1 = fopen(argv[1], "r") ) == NULL) || ((file2 = fopen(argv[2], "r") ) == NULL)){
            printf("Error en el llamado: %s \n", strerror(errno));
            exit(errno);
            }else{
                int elemntsFile1 = countElementsVector(file1);
                rewind(file1);
                int elemntsFile2 = countElementsVector(file2);
                rewind(file2);
                if(elemntsFile1 != elemntsFile2){
                    printf("El numero de elementos de los vectores no es el mismo \n");
                    printf("Error en el llamado: %s \n", strerror(errno));
                    exit(errno);
                }else{
                    if(aux > elemntsFile1){
                        printf("El numero de hilos supera el numero de elementos del vector \n");
                        printf("Error en el llamado: %s \n", strerror(errno));
                        exit(errno);
                    }else{
                        int numbersFile1[elemntsFile1];
                        int numbersFile2[elemntsFile2];
                        saveNumbers(file1, numbersFile1, elemntsFile1);
                        saveNumbers(file2, numbersFile2, elemntsFile2);
                        int tasks[aux];
                        divTask(aux, numbersFile1, tasks);
                        switch(aux){
                            case 2:
                                pthread_t h1;
                                pthread_t h2;
                                break;
                            case 4:
                                break;
                            case 8:
                                break;
                            case 16:
                                break;
                        }
                    }
                    fclose(file1);
                    fclose(file2);
                }
            }
        }
    }
    return 0;
}
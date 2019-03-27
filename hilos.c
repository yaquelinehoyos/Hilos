#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>

extern int errno;

void saveNumbers(FILE *file, int *vector, int elements){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int counter = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        vector[counter] = atoi(line);
        counter++;
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

void divTask(int threads, int elements, double *vector){
    switch(threads){
        case 2:
            if(elements % 2 == 0){
                vector[0] = (double)(elements / 2);
                vector[1] = (double)(elements / 2);
            }else{
                vector[0] = floor((double)(elements/2));
                vector[1] = (double)(elements - vector[0]);
            }
            break;
        case 4:
            if(elements % 4 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = (double)(elements/4);
                }
            }else{
                double auxi1 = floor((double)elements / 2);
                double auxi2 = (double)(elements - auxi1);
                vector[0] = floor((double)(auxi1 / 2));
                vector[1] = (double)(auxi1 - vector[0]);
                vector[2] = floor((double)(auxi2 / 2));
                vector[3] = (double)(auxi2 - vector[2]);
            }
            break;
        case 8:
            if(elements % 8 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/8;
                }
            }else{
                double auxi1 = floor((double)(elements / 2));
                double auxi2 = (double)(elements - auxi1);
                double auxi3 = floor((double)(auxi1 / 2));
                double auxi4 = (double)(auxi1 - auxi3);
                double auxi5 = floor((double)(auxi2 / 2));
                double auxi6 = (double)(auxi2 - auxi5);
                vector[0] = floor((double)(auxi3 / 2));
                vector[1] = (double)(auxi3 - vector[0]);
                vector[2] = floor((double)(auxi4 / 2));
                vector[3] = (double)(auxi4 - vector[2]);
                vector[4] = floor((double)(auxi5 / 2));
                vector[5] = (double)(auxi5 - vector[4]);
                vector[6] = floor((double)(auxi6 / 2));
                vector[7] = (double)(auxi6 - vector[6]);
            }
            break;
        case 16:
            if(elements % 16 == 0){
                for(int i = 0; i < threads; i++){
                    vector[i] = elements/16;
                }
            }else{
                double auxi1 = floor((double)(elements / 2));
                double auxi2 = (double)(elements - auxi1);
                double auxi3 = floor((double)(auxi1 / 2));
                double auxi4 = (double)(auxi1 - auxi3);
                double auxi5 = floor((double)(auxi2 / 2));
                double auxi6 = (double)(auxi2 - auxi5);
                double auxi7 = floor((double)(auxi3 / 2));
                double auxi8 = (double)(auxi3 - auxi7);
                double auxi9 = floor((double)(auxi4 / 2));
                double auxi10 = (double)(auxi4 - auxi9);
                double auxi11 = floor((double)(auxi5 / 2));
                double auxi12 = (double)(auxi5- auxi11);
                double auxi13 = floor((double)(auxi6 / 2));
                double auxi14 = (double)(auxi6 - auxi13);
                vector[0] = floor((double)(auxi7 / 2));
                vector[1] = (double)(auxi7 - vector[0]);
                vector[2] = floor((double)(auxi8 / 2));
                vector[3] = (double)(auxi8 - vector[2]);
                vector[4] = floor((double)(auxi9 / 2));
                vector[5] = (double)(auxi9 - vector[4]);
                vector[6] = floor((double)(auxi10 / 2));
                vector[7] = (double)(auxi10 - vector[6]);
                vector[8] = floor((double)(auxi11 / 2));
                vector[9] = (double)(auxi11 - vector[8]);
                vector[10] = floor((double)(auxi12 / 2));
                vector[11] = (double)(auxi12 - vector[10]);
                vector[12] = floor((double)(auxi13 / 2));
                vector[13] = (double)(auxi13 - vector[12]);
                vector[14] = floor((double)(auxi14 / 2));
                vector[15] = (double)(auxi14 - vector[14]);
            }
            break;
    }
}

/*void multiplicar(partición, tamavectores, vector1, vector2, contador o multi){
    for (int i = 0, i < vec [int - 1], i++){
        mul [i] = vector1 [i] * vector2 [i];
        contador++;
    }
}

void resultado (multi, numero elementos, resultado){
    for (int i = 0, i < elementos, i++){
        int = int + multi[i];
    }
}*/

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
                        double tasks[aux];
                        /*divTask(aux, elemntsFile1, tasks);
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
                        }*/
                    }
                    fclose(file1);
                    fclose(file2);
                }
            }
        }
    }
    return 0;
}
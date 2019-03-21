#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

int multiplyVectors(int *vector1, int *vector2, int elements){
    int result = 0;
    for (int i = 0; i < elements; i++){
        result += vector1[i] * vector2[i];
    }
    return result;
}

int main (int argc, char *argv[]){
    if(argc != 3){
        printf("Señor usuario, no ingreso los argumentos requeridos \n");
        printf("Error en el llamado: %s \n", strerror(errno));
        exit(errno);
    }else{
        FILE *file1, *file2;
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
                int numbersFile1[elemntsFile1];
                int numbersFile2[elemntsFile2];
                saveNumbers(file1, numbersFile1, elemntsFile1);
                saveNumbers(file2, numbersFile2, elemntsFile2);
                /*for(int i = 0; i < elemntsFile1; i++){
                    printf("posicion: %d valor: %d \n", i, numbersFile1[i]);
                }
                printf("\n");
                for(int i = 0; i < elemntsFile2; i++){
                    printf("posicion: %d valor: %d \n", i, numbersFile2[i]);
                }*/
                fclose(file1);
                fclose(file2);
                int result = multiplyVectors(numbersFile1, numbersFile2, elemntsFile1);
                printf("\nResultado: %d \n", result);
            }
        }
    }
    return 0;
}
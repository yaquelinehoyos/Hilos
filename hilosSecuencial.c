#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <errno.h>

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
    int counter = 0;
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
                int *numbersFile1 = malloc(elemntsFile1*sizeof(int));
                int *numbersFile2 = malloc(elemntsFile2*sizeof(int));
                saveNumbers(file1, numbersFile1, elemntsFile1);
                saveNumbers(file2, numbersFile2, elemntsFile2);
                fclose(file1);
                fclose(file2);
                clock_t begin = clock();
                int result = multiplyVectors(numbersFile1, numbersFile2, elemntsFile1);
                clock_t end = clock();
                double time = 0.0;
                time += (double)(end - begin) / CLOCKS_PER_SEC;
                printf("Resultado: %d \n", result);
                printf("El tiempo de ejecución de la multiplicación es: %f segundos \n", time);
            }
        }
    }
    return 0;
}
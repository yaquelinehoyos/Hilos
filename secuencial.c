#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <errno.h>

extern int errno;

void saveNumbers(FILE *file, int *vector, int elements){    //Este método se encarga de leer
    char * line = NULL;                                     //los registros línea por línea
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

int multiplyVectors(int *vector1, int *vector2, int elements){  //Este método se encarga de el
    int result = 0;                                             //producto punto entre dos vectores
    for (int i = 0; i < elements; i++){
        result += vector1[i] * vector2[i];
    }
    return result;
}

int main (int argc, char *argv[]){
    if(argc != 3){          //Verificamos que no hayan más o menos argumentos de los necesarios
        printf("Señor usuario, no ingreso los argumentos requeridos \n");
        printf("Error en el llamado: %s \n", strerror(errno));
        exit(errno);
    }else{
        FILE *file1, *file2;
        if( ((file1 = fopen(argv[1], "r") ) == NULL) || ((file2 = fopen(argv[2], "r") ) == NULL)){  //Verificamos que 
            printf("Error en el llamado: %s \n", strerror(errno));                                  //los archivos existan
            exit(errno);
        }else{
            int elemntsFile1 = countElementsVector(file1);
            rewind(file1);  //Reposicionamos el puntero
            int elemntsFile2 = countElementsVector(file2);
            rewind(file2);  //Reposicionamos el puntero
            if(elemntsFile1 != elemntsFile2){   //Verificamos que los dos vectores tengan el mismo número de elementos
                printf("El numero de elementos de los vectores no es el mismo \n");
                printf("Error en el llamado: %s \n", strerror(errno));
                exit(errno);
            }else{
                int *numbersFile1 = malloc(elemntsFile1*sizeof(int));
                int *numbersFile2 = malloc(elemntsFile2*sizeof(int));
                saveNumbers(file1, numbersFile1, elemntsFile1);
                saveNumbers(file2, numbersFile2, elemntsFile2);
                fclose(file1);  //Cerramos el archivo
                fclose(file2);  //Cerramos el archivo
                clock_t begin = clock();    //Tomamos el tiempo de inicio de la multiplicación
                int result = multiplyVectors(numbersFile1, numbersFile2, elemntsFile1);
                clock_t end = clock();      //Tomamos el tiempo de finalización de la multiplicación
                double time = 0.0;
                time += (double)(end - begin) / CLOCKS_PER_SEC; //Hallamos el tiempo de ejecución de la multiplicación
                printf("Resultado: %d \n", result);
                printf("El tiempo de ejecución de la multiplicación es: %f segundos \n", time);
            }
        }
    }
    return 0;
}
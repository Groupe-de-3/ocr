#include <stddef.h>
#include <stdio.h>

#include "data.h"
#include "ia.h"
#include "matrices.h"
#include "save.h"
#include "mnist.h"
#include "struct.h"
#include "time.h"
#include "start.h"
#include "images.h"

void Launch_()
{
    // analyse sudoku
    // obtenir list de ImageView
    
    //int * result = ia_launch(NN, list de ImageView, taille de la list); (renvoi list de chiffres detecté par l'ia)
    //mettre les results dans la grille de sudoku

    // résoudre le sudoku
}


void input_user(neural_network NN)
{
    char *filename = malloc(sizeof(char) * 30);
    char *output = malloc(sizeof(char) * 10);
    size_t nb_sample = 0;
    while (output[0] != 'q')
    {
        printf("Command : ");

        scanf("%s", output);
        printf("\n");
        switch (output[0])
        {
             case 'l':
                printf("Launch\n");
                Launch_();
                break;
            

            case 'q': 
                printf("Quit\n");
                break;
            
            default:
                printf("Only commands are: \n q : quit\n l : launch\n\n");
                break;
        }
    }

    free(output);
    free(filename);

}


int main() {

    printf("%s", "Launch\n\n");

    //init neural network
    neural_network NN = ia_load("ocr.txt");

    input_user(NN);

    // free the memory
    ia_memory_free(&NN);

    return 0;
}

#include "save.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TRAITEMENT FICHIER / neural network

void clear() {
    if (remove("neural_network.txt") != 0)
        errx(1, "Could not delete file neural_network.txt");
}

void save(neural_network *NN, int layers_number, int layers_sizes[]) // save the neural network in the file "neural_network"
{

    clear();
    FILE *fichier = NULL;
    fichier       = fopen("neural_network.txt", "w");

    fprintf(fichier, "%i\n", layers_number);

    for (int i = 0; i < layers_number; i++) {
        fprintf(fichier, "%i\n", layers_sizes[i]);
    }

    for (int layer_ind = 0; layer_ind < layers_number; layer_ind++) {
        for (int weight_ind = 0; weight_ind < layers_sizes[layer_ind];
             weight_ind++) {
            fprintf(
                fichier, "%f\n", NN->layers_[layer_ind].layer_[weight_ind].bias
            );
            if (layer_ind < layers_number - 1) {
                for (int j = 0; j < layers_sizes[layer_ind + 1]; j++)
                    fprintf(
                        fichier, "%f\n",
                        NN->layers_[layer_ind].layer_[weight_ind].weights[j]
                    );
            }
        }
    }

    fclose(fichier);
}

neural_network load(char* file_name) // initialisation of the neural network from a file
{
    neural_network NN;

    FILE *fichier = NULL;
    fichier       = fopen(file_name, "r");

    // ini base
    char chaine[10] = "";
    fgets(chaine, 10, fichier);
    int layers_number = atoi(chaine);

    int layers_sizes[layers_number];
    for (int i = 0; i < layers_number; i++)
    {
        fgets(chaine, 10, fichier);
        layers_sizes[i] = atoi(chaine);
    }

    for (int layer_ind = 0; layer_ind < layers_number; layer_ind++) {
        Layer Layer_;
        for (int i = 0; i < layers_sizes[layer_ind]; i++) {
            //create neural && read neural

            N    new_neurone;
            char chaine2[10] = "";

            fgets(chaine2, 10, fichier); // biais
            new_neurone.bias = atof(chaine2);

            for (int j = 0; j < layers_sizes[layer_ind + 1]; j++) // weights
            {
                fgets(chaine2, 10, fichier);
                double res             = atof(chaine2);
                new_neurone.weights[j] = res;
            }

            //add neurone
            Layer_.layer_[i] = new_neurone;
        }

        //add layer
        NN.layers_[layer_ind] = Layer_;
    }

    fclose(fichier);

    return NN;
}

neural_network init(int layers_number, int layers_sizes[], double inputs[])// initialisation of a new neural network
{
    neural_network NN;

    for (int layer_ind = 0; layer_ind < layers_number; layer_ind++) {
        Layer Layer_;
        for (int i = 0; i < layers_sizes[layer_ind]; i++) {
            N new_neurone; //create neural

            if (layer_ind == 0)
                new_neurone.bias = inputs[i];
            else
                new_neurone.bias = (rand() / ((double)RAND_MAX)); // ini biais

            if (i < layers_number - 1) // ini weights
            {
                for (int j = 0; j < layers_sizes[layer_ind + 1]; j++)
                    new_neurone.weights[j] = (rand() / ((double)RAND_MAX));
            }

            Layer_.layer_[i] = new_neurone; //add neurone
        }

        NN.layers_[layer_ind] = Layer_; //add layer
    }

    return NN;
}

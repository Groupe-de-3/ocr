#include "save.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int    layers_number  = 3; // number of hidden layer
int    layers_sizes[] = {2, 5, 1};
double inputs[]       = {1, 0};

// TRAITEMENT FICHIER / neural network

void clear() {
    if (remove("neural_network.txt") != 0)
        errx(1, "Could not delete file neural_network.txt");
}

void save(neural_network NN) {
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
                fichier, "%f\n", NN.layers_[layer_ind].layer_[weight_ind].bias
            );
            if (layer_ind < layers_number - 1) {
                for (int j = 0; j < layers_sizes[layer_ind + 1]; j++)
                    fprintf(
                        fichier, "%f\n",
                        NN.layers_[layer_ind].layer_[weight_ind].weights[j]
                    );
            }
        }
    }

    fclose(fichier);
}

neural_network load() {
    neural_network NN;

    FILE *fichier = NULL;
    fichier       = fopen("neural_network.txt", "r");

    // ini base
    char chaine[10] = "";
    fgets(chaine, 10, fichier);
    layers_number = atoi(chaine);

    for (int i = 0; i < layers_number; i++) {
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

neural_network init() {
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

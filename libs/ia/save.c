#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int    layers_number  = 3; // number of idden layer
int    layers_sizes[] = {2, 5, 1};
double inputs[]       = {1, 0};

typedef struct Neurone {
    double biais;
    double weights[10];

} N;

typedef struct Layer {
    N layer_[10];
} Layer;

typedef struct neural_network {
    Layer layers_[10];
} neural_network;

double str_to_double(char *chaine) {
    double res = chaine[0] - 48;
    int    i   = 2;
    while (chaine[i] != '\0') {
        res += (chaine[i] - 48) / (pow(10, i - 1));
        i++;
    }

    return res;
}

int str_to_int(char *chaine) {
    int res = chaine[0] - 48;
    int i   = 2;
    while (chaine[i] != '\0') {
        res += (chaine[i] - 48) / (pow(10, i - 1));
        i++;
    }

    return res;
}

// TRAITEMENT FICHIER / neural network

void clear() // remove neural_network data
{
    system("rm neural_network.txt");
}

void save(neural_network NN
) // save the neural network in the file "neural_network"
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
                fichier, "%f\n", NN.layers_[layer_ind].layer_[weight_ind].biais
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

neural_network load() // initialisation of the neural network from a file
{
    neural_network NN;

    FILE *fichier = NULL;
    fichier       = fopen("neural_network.txt", "r");

    // ini base
    char chaine[10] = "";
    fgets(chaine, 10, fichier);
    layers_number = str_to_int(chaine);

    for (int i = 0; i < layers_number; i++) {
        fgets(chaine, 10, fichier);
        layers_sizes[i] = str_to_int(chaine);
    }

    for (int layer_ind = 0; layer_ind < layers_number; layer_ind++) {
        Layer Layer_;
        for (int i = 0; i < layers_sizes[layer_ind]; i++) {
            //create neural && read neural

            N    new_neurone;
            char chaine[10] = "";

            fgets(chaine, 10, fichier); // biais
            new_neurone.biais = str_to_double(chaine);

            for (int j = 0; j < layers_sizes[layer_ind + 1]; j++) // weights
            {
                fgets(chaine, 10, fichier);
                double res             = str_to_double(chaine);
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

neural_network init() // initialisation of a new neural network
{
    neural_network NN;

    for (int layer_ind = 0; layer_ind < layers_number; layer_ind++) {
        Layer Layer_;
        for (int i = 0; i < layers_sizes[layer_ind]; i++) {
            N new_neurone; //create neural

            if (layer_ind == 0)
                new_neurone.biais = inputs[i];
            else
                new_neurone.biais = (rand() / ((double)RAND_MAX)); // ini biais

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

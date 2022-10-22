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

void save(neural_network *NN) // save the neural network in the file "neural_network"
{
    FILE *f = NULL;
    if ((f = fopen("neural_network.txt", "r")))
    {
        fclose(f);
        clear();
    }
    
    FILE *fichier  = NULL;
    fichier = fopen("neural_network.txt", "w");


    
    fprintf(fichier, "%i\n", NN->layers_number);

    
    for (int i = 0; i < NN->layers_number; i++) 
        fprintf(fichier, "%i\n", NN->layers_sizes[i]);

    
    
    for (int layer_ind = 0; layer_ind < NN->layers_number; layer_ind++) {
        for (int weight_ind = 0; weight_ind < NN->layers_sizes[layer_ind];
             weight_ind++) {
            fprintf(
                fichier, "%f\n", NN->layers_[layer_ind].neural_list[weight_ind].bias
            );
            if (layer_ind < NN->layers_number - 1) {
                for (int j = 0; j < NN->layers_sizes[layer_ind]; j++) // a +1
                    fprintf(
                        fichier, "%f\n",
                        NN->layers_[layer_ind].neural_list[weight_ind].weights[j]
                    );
            }
        }
    }
    

    fclose(fichier);
}

neural_network * load(char* file_name) // initialisation of the neural network from a file
{
    neural_network NN;

    FILE *fichier = NULL;
    fichier       = fopen(file_name, "r");

    // ini base
    char chaine[10] = "";
    fgets(chaine, 10, fichier);
    NN.layers_number = atoi(chaine);

    NN.layers_ = malloc((uint) NN.layers_number * sizeof(Layer));


    NN.layers_sizes = malloc(sizeof(int) * NN.layers_number);
    for (int i = 0; i < NN.layers_number; i++)
    {
        fgets(chaine, 10, fichier);
        NN.layers_sizes[i] = atoi(chaine);
    }

    

    for (int layer_ind = 0; layer_ind < NN.layers_number; layer_ind++) {
        
        Layer Layer_;

        //NN.layers_[layer_ind].neural_list = malloc((uint) layers_sizes[layer_ind] * sizeof(Neurone));

        Layer_.layer_size = NN.layers_sizes[layer_ind]; // ini size of the layer

        for (int i = 0; i < NN.layers_sizes[layer_ind]; i++) {
            //create neural && read neural

            Neurone new_neurone;
            char chaine2[10] = "";

            fgets(chaine2, 10, fichier); // biais
            new_neurone.bias = atof(chaine2);

            
           // new_neurone.weights = malloc(sizeof(double) * (uint) layers_sizes[layer_ind]); // a modif

            for (int j = 0; j < NN.layers_sizes[layer_ind]; j++) // weights
            {
                fgets(chaine2, 10, fichier);
                double res             = atof(chaine2);
                new_neurone.weights[j] = res;
            }

            //add neurone
            Layer_.neural_list[i] = new_neurone;
        }

        //add layer
        NN.layers_[layer_ind] = Layer_;
    }

    fclose(fichier);

    return &NN;
}



void memory_free(neural_network *NN)
{
    free(NN->layers_sizes);
    
    for (int i = 0; i < NN->layers_number ; i++)
    {   
        
        for (int j = 0; j < NN->layers_sizes[i]; j++)
        {
            free(NN->layers_[i].neural_list[j].weights);
        }

        free(NN->layers_[i].neural_list);
    }

    free(NN->layers_);
    
}

neural_network init(int layers_number, int* layers_sizes)// initialisation of a new neural network
{
    
    neural_network NN;
    
    Layer *array_layer = malloc((uint) layers_number * sizeof(Layer));

    NN.layers_sizes = layers_sizes;
    NN.layers_number = layers_number;
    
    
    
    for (int layer_ind = 0; layer_ind < layers_number; layer_ind++) {
        
        Layer Layer_;
        
        Layer_.layer_size = layers_sizes[layer_ind]; // ini size of the layer
        
        Neurone *array_neural = malloc((uint) layers_sizes[layer_ind] * sizeof(Neurone));

        for (int i = 0; i < layers_sizes[layer_ind]; i++) {

            Neurone new_neurone; //create neural
            //new_neurone.weights = malloc(sizeof(double) * layers_sizes[layer_ind]);
            //new_neurone.weights[0] = (rand() / ((double)RAND_MAX));
            
            new_neurone.bias = (double) (rand() / ((double)RAND_MAX)); // ini biais
            
            double *array_weight = malloc(sizeof(double) * (uint) layers_sizes[layer_ind]); // a modif
            for (int j = 0; j < layers_sizes[layer_ind]; j++)
            {
                //double d = (rand() / ((double)RAND_MAX));
                array_weight[j] = 0.5;
            }
                
            

            new_neurone.weights = array_weight;

            array_neural[i] = new_neurone; //add neurone
            
        }


        Layer_.neural_list = array_neural;
        array_layer[layer_ind] = Layer_; //add layer
    }

    NN.layers_ = array_layer;
    
    return NN;
}

#include "save.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>

// TRAITEMENT FICHIER / neural network

void ia_clear() {
    if (remove("neural_network.txt") != 0)
        errx(1, "Could not delete file neural_network.txt");
}


void ia_save(neural_network *NN) // save the neural network in the file "neural_network"
{
    FILE *f = NULL;
    if ((f = fopen("neural_network.txt", "r")))
    {
        fclose(f);
        ia_clear();
    }
    
    FILE *fichier  = NULL;
    fichier = fopen("neural_network.txt", "w");


    
    fprintf(fichier, "%zu\n", NN->layers_number);

    
    for (size_t i = 0; i < NN->layers_number; i++) 
        fprintf(fichier, "%zu\n", NN->layers_sizes[i]);

    
    for (size_t layer_ind = 1; layer_ind < NN->layers_number; layer_ind++) {

        for (size_t j = 0; j < m_length(NN->layers_[layer_ind].m_weight); j++)
        {
            fprintf(fichier, "%f\n", NN->layers_[layer_ind].m_weight[j]);
        }
    
        for (size_t j = 0; j < m_length(NN->layers_[layer_ind].m_bias); j++)
        {
            fprintf(fichier, "%f\n", NN->layers_[layer_ind].m_bias[j]);
        }
    }

    fclose(fichier);
}


neural_network * ia_load(char* file_name) // initialisation of the neural network from a file
{
    neural_network * NN = NULL;

    FILE *fichier = NULL;
    fichier       = fopen(file_name, "r");

    // ini base
    char chaine[10] = "";

    fgets(chaine, 10, fichier);
    NN->layers_number = (size_t) atoi(chaine);

    NN->layers_ = malloc((uint) NN->layers_number * sizeof(Layer));


    NN->layers_sizes = malloc(sizeof(int) * NN->layers_number);
    for (size_t i = 0; i < NN->layers_number; i++)
    {
        fgets(chaine, 10, fichier);
        NN->layers_sizes[i] = (size_t) atoi(chaine);
    }


    Layer *array_layer = malloc((uint) NN->layers_number * sizeof(Layer));
    
    for (size_t layer_ind = 1; layer_ind < NN->layers_number; layer_ind++) {
        
        Layer Layer_;
        
        Layer_.layer_size = NN->layers_sizes[layer_ind]; // ini size of the layer

        Layer_.m_bias = m_new(double, Layer_.layer_size, 1);
        Layer_.m_weight = m_new(double, Layer_.layer_size, NN->layers_sizes[layer_ind-1]);
        
        for (size_t i = 0; i < Layer_.layer_size; i++) {

            fgets(chaine, 10, fichier); // biais
            m_get2(Layer_.m_bias, i, 0) = (double) atof(chaine); // ini biais
        }

        for (size_t i = 0; i < Layer_.layer_size; i++) {

            for (size_t j = 0; j < NN->layers_sizes[layer_ind-1]; j++)
            {
                fgets(chaine, 10, fichier);
                m_get2(Layer_.m_weight, i, j) = (double) atof(chaine); // ini weights
            }
        }
        
        array_layer[layer_ind] = Layer_; //add layer
    }

    NN->layers_ = array_layer;

    fclose(fichier);

    return NN;
}



void ia_memory_free(neural_network *NN)
{
    //free(NN->layers_sizes);
    
    for (size_t i = 1; i < NN->layers_number ; i++)
    {   
        m_destroy(NN->layers_[i].m_bias);
        m_destroy(NN->layers_[i].m_weight);
    }

    free(NN->layers_);
    
}

neural_network ia_init(size_t layers_number, size_t* layers_sizes)// initialisation of a new neural network
{
    
    neural_network NN;
    
    Layer *array_layer = malloc((uint) layers_number * sizeof(Layer));

    NN.layers_sizes = layers_sizes;
    NN.layers_number = layers_number;
    
    
    
    for (size_t layer_ind = 1; layer_ind < layers_number; layer_ind++) {
        
        Layer Layer_;
        
        Layer_.layer_size = layers_sizes[layer_ind]; // ini size of the layer

        Layer_.m_bias = m_new(double, Layer_.layer_size, 1);
        Layer_.m_weight = m_new(double, Layer_.layer_size, layers_sizes[layer_ind-1]);

        for (size_t i = 0; i < Layer_.layer_size; i++) {

            m_get2(Layer_.m_bias, i, 0) = (double) (rand() / ((double)RAND_MAX));; // ini biais
            
            for (size_t j = 0; j < layers_sizes[layer_ind-1]; j++)
            {
                m_get2(Layer_.m_weight, i, j) = (double) (rand() / ((double)RAND_MAX)); // ini weights
            }
        }
        
        array_layer[layer_ind] = Layer_; //add layer
    }

    NN.layers_ = array_layer;
    
    return NN;
}

#include "save.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>

// TRAITEMENT FICHIER / neural network

void ia_clear(char *filename) {
    if (remove(filename) != 0)
        errx(1, "Could not delete file neural_network.txt");
}

void ia_save(neural_network *NN, char * filename) // save the neural network in the file "neural_network"
{
    FILE *f = NULL;
    if ((f = fopen(filename, "r")))
    {
        fclose(f);
        ia_clear(filename);
    }
    
    FILE *fichier  = NULL;
    fichier = fopen(filename, "w");


    
    fprintf(fichier, "%zu\n", NN->layers_number);

    for (size_t i = 0; i < NN->layers_number +1; i++) 
        fprintf(fichier, "%zu\n", NN->layers_sizes[i]);

    
    for (size_t layer_ind = 0; layer_ind < NN->layers_number; layer_ind++) {

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

neural_network ia_load(char* file_name) // initialisation of the neural network from a file
{
    
    neural_network NN;

    FILE *fichier = NULL;
    fichier       = fopen(file_name, "r");

    // ini base
    char chaine[15] = "";

    fgets(chaine, 10, fichier);
    NN.layers_number = (size_t) atoi(chaine);


    NN.layers_sizes = malloc(sizeof(size_t) * (NN.layers_number+1));

    for (size_t i = 0; i < NN.layers_number+1; i++)
    {
        fgets(chaine, 5, fichier);
        NN.layers_sizes[i] = (size_t) atoi(chaine);
    }

    NN.layers_ = malloc(NN.layers_number * sizeof(Layer));
    
    for (size_t layer_ind = 0; layer_ind < NN.layers_number; layer_ind++) {
        
        Layer Layer_;
        
        Layer_.layer_size = NN.layers_sizes[layer_ind + 1]; // ini size of the layer
        
        Layer_.m_bias = m_new(double, 1, Layer_.layer_size);
        Layer_.m_gradB = m_new(double, 1,  Layer_.layer_size);

        Layer_.m_weight = m_new(double, NN.layers_sizes[layer_ind], Layer_.layer_size);
        Layer_.m_gradW  = m_new(double, 1, Layer_.layer_size);

        Layer_.last_output = m_new(double, 1, Layer_.layer_size);
        Layer_.last_output_activated = m_new(double, 1, Layer_.layer_size);

        Layer_.loss = m_new(double, 1,  Layer_.layer_size);
        
        
        for (size_t i = 0; i < NN.layers_sizes[layer_ind]; i++) {

            for (size_t j = 0; j < Layer_.layer_size; j++)
            {
                fgets(chaine, 15, fichier);
                m_get2(Layer_.m_weight, i, j) = (double) atof(chaine); // ini weights
            }
        }

        for (size_t i = 0; i < Layer_.layer_size; i++) {

            fgets(chaine, 15, fichier); // biais
            m_get2(Layer_.m_bias, 0, i) = (double) atof(chaine); // ini biais
        }

        
        NN.layers_[layer_ind] = Layer_; //add layer
    }

    NN.layers_ = NN.layers_;

    fclose(fichier);

    return NN;
}

void ia_memory_free(neural_network *NN) // free the memory
{
    free(NN->layers_sizes);
    
    for (size_t i = 0; i < NN->layers_number ; i++)
    {   
        m_destroy(NN->layers_[i].m_bias);
        m_destroy(NN->layers_[i].m_weight);
        m_destroy(NN->layers_[i].last_output);
        m_destroy(NN->layers_[i].last_output_activated);
        m_destroy(NN->layers_[i].m_gradB);
        m_destroy(NN->layers_[i].m_gradW);

        m_destroy(NN->layers_[i].loss);
    }

    free(NN->layers_);
    
}

neural_network ia_init(size_t layers_number, size_t* layers_sizes)// initialisation of a new neural network
{
    //set seed
    //srand(170);
    
    neural_network NN;
    
    Layer *array_layer = malloc((uint) layers_number * sizeof(Layer));

    NN.layers_sizes = layers_sizes;
    NN.layers_number = layers_number;
    
    
    
    for (size_t layer_ind = 0; layer_ind < layers_number; layer_ind++) {
        
        Layer Layer_;
        
        Layer_.layer_size = layers_sizes[layer_ind + 1]; // ini size of the layer
        
        Layer_.m_bias = m_new(double, 1, Layer_.layer_size);
        Layer_.m_gradB = m_new(double, 1,  Layer_.layer_size);
        Layer_.last_output = m_new(double, 1, Layer_.layer_size);
        Layer_.last_output_activated = m_new(double, 1, Layer_.layer_size);
        Layer_.m_weight = m_new(double, layers_sizes[layer_ind], Layer_.layer_size);
        Layer_.m_gradW  = m_new(double, layers_sizes[layer_ind], Layer_.layer_size);
        
        Layer_.loss = m_new(double, 1,  Layer_.layer_size);

        for (size_t i = 0; i < Layer_.layer_size; i++) {

            m_get2(Layer_.m_bias, 0, i) = (double) (rand() / ((double)RAND_MAX) * 2 -1); // ini biais
            
            for (size_t j = 0; j < layers_sizes[layer_ind]; j++)
            {
                m_get2(Layer_.m_weight, j, i) = (double) (rand() / ((double)RAND_MAX) * 2 -1); // ini weights
            }
        }
        
        array_layer[layer_ind] = Layer_; //add layer
    }

    NN.layers_ = array_layer;
    
    return NN;
}

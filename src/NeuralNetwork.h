//
// Created by Olcay Taner YILDIZ on 3.10.2023.
//

#ifndef WORDTOVEC_NEURALNETWORK_H
#define WORDTOVEC_NEURALNETWORK_H

#include <Dictionary/VectorizedDictionary.h>
#include "Vocabulary.h"
#include "WordToVecParameter.h"

static int EXP_TABLE_SIZE = 1000;
static int MAX_EXP = 6;

struct neural_network{
    double** word_vectors;
    double** word_vector_update;
    Vocabulary_ptr vocabulary;
    Word_to_vec_parameter_ptr parameter;
    Corpus_ptr corpus;
    Array_list_ptr exp_table;
    int vector_length;
};

typedef struct neural_network Neural_network;

typedef Neural_network *Neural_network_ptr;

Neural_network_ptr create_neural_network(Corpus_ptr corpus, Word_to_vec_parameter_ptr parameter);

void free_neural_network(Neural_network_ptr neural_network);

void prepare_exp_table(Neural_network_ptr neural_network);

double calculate_g(Neural_network_ptr neural_network, double f, double alpha, double label);

Vectorized_dictionary_ptr train(Neural_network_ptr neural_network);

void update_output(Neural_network_ptr neural_network, double* outputUpdate, const double* outputs, int l2, double g);

double dot_product_array(Neural_network_ptr neural_network, const double* vector1, const double* vector2);

void train_cbow(Neural_network_ptr neural_network);

void train_skip_gram(Neural_network_ptr neural_network);

#endif //WORDTOVEC_NEURALNETWORK_H

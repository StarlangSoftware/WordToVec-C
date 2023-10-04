//
// Created by Olcay Taner YILDIZ on 3.10.2023.
//

#include <stdlib.h>
#include <math.h>
#include "NeuralNetwork.h"
#include "Iteration.h"

/**
 * Constructor for the NeuralNetwork class. Gets corpus and network parameters as input and sets the
 * corresponding parameters first. After that, initializes the network with random weights between -0.5 and 0.5.
 * Constructs vector update matrix and prepares the exp table.
 * @param corpus Corpus used to train word vectors using Word2Vec algorithm.
 * @param parameter Parameters of the Word2Vec algorithm.
 */
Neural_network_ptr create_neural_network(Corpus_ptr corpus, Word_to_vec_parameter_ptr parameter) {
    Neural_network_ptr result = malloc(sizeof(Neural_network));
    int row;
    srandom(parameter->seed);
    result->vocabulary = create_vocabulary(corpus);
    result->parameter = parameter;
    result->vector_length = parameter->layer_size;
    result->corpus = corpus;
    result->exp_table = create_array_list();
    row = size_of_vocabulary(result->vocabulary);
    result->word_vectors = malloc(row * sizeof(double*));
    for (int i = 0; i < row; i++){
        result->word_vectors[i] = malloc(sizeof(double));
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < result->vector_length; j++) {
            result->word_vectors[i][j] = -0.5 + ((double)random()) / RAND_MAX;
        }
    }
    result->word_vector_update = malloc(row * sizeof(double*));
    for (int i = 0; i < row; i++){
        result->word_vector_update[i] = malloc(sizeof(double));
    }
    prepare_exp_table(result);
    return result;
}

void free_neural_network(Neural_network_ptr neural_network) {
    int row = size_of_vocabulary(neural_network->vocabulary);
    for (int i = 0; i < row; i++){
        free(neural_network->word_vector_update[i]);
    }
    free(neural_network->word_vector_update);
    for (int i = 0; i < row; i++){
        free(neural_network->word_vectors[i]);
    }
    free(neural_network->word_vectors);
    free_vocabulary(neural_network->vocabulary);
    free_array_list(neural_network->exp_table, free);
    free(neural_network);
}

/**
 * Constructs the fast exponentiation table. Instead of taking exponent at each time, the algorithm will lookup
 * the table.
 */
void prepare_exp_table(Neural_network_ptr neural_network) {
    for (int i = 0; i < EXP_TABLE_SIZE; i++) {
        double value = exp((i / (EXP_TABLE_SIZE + 0.0) * 2 - 1) * MAX_EXP);
        array_list_add_double(neural_network->exp_table, value / (value + 1));
    }
    array_list_add_double(neural_network->exp_table, 0.0);
}

/**
 * Calculates G value in the Word2Vec algorithm.
 * @param f F value.
 * @param alpha Learning rate alpha.
 * @param label Label of the instance.
 * @return Calculated G value.
 */
double calculate_g(Neural_network_ptr neural_network, double f, double alpha, double label) {
    if (f > MAX_EXP){
        return (label - 1) * alpha;
    } else {
        if (f < -MAX_EXP){
            return label * alpha;
        } else {
            int index = (int) ((f + MAX_EXP) * (EXP_TABLE_SIZE / MAX_EXP / 2));
            if (index >= 0 && index < EXP_TABLE_SIZE){
                return (label - array_list_get_double(neural_network->exp_table, index)) * alpha;
            } else {
                return (label - 1) * alpha;
            }
        }
    }
}

/**
 * Main method for training the Word2Vec algorithm. Depending on the training parameter, CBox or SkipGram algorithm
 * is applied.
 * @return Dictionary of word vectors.
 */
Vectorized_dictionary_ptr train(Neural_network_ptr neural_network) {
    Vectorized_dictionary_ptr result = create_vectorized_dictionary();
    if (neural_network->parameter->cbow){
        train_cbow(neural_network);
    } else {
        train_skip_gram(neural_network);
    }
    for (int i = 0; i < size_of_vocabulary(neural_network->vocabulary); i++){
        Vector_ptr vector = create_vector2(0, 0);
        for (int j = 0; j < neural_network->vector_length; j++){
            add_value_to_vector(vector, neural_network->word_vectors[i][j]);
        }
        add_word_vectorized(result, create_vectorized_word(vocabulary_get_word(neural_network->vocabulary, i)->name, vector));
    }
    sort_vectorized(result);
    return result;
}

void update_output(Neural_network_ptr neural_network,
                   double *outputUpdate,
                   const double *outputs,
                   int l2,
                   double g) {
    for (int j = 0; j < neural_network->vector_length; j++){
        outputUpdate[j] += neural_network->word_vector_update[l2][j] * g;
    }
    for (int j = 0; j < neural_network->vector_length; j++){
        neural_network->word_vector_update[l2][j] += outputs[j] * g;
    }
}

double dot_product_array(Neural_network_ptr neural_network, const double *vector1, const double *vector2) {
    double sum = 0;
    for (int j = 0; j < neural_network->vector_length; j++){
        sum += vector1[j] * vector2[j];
    }
    return sum;
}

/**
 * Main method for training the CBow version of Word2Vec algorithm.
 */
void train_cbow(Neural_network_ptr neural_network) {
    int wordIndex, lastWordIndex;
    Iteration_ptr iteration = create_iteration(neural_network->corpus, neural_network->parameter);
    int target, label, l2, b, cw;
    double f, g;
    corpus_open(neural_network->corpus);
    Sentence_ptr currentSentence = corpus_get_sentence2(neural_network->corpus);
    Vocabulary_word_ptr currentWord;
    double* outputs = malloc(neural_network->vector_length * sizeof(double));
    double* outputUpdate = malloc(neural_network->vector_length * sizeof(double));
    while (iteration->iteration_count < neural_network->parameter->number_of_iterations) {
        alpha_update(iteration, neural_network->vocabulary->total_number_of_words);
        wordIndex = get_position(neural_network->vocabulary, array_list_get(currentSentence->words, iteration->sentence_position));
        currentWord = vocabulary_get_word(neural_network->vocabulary, wordIndex);
        for (int i = 0; i < neural_network->vector_length; i++){
            outputs[i] = 0;
            outputUpdate[i] = 0;
        }
        b = random() % neural_network->parameter->window;
        cw = 0;
        for (int a = b; a < neural_network->parameter->window * 2 + 1 - b; a++){
            int c = iteration->sentence_position - neural_network->parameter->window + a;
            if (a != neural_network->parameter->window && sentence_safe_index(currentSentence, c)) {
                lastWordIndex = get_position(neural_network->vocabulary, array_list_get(currentSentence->words, c));
                for (int j = 0; j < neural_network->vector_length; j++){
                    outputs[j] += neural_network->word_vectors[lastWordIndex][j];
                }
                cw++;
            }
        }
        if (cw > 0) {
            for (int j = 0; j < neural_network->vector_length; j++){
                outputs[j] /= cw;
            }
            if (neural_network->parameter->hierarchical_soft_max){
                for (int d = 0; d < currentWord->code_length; d++) {
                    l2 = currentWord->point[d];
                    f = dot_product_array(neural_network, outputs, neural_network->word_vector_update[l2]);
                    if (f <= -MAX_EXP || f >= MAX_EXP){
                        continue;
                    } else{
                        int index = (int)((f + MAX_EXP) * (EXP_TABLE_SIZE / MAX_EXP / 2));
                        if (index >= 0 && index < EXP_TABLE_SIZE){
                            f = array_list_get_double(neural_network->exp_table, index);
                        } else {
                            continue;
                        }
                    }
                    g = (1 - currentWord->code[d] - f) * iteration->alpha;
                    update_output(neural_network, outputUpdate, outputs, l2, g);
                }
            } else {
                for (int d = 0; d < neural_network->parameter->negative_sampling_size + 1; d++) {
                    if (d == 0) {
                        target = wordIndex;
                        label = 1;
                    } else {
                        target = get_table_value(neural_network->vocabulary, random() % neural_network->vocabulary->table->size);
                        if (target == 0)
                            target = random() % (size_of_vocabulary(neural_network->vocabulary) - 1) + 1;
                        if (target == wordIndex)
                            continue;
                        label = 0;
                    }
                    l2 = target;
                    f = dot_product_array(neural_network, outputs, neural_network->word_vector_update[l2]);
                    g = calculate_g(neural_network, f, iteration->alpha, label);
                    update_output(neural_network, outputUpdate, outputs, l2, g);
                }
            }
            for (int a = b; a < neural_network->parameter->window * 2 + 1 - b; a++){
                int c = iteration->sentence_position - neural_network->parameter->window + a;
                if (a != neural_network->parameter->window && sentence_safe_index(currentSentence, c)) {
                    lastWordIndex = get_position(neural_network->vocabulary, array_list_get(currentSentence->words, c));
                    for (int j = 0; j < neural_network->vector_length; j++){
                        neural_network->word_vectors[lastWordIndex][j] += outputUpdate[j];
                    }
                }
            }
        }
        currentSentence = sentence_update(iteration, currentSentence);
    }
    corpus_close(neural_network->corpus);
}

/**
 * Main method for training the SkipGram version of Word2Vec algorithm.
 */
void train_skip_gram(Neural_network_ptr neural_network) {
    int wordIndex, lastWordIndex;
    Iteration_ptr iteration = create_iteration(neural_network->corpus, neural_network->parameter);
    int target, label, l1, l2, b;
    double f, g;
    corpus_open(neural_network->corpus);
    Sentence_ptr currentSentence = corpus_get_sentence2(neural_network->corpus);
    Vocabulary_word_ptr currentWord;
    double* outputUpdate = malloc(neural_network->vector_length * sizeof(double));
    while (iteration->iteration_count < neural_network->parameter->number_of_iterations) {
        alpha_update(iteration, neural_network->vocabulary->total_number_of_words);
        wordIndex = get_position(neural_network->vocabulary, array_list_get(currentSentence->words, iteration->sentence_position));
        currentWord = vocabulary_get_word(neural_network->vocabulary, wordIndex);
        for (int i = 0; i < neural_network->vector_length; i++){
            outputUpdate[i] = 0;
        }
        b = random() % neural_network->parameter->window;
        for (int a = b; a < neural_network->parameter->window * 2 + 1 - b; a++) {
            int c = iteration->sentence_position - neural_network->parameter->window + a;
            if (a != neural_network->parameter->window && sentence_safe_index(currentSentence, c)) {
                lastWordIndex = get_position(neural_network->vocabulary, array_list_get(currentSentence->words, c));
                l1 = lastWordIndex;
                for (int i = 0; i < neural_network->vector_length; i++){
                    outputUpdate[i] = 0;
                }
                if (neural_network->parameter->hierarchical_soft_max) {
                    for (int d = 0; d < currentWord->code_length; d++) {
                        l2 = currentWord->point[d];
                        f = dot_product_array(neural_network, neural_network->word_vectors[l1], neural_network->word_vector_update[l2]);
                        if (f <= -MAX_EXP || f >= MAX_EXP){
                            continue;
                        } else{
                            int index = (int)((f + MAX_EXP) * (EXP_TABLE_SIZE / MAX_EXP / 2));
                            if (index >= 0 && index < EXP_TABLE_SIZE){
                                f = array_list_get_double(neural_network->exp_table, index);
                            } else {
                                continue;
                            }
                        }
                        g = (1 - currentWord->code[d] - f) * iteration->alpha;
                        update_output(neural_network, outputUpdate, neural_network->word_vectors[l1], l2, g);
                    }
                } else {
                    for (int d = 0; d < neural_network->parameter->negative_sampling_size + 1; d++) {
                        if (d == 0) {
                            target = wordIndex;
                            label = 1;
                        } else {
                            target = get_table_value(neural_network->vocabulary, random() % neural_network->vocabulary->table->size);
                            if (target == 0)
                                target = random() % (size_of_vocabulary(neural_network->vocabulary) - 1) + 1;
                            if (target == wordIndex)
                                continue;
                            label = 0;
                        }
                        l2 = target;
                        f = dot_product_array(neural_network, neural_network->word_vectors[l1], neural_network->word_vector_update[l2]);
                        g = calculate_g(neural_network, f, iteration->alpha, label);
                        update_output(neural_network, outputUpdate, neural_network->word_vectors[l1], l2, g);
                    }
                }
                for (int j = 0; j < neural_network->vector_length; j++){
                    neural_network->word_vectors[l1][j] += outputUpdate[j];
                }
            }
        }
        currentSentence = sentence_update(iteration, currentSentence);
    }
    corpus_close(neural_network->corpus);
}

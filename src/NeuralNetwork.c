//
// Created by Olcay Taner YILDIZ on 3.10.2023.
//

#include <stdlib.h>
#include <math.h>
#include <Memory/Memory.h>
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
    Neural_network_ptr result = malloc_(sizeof(Neural_network));
    int row;
    srandom(parameter->seed);
    result->vocabulary = create_vocabulary(corpus);
    result->parameter = parameter;
    result->vector_length = parameter->layer_size;
    result->corpus = corpus;
    result->exp_table = create_array_list();
    row = size_of_vocabulary(result->vocabulary);
    result->word_vectors = allocate_2d(row, result->vector_length);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < result->vector_length; j++) {
            result->word_vectors[i][j] = -0.5 + ((double)random()) / RAND_MAX;
        }
    }
    result->word_vector_update = allocate_2d(row, result->vector_length);
    prepare_exp_table(result);
    return result;
}

/**
 * Frees memory allocated for the neural network. Freesword vector update, word vectors, vocabulary, exp_table.
 * @param neural_network Neural network to deallocate.
 */
void free_neural_network(Neural_network_ptr neural_network) {
    int row = size_of_vocabulary(neural_network->vocabulary);
    free_2d(neural_network->word_vector_update, row);
    free_2d(neural_network->word_vectors, row);
    free_vocabulary(neural_network->vocabulary);
    free_array_list(neural_network->exp_table, free_);
    free_(neural_network);
}

/**
 * Constructs the fast exponentiation table. Instead of taking exponent at each time, the algorithm will lookup
 * the table.
 * @param neural_network Current neural network object
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
 * @param neural_network Current neural network object
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
        add_word((Dictionary_ptr) result, (Word_ptr) create_vectorized_word(vocabulary_get_word(neural_network->vocabulary, i)->name, vector));
    }
    sort((Dictionary_ptr) result);
    return result;
}

/**
 * Calculate the update of outputs for word indexed with l2. It also calculates the word vector updates for word
 * indexed at l2.
 * @param neural_network Current neural network object
 * @param outputUpdate Output update to be added.
 * @param outputs Current outputs.
 * @param l2 Index of the input
 * @param g Multiplier for the update.
 */
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

/**
 * Calculates the dot product of two vectors represented as array of doubles.
 * @param neural_network Current neural network object
 * @param vector1 First vector to multiply.
 * @param vector2 Second vector to multiply.
 * @return Dot product of two given vectors.
 */
double dot_product_array(Neural_network_ptr neural_network, const double *vector1, const double *vector2) {
    double sum = 0;
    for (int j = 0; j < neural_network->vector_length; j++){
        sum += vector1[j] * vector2[j];
    }
    return sum;
}

/**
 * Main method for training the CBow version of Word2Vec algorithm.
 * @param neural_network Current neural network object
 */
void train_cbow(Neural_network_ptr neural_network) {
    int word_index, last_word_index;
    Iteration_ptr iteration = create_iteration(neural_network->corpus, neural_network->parameter);
    int target, label, l2, b, cw;
    double f, g;
    corpus_open(neural_network->corpus);
    Sentence_ptr current_sentence = corpus_get_sentence2(neural_network->corpus);
    Vocabulary_word_ptr current_word;
    srandom(neural_network->parameter->seed);
    double* outputs = malloc_(neural_network->vector_length * sizeof(double));
    double* output_update = malloc_(neural_network->vector_length * sizeof(double));
    while (iteration->iteration_count < neural_network->parameter->number_of_iterations) {
        alpha_update(iteration, neural_network->vocabulary->total_number_of_words);
        word_index = get_position(neural_network->vocabulary, array_list_get(current_sentence->words, iteration->sentence_position));
        current_word = vocabulary_get_word(neural_network->vocabulary, word_index);
        for (int i = 0; i < neural_network->vector_length; i++){
            outputs[i] = 0;
            output_update[i] = 0;
        }
        b = random() % neural_network->parameter->window;
        cw = 0;
        for (int a = b; a < neural_network->parameter->window * 2 + 1 - b; a++){
            int c = iteration->sentence_position - neural_network->parameter->window + a;
            if (a != neural_network->parameter->window && sentence_safe_index(current_sentence, c)) {
                last_word_index = get_position(neural_network->vocabulary, array_list_get(current_sentence->words, c));
                for (int j = 0; j < neural_network->vector_length; j++){
                    outputs[j] += neural_network->word_vectors[last_word_index][j];
                }
                cw++;
            }
        }
        if (cw > 0) {
            for (int j = 0; j < neural_network->vector_length; j++){
                outputs[j] /= cw;
            }
            if (neural_network->parameter->hierarchical_soft_max){
                for (int d = 0; d < current_word->code_length; d++) {
                    l2 = current_word->point[d];
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
                    g = (1 - current_word->code[d] - f) * iteration->alpha;
                    update_output(neural_network, output_update, outputs, l2, g);
                }
            } else {
                for (int d = 0; d < neural_network->parameter->negative_sampling_size + 1; d++) {
                    if (d == 0) {
                        target = word_index;
                        label = 1;
                    } else {
                        target = get_table_value(neural_network->vocabulary, random() % neural_network->vocabulary->table->size);
                        if (target == 0)
                            target = random() % (size_of_vocabulary(neural_network->vocabulary) - 1) + 1;
                        if (target == word_index)
                            continue;
                        label = 0;
                    }
                    l2 = target;
                    f = dot_product_array(neural_network, outputs, neural_network->word_vector_update[l2]);
                    g = calculate_g(neural_network, f, iteration->alpha, label);
                    update_output(neural_network, output_update, outputs, l2, g);
                }
            }
            for (int a = b; a < neural_network->parameter->window * 2 + 1 - b; a++){
                int c = iteration->sentence_position - neural_network->parameter->window + a;
                if (a != neural_network->parameter->window && sentence_safe_index(current_sentence, c)) {
                    last_word_index = get_position(neural_network->vocabulary, array_list_get(current_sentence->words, c));
                    for (int j = 0; j < neural_network->vector_length; j++){
                        neural_network->word_vectors[last_word_index][j] += output_update[j];
                    }
                }
            }
        }
        current_sentence = sentence_update(iteration, current_sentence);
    }
    corpus_close(neural_network->corpus);
    free_iteration(iteration);
    free_(outputs);
    free_(output_update);
}

/**
 * Main method for training the SkipGram version of Word2Vec algorithm.
 * @param neural_network Current neural network object
 */
void train_skip_gram(Neural_network_ptr neural_network) {
    int word_index, last_word_index;
    Iteration_ptr iteration = create_iteration(neural_network->corpus, neural_network->parameter);
    int target, label, l1, l2, b;
    double f, g;
    corpus_open(neural_network->corpus);
    Sentence_ptr current_sentence = corpus_get_sentence2(neural_network->corpus);
    Vocabulary_word_ptr current_word;
    srandom(neural_network->parameter->seed);
    double* output_update = malloc_(neural_network->vector_length * sizeof(double));
    while (iteration->iteration_count < neural_network->parameter->number_of_iterations) {
        alpha_update(iteration, neural_network->vocabulary->total_number_of_words);
        word_index = get_position(neural_network->vocabulary, array_list_get(current_sentence->words, iteration->sentence_position));
        current_word = vocabulary_get_word(neural_network->vocabulary, word_index);
        for (int i = 0; i < neural_network->vector_length; i++){
            output_update[i] = 0;
        }
        b = random() % neural_network->parameter->window;
        for (int a = b; a < neural_network->parameter->window * 2 + 1 - b; a++) {
            int c = iteration->sentence_position - neural_network->parameter->window + a;
            if (a != neural_network->parameter->window && sentence_safe_index(current_sentence, c)) {
                last_word_index = get_position(neural_network->vocabulary, array_list_get(current_sentence->words, c));
                l1 = last_word_index;
                for (int i = 0; i < neural_network->vector_length; i++){
                    output_update[i] = 0;
                }
                if (neural_network->parameter->hierarchical_soft_max) {
                    for (int d = 0; d < current_word->code_length; d++) {
                        l2 = current_word->point[d];
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
                        g = (1 - current_word->code[d] - f) * iteration->alpha;
                        update_output(neural_network, output_update, neural_network->word_vectors[l1], l2, g);
                    }
                } else {
                    for (int d = 0; d < neural_network->parameter->negative_sampling_size + 1; d++) {
                        if (d == 0) {
                            target = word_index;
                            label = 1;
                        } else {
                            target = get_table_value(neural_network->vocabulary, random() % neural_network->vocabulary->table->size);
                            if (target == 0)
                                target = random() % (size_of_vocabulary(neural_network->vocabulary) - 1) + 1;
                            if (target == word_index)
                                continue;
                            label = 0;
                        }
                        l2 = target;
                        f = dot_product_array(neural_network, neural_network->word_vectors[l1], neural_network->word_vector_update[l2]);
                        g = calculate_g(neural_network, f, iteration->alpha, label);
                        update_output(neural_network, output_update, neural_network->word_vectors[l1], l2, g);
                    }
                }
                for (int j = 0; j < neural_network->vector_length; j++){
                    neural_network->word_vectors[l1][j] += output_update[j];
                }
            }
        }
        current_sentence = sentence_update(iteration, current_sentence);
    }
    corpus_close(neural_network->corpus);
    free_iteration(iteration);
    free_(output_update);
}

//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#include <Memory/Memory.h>
#include "WordToVecParameter.h"

/**
 * Empty constructor for Word2Vec parameter
 */
Word_to_vec_parameter_ptr create_word_to_vec_parameter() {
    Word_to_vec_parameter_ptr result = malloc_(sizeof(Word_to_vec_parameter), "create_word_to_vec_parameter");
    result->layer_size = 100;
    result->alpha = 0.025;
    result->window = 5;
    result->hierarchical_soft_max = false;
    result->negative_sampling_size = 5;
    result->number_of_iterations = 2;
    result->seed = 1;
    return result;
}

/**
 * Frees memory allocated to word to vec parameter.
 * @param word_to_vec_parameter Word to vec parameter to deallocate.
 */
void free_word_to_vec_parameter(Word_to_vec_parameter_ptr word_to_vec_parameter) {
    free_(word_to_vec_parameter);
}

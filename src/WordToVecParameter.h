//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#ifndef WORDTOVEC_WORDTOVECPARAMETER_H
#define WORDTOVEC_WORDTOVECPARAMETER_H

#include <stdbool.h>

struct word_to_vec_parameter{
    int layer_size;
    bool cbow;
    double alpha;
    int window;
    bool hierarchical_soft_max;
    int negative_sampling_size;
    int number_of_iterations;
    int seed;
};

typedef struct word_to_vec_parameter Word_to_vec_parameter;

typedef Word_to_vec_parameter *Word_to_vec_parameter_ptr;

Word_to_vec_parameter_ptr create_word_to_vec_parameter();

void free_word_to_vec_parameter(Word_to_vec_parameter_ptr word_to_vec_parameter);

#endif //WORDTOVEC_WORDTOVECPARAMETER_H

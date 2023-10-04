//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#ifndef WORDTOVEC_ITERATION_H
#define WORDTOVEC_ITERATION_H

#include <Corpus.h>
#include "WordToVecParameter.h"

struct iteration{
    int word_count;
    int last_word_count;
    int word_count_actual;
    int iteration_count;
    int sentence_position;
    double starting_alpha;
    double alpha;
    Word_to_vec_parameter_ptr parameter;
    Corpus_ptr corpus;
};

typedef struct iteration Iteration;

typedef Iteration *Iteration_ptr;

Iteration_ptr create_iteration(Corpus_ptr corpus, Word_to_vec_parameter_ptr parameter);

void free_iteration(Iteration_ptr iteration);

void alpha_update(Iteration_ptr iteration, int total_number_of_words);

Sentence_ptr sentence_update(Iteration_ptr iteration, Sentence_ptr current_sentence);

#endif //WORDTOVEC_ITERATION_H

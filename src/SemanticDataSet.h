//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#ifndef WORDTOVEC_SEMANTICDATASET_H
#define WORDTOVEC_SEMANTICDATASET_H

#include <ArrayList.h>
#include "Dictionary/VectorizedDictionary.h"
#include "WordPair.h"

struct semantic_data_set{
    Array_list_ptr pairs;
};

typedef struct semantic_data_set Semantic_data_set;

typedef Semantic_data_set *Semantic_data_set_ptr;

Semantic_data_set_ptr create_semantic_data_set(char* file_name);

Semantic_data_set_ptr create_semantic_data_set2();

void free_semantic_data_set(Semantic_data_set_ptr semantic_data_set);

Semantic_data_set_ptr calculate_similarities(Semantic_data_set_ptr semantic_data_set, Vectorized_dictionary_ptr dictionary);

void sort_semantic_data_set(Semantic_data_set_ptr semantic_data_set);

int index_of_word_pair(Semantic_data_set_ptr semantic_data_set, Word_pair_ptr word_pair);

double spearman_correlation(Semantic_data_set_ptr semantic_data_set1, Semantic_data_set_ptr semantic_data_set2);

#endif //WORDTOVEC_SEMANTICDATASET_H

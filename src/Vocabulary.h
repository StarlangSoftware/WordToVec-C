//
// Created by Olcay Taner YILDIZ on 2.10.2023.
//

#ifndef WORDTOVEC_VOCABULARY_H
#define WORDTOVEC_VOCABULARY_H

#include <ArrayList.h>
#include <HashMap/HashMap.h>
#include <Corpus.h>
#include "VocabularyWord.h"

static int MAX_CODE_LENGTH = 40;

struct vocabulary{
    Array_list_ptr vocabulary;
    Array_list_ptr table;
    Hash_map_ptr word_map;
    int total_number_of_words;
};

typedef struct vocabulary Vocabulary;

typedef Vocabulary *Vocabulary_ptr;

Vocabulary_ptr create_vocabulary(Corpus_ptr corpus);

Vocabulary_ptr create_vocabulary2();

void free_vocabulary(Vocabulary_ptr vocabulary);

void create_uni_gram_table(Vocabulary_ptr vocabulary);

void construct_huffman_tree(Vocabulary_ptr vocabulary);

int get_position(Vocabulary_ptr vocabulary, const char* word);

int size_of_vocabulary(Vocabulary_ptr vocabulary);

Vocabulary_word_ptr vocabulary_get_word(Vocabulary_ptr vocabulary, int index);

int get_table_value(Vocabulary_ptr vocabulary, int index);

#endif //WORDTOVEC_VOCABULARY_H

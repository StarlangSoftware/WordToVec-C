//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#ifndef WORDTOVEC_VOCABULARYWORD_H
#define WORDTOVEC_VOCABULARYWORD_H

#include <ArrayList.h>

struct vocabulary_word{
    char* name;
    int count;
    int code_length;
    int code[40];
    int point[40];
};

typedef struct vocabulary_word Vocabulary_word;

typedef Vocabulary_word *Vocabulary_word_ptr;

Vocabulary_word_ptr create_vocabulary_word(const char* name, int count);

void free_vocabulary_word(Vocabulary_word_ptr word);

int compare_vocabulary_word(const Vocabulary_word* word1, const Vocabulary_word* word2);

int compare_vocabulary_word2(const Vocabulary_word* word1, const Vocabulary_word* word2);

#endif //WORDTOVEC_VOCABULARYWORD_H

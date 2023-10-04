//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#ifndef WORDTOVEC_WORDPAIR_H
#define WORDTOVEC_WORDPAIR_H

struct word_pair{
    char* word1;
    char* word2;
    double related_by;
};

typedef struct word_pair Word_pair;

typedef Word_pair *Word_pair_ptr;

Word_pair_ptr create_word_pair(char* word1, char* word2, double related_by);

void free_word_pair(Word_pair_ptr word_pair);

int compare_word_pair(const Word_pair* word_pair1, const Word_pair* word_pair2);

int compare_word_pair2(const Word_pair* word_pair1, const Word_pair* word_pair2);

#endif //WORDTOVEC_WORDPAIR_H

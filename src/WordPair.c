//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#include <stdlib.h>
#include <StringUtils.h>
#include <HashMap/HashMap.h>
#include "WordPair.h"

Word_pair_ptr create_word_pair(char *word1, char *word2, double related_by) {
    Word_pair_ptr result = malloc(sizeof(Word_pair));
    result->word1 = str_copy(result->word1, word1);
    result->word2 = str_copy(result->word2, word2);
    result->related_by = related_by;
    return result;
}

void free_word_pair(Word_pair_ptr word_pair) {
    free(word_pair->word1);
    free(word_pair->word2);
    free(word_pair);
}

int compare_word_pair(const Word_pair *word_pair1, const Word_pair *word_pair2) {
    if (compare_string(word_pair1->word1, word_pair2->word1) != 0){
        return compare_string(word_pair1->word1, word_pair2->word1);
    } else {
        return compare_string(word_pair1->word2, word_pair2->word2);
    }
}

int compare_word_pair2(const Word_pair *word_pair1, const Word_pair *word_pair2) {
    return compare_double_r(&(word_pair1->related_by), &(word_pair2->related_by));
}

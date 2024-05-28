//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#include <StringUtils.h>
#include <HashMap/HashMap.h>
#include <Memory/Memory.h>
#include "WordPair.h"

/**
 * Constructor of the WordPair object. WordPair stores the information about two words and their similarity scores.
 * @param word1 First word
 * @param word2 Second word
 * @param related_by Similarity score between first and second word.
 */
Word_pair_ptr create_word_pair(char *word1, char *word2, double related_by) {
    Word_pair_ptr result = malloc_(sizeof(Word_pair), "create_word_pair");
    result->word1 = str_copy(result->word1, word1);
    result->word2 = str_copy(result->word2, word2);
    result->related_by = related_by;
    return result;
}

/**
 * Frees memory allocated for word pair.
 * @param word_pair Word pair to deallocate.
 */
void free_word_pair(Word_pair_ptr word_pair) {
    free_(word_pair->word1);
    free_(word_pair->word2);
    free_(word_pair);
}

/**
 * Comparator function for word pair object.
 * @param word_pair1 First word pair to compare
 * @param word_pair2 Second word pair to compare
 * @return Comparison result according to the first word of the word pairs, if they are equal
 * comparison is done based on the second word of the word pairs.
 */
int compare_word_pair(const Word_pair *word_pair1, const Word_pair *word_pair2) {
    if (compare_string(word_pair1->word1, word_pair2->word1) != 0){
        return compare_string(word_pair1->word1, word_pair2->word1);
    } else {
        return compare_string(word_pair1->word2, word_pair2->word2);
    }
}

/**
 * Comparator function for word pair object.
 * @param word_pair1 First word pair to compare
 * @param word_pair2 Second word pair to compare
 * @return Comparison result according to the similarity score of the word pairs.
 */
int compare_word_pair2(const Word_pair *word_pair1, const Word_pair *word_pair2) {
    return compare_double_r(&(word_pair1->related_by), &(word_pair2->related_by));
}

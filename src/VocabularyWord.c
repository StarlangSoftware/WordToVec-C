//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#include <StringUtils.h>
#include <HashMap/HashMap.h>
#include "VocabularyWord.h"
#include "Memory/Memory.h"

/**
 * Constructor for a VocabularyWord. The constructor gets name and count values and sets the corresponding
 * attributes. It also initializes the code and point arrays for this word.
 * @param name Lemma of the word
 * @param count Number of occurrences of this word in the corpus
 */
Vocabulary_word_ptr create_vocabulary_word(const char *name, int count) {
    Vocabulary_word_ptr result = malloc_(sizeof(Vocabulary_word), "create_vocabulary_word");
    result->name = str_copy(result->name, name);
    result->count = count;
    result->code_length = 0;
    return result;
}

int compare_vocabulary_word(const Vocabulary_word *word1, const Vocabulary_word *word2) {
    return compare_string(word1->name, word2->name);
}

int compare_vocabulary_word2(const Vocabulary_word *word1, const Vocabulary_word *word2) {
    return compare_int_r(&(word1->count), &(word2->count));
}

/**
 * Frees memory allocated for vocabulary word.
 * @param word Vocabulary word to deallocate.
 */
void free_vocabulary_word(Vocabulary_word_ptr word) {
    free_(word->name);
    free_(word);
}

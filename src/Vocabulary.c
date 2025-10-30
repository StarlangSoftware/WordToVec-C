//
// Created by Olcay Taner YILDIZ on 2.10.2023.
//

#include <math.h>
#include <Memory/Memory.h>
#include "Vocabulary.h"
#include "VocabularyWord.h"

/**
 * Constructor for the Vocabulary class. For each distinct word in the corpus, a VocabularyWord
 * instance is created. After that, words are sorted according to their occurences. Unigram table is constructed,
 * whereafter Huffman tree is created based on the number of occurrences of the words.
 * @param corpus Corpus used to train word vectors using Word2Vec algorithm.
 */
Vocabulary_ptr create_vocabulary(Corpus_ptr corpus) {
    Vocabulary_ptr result = create_vocabulary2();
    Counter_hash_map_ptr counts = create_counter_hash_map((unsigned int (*)(const void *, int)) hash_function_string,
                                                          (int (*)(const void *, const void *)) compare_string);
    corpus_open(corpus);
    Sentence_ptr sentence = corpus_get_sentence2(corpus);
    while (sentence != NULL){
        for (int i = 0; i < sentence_word_count(sentence); i++){
            put_counter_hash_map(counts, sentence_get_word(sentence, i));
        }
        result->total_number_of_words += sentence_word_count(sentence);
        sentence = corpus_get_sentence2(corpus);
    }
    Array_list_ptr list = key_value_list(counts->map);
    for (int i = 0; i < list->size; i++){
        Hash_node_ptr node = array_list_get(list, i);
        array_list_add(result->vocabulary, create_vocabulary_word(node->key, *(int*)node->value));
    }
    free_array_list(list, NULL);
    array_list_sort(result->vocabulary, (int (*)(const void *, const void *)) compare_vocabulary_word2);
    create_uni_gram_table(result);
    construct_huffman_tree(result);
    array_list_sort(result->vocabulary, (int (*)(const void *, const void *)) compare_vocabulary_word);
    for (int i = 0; i < result->vocabulary->size; i++){
        int* index = malloc_(sizeof(int));
        *index = i;
        hash_map_insert(result->word_map, ((Vocabulary_word_ptr)array_list_get(result->vocabulary, i))->name, index);
    }
    free_counter_hash_map(counts);
    return result;
}

/**
 * Empty constructor for vocabulary. Allocates empty vocabulary, table and word map.
 * @return An empty allocated vocabulary.
 */
Vocabulary_ptr create_vocabulary2() {
    Vocabulary_ptr result = malloc_(sizeof(Vocabulary));
    result->vocabulary = create_array_list();
    result->table = create_array_list();
    result->word_map = create_string_hash_map();
    result->total_number_of_words = 0;
    return result;
}

/**
 * Searches a word and returns the position of that word in the vocabulary. Search is done using binary search.
 * @param vocabulary Current vocabulary object
 * @param word Word to be searched.
 * @return Position of the word searched.
 */
int get_position(Vocabulary_ptr vocabulary, const char* word) {
    int* position = hash_map_get(vocabulary->word_map, word);
    return *position;
}

/**
 * Returns number of words in the vocabulary.
 * @param vocabulary Current vocabulary object
 * @return Number of words in the vocabulary.
 */
int size_of_vocabulary(Vocabulary_ptr vocabulary) {
    return vocabulary->vocabulary->size;
}

/**
 * Returns the word at a given index.
 * @param vocabulary Current vocabulary object
 * @param index Index of the word.
 * @return The word at a given index.
 */
Vocabulary_word_ptr vocabulary_get_word(Vocabulary_ptr vocabulary, int index) {
    return array_list_get(vocabulary->vocabulary, index);
}

/**
 * Accessor for the unigram table.
 * @param vocabulary Current vocabulary object
 * @param index Index of the word.
 * @return Unigram table value at a given index.
 */
int get_table_value(Vocabulary_ptr vocabulary, int index) {
    return array_list_get_int(vocabulary->table, index);
}

/**
 * Constructs the unigram table based on the number of occurrences of the words.
 * @param vocabulary Current vocabulary object
 */
void create_uni_gram_table(Vocabulary_ptr vocabulary) {
    int i;
    double total = 0;
    double d1;
    Vocabulary_word_ptr word;
    for (i = 0; i < vocabulary->vocabulary->size; i++) {
        word = array_list_get(vocabulary->vocabulary, i);
        total += pow(word->count, 0.75);
    }
    i = 0;
    word = array_list_get(vocabulary->vocabulary, i);
    d1 = pow(word->count, 0.75) / total;
    for (int a = 0; a < 2 * vocabulary->vocabulary->size; a++) {
        array_list_add_int(vocabulary->table, i);
        if (a / (2 * vocabulary->vocabulary->size + 0.0) > d1) {
            i++;
            word = array_list_get(vocabulary->vocabulary, i);
            d1 += pow(word->count, 0.75) / total;
        }
        if (i >= vocabulary->vocabulary->size){
            i = vocabulary->vocabulary->size - 1;
        }
    }
}

/**
 * Constructs Huffman Tree based on the number of occurrences of the words.
 * @param vocabulary Current vocabulary object
 */
void construct_huffman_tree(Vocabulary_ptr vocabulary) {
    int min1i, min2i, b, i, size = vocabulary->vocabulary->size;
    int count[size * 2 + 1];
    int code[MAX_CODE_LENGTH];
    int point[MAX_CODE_LENGTH];
    int binary[size * 2 + 1];
    int parentNode[size * 2 + 1];
    Vocabulary_word_ptr word;
    for (int a = 0; a < size; a++){
        word = array_list_get(vocabulary->vocabulary, a);
        count[a] = word->count;
    }
    for (int a = size; a < size * 2; a++)
        count[a] = 1000000000;
    int pos1 = size - 1;
    int pos2 = size;
    for (int a = 0; a < size - 1; a++) {
        if (pos1 >= 0) {
            if (count[pos1] < count[pos2]) {
                min1i = pos1;
                pos1--;
            } else {
                min1i = pos2;
                pos2++;
            }
        } else {
            min1i = pos2;
            pos2++;
        }
        if (pos1 >= 0) {
            if (count[pos1] < count[pos2]) {
                min2i = pos1;
                pos1--;
            } else {
                min2i = pos2;
                pos2++;
            }
        } else {
            min2i = pos2;
            pos2++;
        }
        count[size + a] = count[min1i] + count[min2i];
        parentNode[min1i] = size + a;
        parentNode[min2i] = size + a;
        binary[min2i] = 1;
    }
    for (int a = 0; a < size; a++) {
        b = a;
        i = 0;
        while (true) {
            code[i] = binary[b];
            point[i] = b;
            i++;
            b = parentNode[b];
            if (b == size * 2 - 2)
                break;
        }
        word = array_list_get(vocabulary->vocabulary, a);
        word->code_length = i;
        word->point[0] = size - 2;
        for (b = 0; b < i; b++) {
            word->code[i - b - 1] = code[b];
            word->point[i - b] = point[b] - size;
        }
    }
}

/**
 * Frees memory allocated for the vocabulary. Frees vocabulary and table array lists and word_map hash map.
 * @param vocabulary Vocabulary to deallocate.
 */
void free_vocabulary(Vocabulary_ptr vocabulary) {
    free_array_list(vocabulary->vocabulary, (void (*)(void *)) free_vocabulary_word);
    free_array_list(vocabulary->table, free_);
    free_hash_map2(vocabulary->word_map, NULL, free_);
    free_(vocabulary);
}

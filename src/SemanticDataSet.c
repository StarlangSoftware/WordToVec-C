//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#include <stdlib.h>
#include <FileUtils.h>
#include <StringUtils.h>
#include <Memory/Memory.h>
#include "SemanticDataSet.h"
#include "WordPair.h"

/**
 * Constructor for the semantic dataset. Reads word pairs and their similarity scores from an input file.
 * @param file_name Input file that stores the word pair and similarity scores.
 * @return Semantic dataset read from an input file
 */
Semantic_data_set_ptr create_semantic_data_set(const char *file_name) {
    Semantic_data_set_ptr result = create_semantic_data_set2();
    Array_list_ptr lines = read_lines(file_name);
    for (int i = 0; i < lines->size; i++){
        char* line = array_list_get(lines, i);
        Array_list_ptr items = str_split(line, ' ');
        array_list_add(result->pairs, create_word_pair(array_list_get(items, 0), array_list_get(items, 1), atof(array_list_get(items, 2))));
        free_array_list(items, free_);
    }
    free_array_list(lines, free_);
    return result;
}

/**
 * Empty constructor for the semantic dataset.
 * @return Empty semantic dataset
 */
Semantic_data_set_ptr create_semantic_data_set2() {
    Semantic_data_set_ptr result = malloc_(sizeof(Semantic_data_set), "create_semantic_data_set2");
    result->pairs = create_array_list();
    return result;
}

/**
 * Frees memory allocated for a semantic dataset. Frees pairs array list.
 * @param semantic_data_set Semantic dataset to deallocate
 */
void free_semantic_data_set(Semantic_data_set_ptr semantic_data_set) {
    free_array_list(semantic_data_set->pairs, (void (*)(void *)) free_word_pair);
    free_(semantic_data_set);
}

/**
 * Calculates the similarities between words in the dataset. The word vectors will be taken from the input
 * vectorized dictionary.
 * @param semantic_data_set Semantic dataset
 * @param dictionary Vectorized dictionary that stores the word vectors.
 * @return Word pairs and their calculated similarities stored as a semantic dataset.
 */
Semantic_data_set_ptr
calculate_similarities(Semantic_data_set_ptr semantic_data_set, Vectorized_dictionary_ptr dictionary) {
    Semantic_data_set_ptr result = create_semantic_data_set2();
    double similarity;
    for (int i = 0; i < semantic_data_set->pairs->size; i++){
        char* word1 = ((Word_pair_ptr) array_list_get(semantic_data_set->pairs, i))->word1;
        char* word2 = ((Word_pair_ptr) array_list_get(semantic_data_set->pairs, i))->word2;
        Vectorized_word_ptr vectorized_word_1 = get_word2(dictionary, word1);
        Vectorized_word_ptr vectorized_word_2 = get_word2(dictionary, word2);
        if (vectorized_word_1 != NULL && vectorized_word_2 != NULL){
            similarity = cosine_similarity(vectorized_word_1->vector, vectorized_word_2->vector);
            array_list_add(result->pairs, create_word_pair(word1, word2, similarity));
        } else {
            array_list_remove(semantic_data_set->pairs, i, (void (*)(void *)) free_word_pair);
            i--;
        }
    }
    return result;
}

/**
 * Sorts the word pairs in the dataset according to the WordPairComparator.
 * @param semantic_data_set Semantic dataset
 */
void sort_semantic_data_set(Semantic_data_set_ptr semantic_data_set) {
    array_list_sort(semantic_data_set->pairs, (int (*)(const void *, const void *)) compare_word_pair2);
}

/**
 * Finds and returns the index of a word pair in the pairs array list. If there is no such word pair, it
 * returns -1.
 * @param semantic_data_set Semantic dataset
 * @param word_pair Word pair to search in the semantic dataset.
 * @return Index of the given word pair in the pairs array list. If it does not exist, the method returns -1.
 */
int index_of_word_pair(Semantic_data_set_ptr semantic_data_set, Word_pair_ptr word_pair) {
    for (int i = 0; i < semantic_data_set->pairs->size; i++){
        Word_pair_ptr current = array_list_get(semantic_data_set->pairs, i);
        if (compare_word_pair(current, word_pair) == 0){
            return i;
        }
    }
    return -1;
}

/**
 * Calculates the Spearman correlation coefficient between two given semantic datasets.
 * @param semantic_data_set1 First semantic dataset with which Spearman correlation coefficient is calculated.
 * @param semantic_data_set2 Second semantic dataset with which Spearman correlation coefficient is calculated.
 * @return Spearman correlation coefficient between two given semantic datasets.
 */
double spearman_correlation(Semantic_data_set_ptr semantic_data_set1, Semantic_data_set_ptr semantic_data_set2) {
    double sum = 0;
    int rank1, rank2 = 0;
    sort_semantic_data_set(semantic_data_set1);
    sort_semantic_data_set(semantic_data_set2);
    for (int i = 0; i < semantic_data_set1->pairs->size; i++){
        rank1 = i + 1;
        int index = index_of_word_pair(semantic_data_set2, array_list_get(semantic_data_set1->pairs, i));
        if (index != -1){
            rank2 = index + 1;
        }
        double di = rank1 - rank2;
        sum += 6 * di * di;
    }
    double n = semantic_data_set1->pairs->size;
    double ratio = sum / (n * (n * n - 1));
    return 1 - ratio;
}

//
// Created by Olcay Taner YILDIZ on 1.10.2023.
//

#include <stdlib.h>
#include <Memory/Memory.h>
#include "Iteration.h"

/**
 * Constructor for the Iteration class. Get corpus and parameter as input, sets the corresponding
 * parameters.
 * @param corpus Corpus used to train word vectors using Word2Vec algorithm.
 * @param wordToVecParameter Parameters of the Word2Vec algorithm.
 */
Iteration_ptr create_iteration(Corpus_ptr corpus, Word_to_vec_parameter_ptr parameter) {
    Iteration_ptr result = malloc_(sizeof(Iteration), "create_iteration");
    result->word_count = 0;
    result->last_word_count = 0;
    result->word_count_actual = 0;
    result->iteration_count = 0;
    result->sentence_position = 0;
    result->corpus = corpus;
    result->parameter = parameter;
    result->starting_alpha = parameter->alpha;
    result->alpha = parameter->alpha;
    return result;
}

void free_iteration(Iteration_ptr iteration) {
    free_(iteration);
}

/**
 * Updates the alpha parameter after 10000 words has been processed.
 */
void alpha_update(Iteration_ptr iteration, int total_number_of_words) {
    if (iteration->word_count - iteration->last_word_count > 10000) {
        iteration->word_count_actual += iteration->word_count - iteration->last_word_count;
        iteration->last_word_count = iteration->word_count;
        iteration->alpha = iteration->starting_alpha * (1 - iteration->word_count_actual / (iteration->parameter->number_of_iterations * total_number_of_words + 1.0));
        if (iteration->alpha < iteration->starting_alpha * 0.0001)
            iteration->alpha = iteration->starting_alpha * 0.0001;
    }
}

/**
 * Updates sentencePosition, sentenceIndex (if needed) and returns the current sentence processed. If one sentence
 * is finished, the position shows the beginning of the next sentence and sentenceIndex is incremented. If the
 * current sentence is the last sentence, the system shuffles the sentences and returns the first sentence.
 * @param currentSentence Current sentence processed.
 * @return If current sentence is not changed, currentSentence; if changed the next sentence; if next sentence is
 * the last sentence; shuffles the corpus and returns the first sentence.
 */
Sentence_ptr sentence_update(Iteration_ptr iteration, Sentence_ptr current_sentence) {
    iteration->sentence_position++;
    if (iteration->sentence_position >= sentence_word_count(current_sentence)) {
        iteration->word_count += sentence_word_count(current_sentence);
        iteration->sentence_position = 0;
        Sentence* sentence = corpus_get_sentence2(iteration->corpus);
        if (sentence == NULL){
            iteration->iteration_count++;
            iteration->word_count = 0;
            iteration->last_word_count = 0;
            corpus_close(iteration->corpus);
            corpus_open(iteration->corpus);
            sentence = corpus_get_sentence2(iteration->corpus);
        }
        return sentence;
    }
    return current_sentence;
}

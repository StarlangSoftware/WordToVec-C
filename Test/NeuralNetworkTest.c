//
// Created by Olcay Taner YILDIZ on 4.10.2023.
//

#include <Corpus.h>
#include <Memory/Memory.h>

#include "../src/SemanticDataSet.h"
#include "../src/WordToVecParameter.h"
#include "../src/NeuralNetwork.h"

void test_train_english_cbow(){
    Semantic_data_set_ptr mc, rg, ws, men, mturk, rare;
    mc = create_semantic_data_set("MC.txt");
    rg = create_semantic_data_set("RG.txt");
    ws = create_semantic_data_set("WS353.txt");
    men = create_semantic_data_set("MEN.txt");
    mturk = create_semantic_data_set("MTurk771.txt");
    rare = create_semantic_data_set("RareWords.txt");
    Corpus_ptr english = create_corpus2("english-xs.txt");
    Word_to_vec_parameter_ptr parameter = create_word_to_vec_parameter();
    parameter->cbow = true;
    Neural_network_ptr neural_network = create_neural_network(english, parameter);
    Vectorized_dictionary_ptr dictionary = train(neural_network);
    free_word_to_vec_parameter(parameter);
    Semantic_data_set_ptr mc2 = calculate_similarities(mc, dictionary);
    printf("%.6lf\n", spearman_correlation(mc, mc2));
    free_semantic_data_set(mc);
    free_semantic_data_set(mc2);
    Semantic_data_set_ptr rg2 = calculate_similarities(rg, dictionary);
    printf("%.6lf\n", spearman_correlation(rg, rg2));
    free_semantic_data_set(rg);
    free_semantic_data_set(rg2);
    Semantic_data_set_ptr ws2 = calculate_similarities(ws, dictionary);
    printf("%.6lf\n", spearman_correlation(ws, ws2));
    free_semantic_data_set(ws);
    free_semantic_data_set(ws2);
    Semantic_data_set_ptr men2 = calculate_similarities(men, dictionary);
    printf("%.6lf\n", spearman_correlation(men, men2));
    free_semantic_data_set(men);
    free_semantic_data_set(men2);
    Semantic_data_set_ptr mturk2 = calculate_similarities(mturk, dictionary);
    printf("%.6lf\n", spearman_correlation(mturk, mturk2));
    free_semantic_data_set(mturk);
    free_semantic_data_set(mturk2);
    Semantic_data_set_ptr rare2 = calculate_similarities(rare, dictionary);
    printf("%.6lf\n", spearman_correlation(rare, rare2));
    free_semantic_data_set(rare);
    free_semantic_data_set(rare2);
    free_vectorized_dictionary(dictionary);
    free_neural_network(neural_network);
    free_corpus(english);
}

void test_train_english_skip_gram(){
    Semantic_data_set_ptr mc, rg, ws, men, mturk, rare;
    mc = create_semantic_data_set("MC.txt");
    rg = create_semantic_data_set("RG.txt");
    ws = create_semantic_data_set("WS353.txt");
    men = create_semantic_data_set("MEN.txt");
    mturk = create_semantic_data_set("MTurk771.txt");
    rare = create_semantic_data_set("RareWords.txt");
    Corpus_ptr english = create_corpus2("english-xs.txt");
    Word_to_vec_parameter_ptr parameter = create_word_to_vec_parameter();
    parameter->cbow = false;
    Neural_network_ptr neural_network = create_neural_network(english, parameter);
    Vectorized_dictionary_ptr dictionary = train(neural_network);
    free_word_to_vec_parameter(parameter);
    Semantic_data_set_ptr mc2 = calculate_similarities(mc, dictionary);
    printf("%.6lf\n", spearman_correlation(mc, mc2));
    free_semantic_data_set(mc);
    free_semantic_data_set(mc2);
    Semantic_data_set_ptr rg2 = calculate_similarities(rg, dictionary);
    printf("%.6lf\n", spearman_correlation(rg, rg2));
    free_semantic_data_set(rg);
    free_semantic_data_set(rg2);
    Semantic_data_set_ptr ws2 = calculate_similarities(ws, dictionary);
    printf("%.6lf\n", spearman_correlation(ws, ws2));
    free_semantic_data_set(ws);
    free_semantic_data_set(ws2);
    Semantic_data_set_ptr men2 = calculate_similarities(men, dictionary);
    printf("%.6lf\n", spearman_correlation(men, men2));
    free_semantic_data_set(men);
    free_semantic_data_set(men2);
    Semantic_data_set_ptr mturk2 = calculate_similarities(mturk, dictionary);
    printf("%.6lf\n", spearman_correlation(mturk, mturk2));
    free_semantic_data_set(mturk);
    free_semantic_data_set(mturk2);
    Semantic_data_set_ptr rare2 = calculate_similarities(rare, dictionary);
    printf("%.6lf\n", spearman_correlation(rare, rare2));
    free_semantic_data_set(rare);
    free_semantic_data_set(rare2);
    free_vectorized_dictionary(dictionary);
    free_neural_network(neural_network);
    free_corpus(english);
}

void test_with_word_vectors(){
    Semantic_data_set_ptr mc, rg, ws, men, mturk, rare;
    mc = create_semantic_data_set("MC.txt");
    rg = create_semantic_data_set("RG.txt");
    ws = create_semantic_data_set("WS353.txt");
    men = create_semantic_data_set("MEN.txt");
    mturk = create_semantic_data_set("MTurk771.txt");
    rare = create_semantic_data_set("RareWords.txt");
    Vectorized_dictionary_ptr dictionary = create_vectorized_dictionary2("vectors-english-xs.txt");
    printf("%d\n", dictionary->dictionary->words->size);
    Semantic_data_set_ptr mc2 = calculate_similarities(mc, dictionary);
    printf("%.6lf\n", spearman_correlation(mc, mc2));
    free_semantic_data_set(mc);
    free_semantic_data_set(mc2);
    Semantic_data_set_ptr rg2 = calculate_similarities(rg, dictionary);
    printf("%.6lf\n", spearman_correlation(rg, rg2));
    free_semantic_data_set(rg);
    free_semantic_data_set(rg2);
    Semantic_data_set_ptr ws2 = calculate_similarities(ws, dictionary);
    printf("%.6lf\n", spearman_correlation(ws, ws2));
    free_semantic_data_set(ws);
    free_semantic_data_set(ws2);
    Semantic_data_set_ptr men2 = calculate_similarities(men, dictionary);
    printf("%.6lf\n", spearman_correlation(men, men2));
    free_semantic_data_set(men);
    free_semantic_data_set(men2);
    Semantic_data_set_ptr mturk2 = calculate_similarities(mturk, dictionary);
    printf("%.6lf\n", spearman_correlation(mturk, mturk2));
    free_semantic_data_set(mturk);
    free_semantic_data_set(mturk2);
    Semantic_data_set_ptr rare2 = calculate_similarities(rare, dictionary);
    printf("%.6lf\n", spearman_correlation(rare, rare2));
    free_semantic_data_set(rare);
    free_semantic_data_set(rare2);
    free_vectorized_dictionary(dictionary);
}

int main(){
    start_large_memory_check();
    test_train_english_cbow();
    end_memory_check();
}
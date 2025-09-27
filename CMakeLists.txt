cmake_minimum_required(VERSION 3.23)
project(WordToVec)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

find_package(corpus_c REQUIRED)

add_library(WordToVec src/WordToVecParameter.c src/WordToVecParameter.h src/Iteration.c src/Iteration.h src/WordPair.c src/WordPair.h src/SemanticDataSet.c src/SemanticDataSet.h src/VocabularyWord.c src/VocabularyWord.h src/Vocabulary.c src/Vocabulary.h src/NeuralNetwork.c src/NeuralNetwork.h)
target_link_libraries(WordToVec corpus_c::corpus_c m)
add_executable(SemanticDataSetTest src/WordToVecParameter.c src/WordToVecParameter.h src/Iteration.c src/Iteration.h src/WordPair.c src/WordPair.h src/SemanticDataSet.c src/SemanticDataSet.h src/VocabularyWord.c src/VocabularyWord.h src/Vocabulary.c src/Vocabulary.h src/NeuralNetwork.c src/NeuralNetwork.h Test/SemanticDataSetTest.c)
target_link_libraries(SemanticDataSetTest corpus_c::corpus_c m)
add_executable(NeuralNetworkTest src/WordToVecParameter.c src/WordToVecParameter.h src/Iteration.c src/Iteration.h src/WordPair.c src/WordPair.h src/SemanticDataSet.c src/SemanticDataSet.h src/VocabularyWord.c src/VocabularyWord.h src/Vocabulary.c src/Vocabulary.h src/NeuralNetwork.c src/NeuralNetwork.h Test/NeuralNetworkTest.c)
target_link_libraries(NeuralNetworkTest corpus_c::corpus_c m)

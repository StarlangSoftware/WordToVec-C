cmake_minimum_required(VERSION 3.23)
project(WordToVec)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

find_package(corpus_c REQUIRED)

add_library(WordToVec WordToVecParameter.c WordToVecParameter.h Iteration.c Iteration.h WordPair.c WordPair.h SemanticDataSet.c SemanticDataSet.h VocabularyWord.c VocabularyWord.h Vocabulary.c Vocabulary.h NeuralNetwork.c NeuralNetwork.h)
target_link_libraries(WordToVec corpus_c::corpus_c)


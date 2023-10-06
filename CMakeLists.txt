cmake_minimum_required(VERSION 3.23)
project(WordToVec C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(WordToVec src/WordToVecParameter.c src/WordToVecParameter.h src/Iteration.c src/Iteration.h src/WordPair.c src/WordPair.h src/SemanticDataSet.c src/SemanticDataSet.h src/VocabularyWord.c src/VocabularyWord.h src/Vocabulary.c src/Vocabulary.h src/NeuralNetwork.c src/NeuralNetwork.h)
target_link_libraries(WordToVec ${CONAN_LIBS})
add_executable(SemanticDataSetTest src/WordToVecParameter.c src/WordToVecParameter.h src/Iteration.c src/Iteration.h src/WordPair.c src/WordPair.h src/SemanticDataSet.c src/SemanticDataSet.h src/VocabularyWord.c src/VocabularyWord.h src/Vocabulary.c src/Vocabulary.h src/NeuralNetwork.c src/NeuralNetwork.h Test/SemanticDataSetTest.c)
target_link_libraries(SemanticDataSetTest ${CONAN_LIBS})
add_executable(NeuralNetworkTest src/WordToVecParameter.c src/WordToVecParameter.h src/Iteration.c src/Iteration.h src/WordPair.c src/WordPair.h src/SemanticDataSet.c src/SemanticDataSet.h src/VocabularyWord.c src/VocabularyWord.h src/Vocabulary.c src/Vocabulary.h src/NeuralNetwork.c src/NeuralNetwork.h Test/NeuralNetworkTest.c)
target_link_libraries(NeuralNetworkTest ${CONAN_LIBS})

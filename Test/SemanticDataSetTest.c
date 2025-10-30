//
// Created by Olcay Taner YILDIZ on 4.10.2023.
//

#include <Memory/Memory.h>

#include "../src/SemanticDataSet.h"

int main(){
    start_medium_memory_check();
    Semantic_data_set_ptr semanticDataSet = create_semantic_data_set("MC.txt");
    if (spearman_correlation(semanticDataSet, semanticDataSet) != 1.0){
        printf("Error 1 %.6lf\n", spearman_correlation(semanticDataSet, semanticDataSet));
    }
    free_semantic_data_set(semanticDataSet);
    semanticDataSet = create_semantic_data_set("AnlamverRel.txt");
    if (spearman_correlation(semanticDataSet, semanticDataSet) != 1.0){
        printf("Error 2 %.6lf\n", spearman_correlation(semanticDataSet, semanticDataSet));
    }
    free_semantic_data_set(semanticDataSet);
    semanticDataSet = create_semantic_data_set("MEN.txt");
    if (spearman_correlation(semanticDataSet, semanticDataSet) != 1.0){
        printf("Error 3 %.6lf\n", spearman_correlation(semanticDataSet, semanticDataSet));
    }
    free_semantic_data_set(semanticDataSet);
    semanticDataSet = create_semantic_data_set("MTurk771.txt");
    if (spearman_correlation(semanticDataSet, semanticDataSet) != 1.0){
        printf("Error 4 %.6lf\n", spearman_correlation(semanticDataSet, semanticDataSet));
    }
    free_semantic_data_set(semanticDataSet);
    semanticDataSet = create_semantic_data_set("RareWords.txt");
    if (spearman_correlation(semanticDataSet, semanticDataSet) != 1.0){
        printf("Error 5 %.6lf\n", spearman_correlation(semanticDataSet, semanticDataSet));
    }
    free_semantic_data_set(semanticDataSet);
    semanticDataSet = create_semantic_data_set("RG.txt");
    if (spearman_correlation(semanticDataSet, semanticDataSet) != 1.0){
        printf("Error 6 %.6lf\n", spearman_correlation(semanticDataSet, semanticDataSet));
    }
    free_semantic_data_set(semanticDataSet);
    end_memory_check();
}
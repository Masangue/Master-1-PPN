#include "evaluation.h" 
#include <stdio.h>

void init_score( Score * score ){
    score->f1 = 0;
    score->precision = 0;
    score->recall = 0;
    score->accuracy = 0;

    score->specificity = 0;     // false positive rate

    score->true_positive  = 0;
    score->false_positive = 0;
    score->true_negative  = 0;
    score->false_negative = 0;

}

void update_score( Layer * output_layer, f64 * expected, Score * score ) {
    u64 size = output_layer->size;
    f64 err = 0;
    for( u64 i = 0; i < size; i++ ){
            err += sqrt(pow(expected[i] - output_layer->neurons[i],2));
    }

    err = err / size;
    
    // only work for size = 1
    // will be update later

    // expected 0 = négatif 
    // expected 1 = positif
    if(expected[0] > 0.5 ){
        if( err < 0.5 )
            score->true_positive++;
        else
            score->false_negative++;
        }
    else{
        if( err < 0.5)
            score->true_negative++;
        else
            score->false_positive++;            
        }

}


void process_score( Score * score ) {
    int tp = score->true_positive;
    int tn = score->true_negative;
    int fp = score->false_positive;
    int fn = score->false_negative;
    double recall = tp / (double)( tp + fn ); // also sensitivity, also true positive rate
    double specificity = tn / (double) ( tn + fp );
    double precision =  tp / (double)( tp + fp ); 
    double accuracy = ( tp + tn ) / (double)( tp + tn + fp + fn );
    double false_positive_rate = fp / (double) ( fp + tn );

    if( tp + fn == 0)
        recall = 0;
    if( tn + tp == 0)
        specificity = 0;
    if( tp + fp == 0)
        precision = 0;

    double f1 = 2 * (recall * precision) / ( recall + precision );  

    if( recall + precision == 0)
        f1 = 0;
    
    // printf(" -> %lf, %lf, %lf, %lf\n", recall, specificity, precision,  accuracy );
    printf(" -> tp : %d, tn : %d, fp : %d, fn : %d\n", tp, tn, fp, fn );


    // double f1 = 
    score->recall = recall;
    score->precision = precision;
    score->accuracy = accuracy;
    score->specificity = false_positive_rate;
    score->f1 = f1;
}

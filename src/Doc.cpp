/*
 * Doc.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: roseyu
 */

#include "Doc.h"
using namespace std;

Doc::Doc() {
	// TODO Auto-generated constructor stub

}

Doc::~Doc() {
	// TODO Auto-generated destructor stub
}

void Doc::Add_pair(int wordID, int count){

	pair <int, int> word_count (wordID-1, count); // index should be minus one
	this->bag_of_words.push_back(word_count);
	this->num_word += count;

}
int Doc::Gen_random_topic2(double prob[], int N, int K){
		 double prob_acc [K];


		  double u = rand()/((double)RAND_MAX);

		  prob_acc[0]=prob[0];
		  for (int k = 1; k < K; k++){
		  			prob_acc[k]=prob[k]+prob_acc[k-1];
		  }


		  for (int n = 0; n<N; n++ ){
			  for (int k = 0; k < K; k++){
			    if (u<=prob_acc[k] ){
			    	return k;
				  }
			  }
		  }
		  return -1;

}
int Doc::Gen_random_topic(double prob[], int N, int num_topic){

		int result = 0;

		double prob_in[num_topic];
		for(int k = 0; k < num_topic; k++){
				prob_in[k]= prob[k];
		}

		const gsl_rng_type * T;
		gsl_rng * r;
		gsl_rng_env_setup();
		T = gsl_rng_default;
		r = gsl_rng_alloc (T);
		unsigned int rand_topic[num_topic];


		gsl_ran_multinomial (r, num_topic, N, prob_in,rand_topic);

		for (int k = 0; k < num_topic; k++){
		   if (rand_topic[k] ==1){
			   return k;
		   }

		}

		return 0;
}

int Doc::Sample_topic (vector<int> doc_topic_count, vector<int> term_topic_count, vector <int> topic_count,int alpha, int beta){  //Sample_topic (int doc_topic_count[][NUM_TOPIC] ,int term_topic_count[][NUM_TOPIC], int topic_count[],int wordID){
		   double sum_prob=0;

		   int num_topic = doc_topic_count.size();

		   double prob[num_topic];


		   for ( int k = 0; k < num_topic ; k++){
			   prob[k] = (alpha +(double)doc_topic_count[k])*(beta + (double)term_topic_count[k]);
			   prob[k]  = prob[k] /(this->num_term * beta+ (double)topic_count[k]);
			   sum_prob += prob[k];
		   }

		   if(sum_prob <0 ){
     	   cout << "sum_prob " << sum_prob << endl;
		   }
			   // normalize
		   for ( int k = 0; k < num_topic ; k++){
			   prob[k] =  prob[k] / sum_prob;
		   }

		   return this->Gen_random_topic2(prob,1, num_topic);
}

void Doc::Init_random_topic(int num_topic){
		 double prob[num_topic];
		 for (int k = 0; k < num_topic; k ++){
			 prob[k] = 1.0/num_topic;
		 }


		 for (int i = 0 ; i < this->num_word ; i++){

//			 int topic = this->Gen_random_topic(prob,1);
			 int topic =i%num_topic;
			 this->word_topic.push_back(topic);
		 }

}



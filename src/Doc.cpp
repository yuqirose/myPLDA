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

	pair <int, int> word_count (wordID, count);
	this->bag_of_words.push_back(word_count);
	this->num_word++;

}

int Doc::Sample_topic (vector<int> doc_topic_count, vector<int> term_topic_count, vector <int> topic_count,int alpha, int beta){  //Sample_topic (int doc_topic_count[][NUM_TOPIC] ,int term_topic_count[][NUM_TOPIC], int topic_count[],int wordID){
		   int sum_prob=0;
		   int num_topic = doc_topic_count.size();
		   vector<int > prob(num_topic, 0);

		   cout<< "num_topic "<< num_topic << endl;



		   for ( int k = 0; k < num_topic ; k++){
			   prob[k] = (alpha +doc_topic_count[k])*(beta + term_topic_count[k]);
			   prob[k]  = prob[k] /(num_topic * beta+ topic_count[k]);
			   sum_prob += prob[k];
		   }

			   // normalize
		   for ( int k = 0; k < num_topic ; k++){
			   prob[k] =  prob[k] / sum_prob;
		   }

   cout << "work" << endl;
   return 1;
}

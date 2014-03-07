//============================================================================
// Name        : myPLDA.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "myPLDA.h"
using namespace std;




int main() {

	vector<Doc> corpus;
	string fname = "/Users/roseyu/Downloads/DataSet/BagofWords/docword.kos.txt";


	int num_doc, num_term, num_word;


	ifstream bowData(fname.c_str());
    if(bowData.is_open ()){
		bowData >> num_doc >> num_term >> num_word;
    }
    else  cout << "Unable to open file";



    int docID, wordID, count;
    bowData >> docID >> wordID >> count;
    for(int d = 1; d <= num_doc ; d++){
    	Doc new_doc = Doc(docID);
    	while(d == docID){
    		new_doc.Add_pair(wordID,count);
    		if(!bowData.eof())
    			bowData >> docID >> wordID >> count;
    		else
    			break;
    	}

    	corpus.push_back(new_doc);
    	cout << d <<endl;
    }
    cout << "read " << corpus.size() << " Documents";



    /** Sampling
     *
     */
int num_topic = 10; int alpha = 0.1;int beta =0.1;

int num_thread;
int doc_topic_count[num_doc][num_topic];
int term_topic_count [num_term][num_topic];
int topic_count[num_topic];


#pragma omp parallel for
   for (int d = 0 ;  d< 2 ; d++ ){
	   Doc curr_doc = corpus[d];

	   for(int t = 0 ; t< num_term; t++){
		   // remove current count
		   int prob[num_topic];
		   int sum_prob=0;
		   for ( int k = 0; k < num_topic ; k++){
			   prob[k] = (alpha +doc_topic_count[d][k])*(beta + term_topic_count[t][k]);
			   prob[k]  = prob[k] /(num_topic * beta+ topic_count[k]);
			   sum_prob += prob[k];
		   }
		   // normalize
		   for ( int k = 0; k < num_topic ; k++){
			   prob[k] =  prob[k] / sum_prob;
		   }

	   }

   }



	return 0;
}


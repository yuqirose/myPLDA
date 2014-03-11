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
//    	cout << d <<endl;
    }


    cout << "read " << corpus.size() << " Documents" << endl;;



	int num_topic = 10; double alpha = 0.1;double beta =0.1;

	//int num_thread;
	vector< vector<int> > doc_topic_count(num_doc, vector<int>(num_topic,0));
	vector< vector<int> > term_topic_count(num_term, vector<int>(num_topic,0));

	vector < vector < vector<int > > > term_topic_thread ( MAX_TOPIC, vector< vector<int> > (num_term,vector<int>(num_topic,0) ) ) ;
	vector<int>  topic_count(num_topic, 0);


	/** initialization
	 *
	 */

	int initial_doc_topic =num_word/(num_topic * num_doc);
	int initial_term_topic = num_word /(num_topic * num_term);


	#pragma omp parallel for
	for(int d = 0; d< num_doc ; d++)
	{
		for ( int topic = 0; topic < num_topic ;topic++)
		{
			doc_topic_count[d][topic] = initial_doc_topic;
		}
	}

	#pragma omp parallel for
	for(int word = 0 ; word<num_term; word++)
	{
		for (int topic = 0 ; topic < num_topic ;  topic ++)

		{
			term_topic_count[word][topic] = initial_term_topic;
		}
	}


	/** Sampling
	 *
	 */
	#pragma omp parallel for
	for (int t = 0 ; t < term_topic_thread.size(); t++)
		term_topic_thread[t] = term_topic_count;



	#pragma omp parallel for
	   for (int d = 0 ;  d< 1 ; d++ ){
		   Doc curr_doc = corpus[d];
	//		   // remove current count
		   vector<int> prob(num_topic);
		   vector<pair<int, int> >::iterator word_count_iter;
		   vector<pair<int, int> > bagofwords =curr_doc.Get_bagofwords();

		  int  num_thread = omp_get_thread_num();
	//	   cout << "num_thread" << num_thread << endl;

		   for(word_count_iter= bagofwords.begin(); word_count_iter!=bagofwords.end(); ++word_count_iter){
	//		   	  cout << word_count_iter ->first << " "<< word_count_iter->second <<endl;
				int word = word_count_iter ->first;
				int count = word_count_iter ->second;
				cout << "word " << word << " count "<< count << endl;
				for (int c = 0; c < count ; c++){
					int topic = curr_doc.Sample_topic (doc_topic_count[d], term_topic_count[word], topic_count,alpha, beta);
					cout << "topic "<<topic;
					doc_topic_count[d][topic] ++;

					int tid = omp_get_num_threads();
					term_topic_thread[tid %num_thread][word][topic] ++;

				}
		   }

	   }
	   // sync up
	   for (int t = 0 ; t< MAX_TOPIC;  t++){
		   for ( int topic =0; topic < num_topic ; topic++ )
			   for (int word= 0 ; word < num_term; word++)
		   term_topic_count[word][topic] += term_topic_thread[t][word][topic] - term_topic_count[word][topic];
	   }

	   for (int t = 0 ; t< MAX_TOPIC ; t++ ){
		   term_topic_thread[t] = term_topic_count;
	   }

		return 0;
}


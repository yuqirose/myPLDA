//============================================================================
// Name        : myPLDA.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//test
#include <iostream>
#include <math.h>
#include "myPLDA.h"

using namespace std;


double cal_perplexity(vector <Doc> corpus, vector < vector<double> >  Theta, vector < vector<double> >  Phi,int num_doc, int num_topic){
	double perplexity=0;
	int ntest=0;
	for (int d = 0; d< num_doc;d++){
		Doc curr_doc = corpus[d];
		vector<int> prob(num_topic);
		vector<pair<int, int> >::iterator word_count_iter;
		vector<pair<int, int> > bagofwords =curr_doc.Get_bagofwords();

		
		for(word_count_iter= bagofwords.begin(); word_count_iter!=bagofwords.end(); ++word_count_iter){
			double temp=0;
			int word = word_count_iter ->first;
			int count = word_count_iter ->second;
			ntest+=count;
			for( int k = 0; k < num_topic; k++){
				temp+=Theta[d][k]*Phi[k][word];
			}
			perplexity+=log(temp)*count;
			//cout<<perplexity<<endl;
		}
	}
	perplexity=exp(-perplexity/ntest);
	return perplexity;
}

struct compare_index
{
  const vector < double >base_arr;
    compare_index (vector < double >&arr):base_arr (arr)
  {
  }

  bool operator () (int a, int b) const
  {
    return (base_arr[a] < base_arr[b]);
  }
};

int main(int argc, char* argv[]) {

	vector<Doc> corpus;
	string fname = argv[1];
	int max_iter = atoi(argv[2]);
	int num_topic = atoi(argv[3]);;

	int num_doc, num_term, num_word;
	string topic_fname = "topics.txt";
	string doctopic_fname = "doctopics.txt";
	string perp_fname = "perplexity.txt";
	string dic_fname = "BagofWords/vocab.kos.txt";


	ifstream bowData(fname.c_str());
    if(bowData.is_open ()){
		bowData >> num_doc >> num_term >> num_word;
	    cout << "num_doc: "<<num_doc<<" num_term: "<<num_term <<" num_word: "<< num_word << endl;

    }
    else  cout << "Unable to open file";



    int docID, wordID, count;
    bowData >> docID >> wordID >> count;
    for(int d = 1; d <= num_doc ; d++){
    	Doc new_doc = Doc(docID);
    	new_doc.Set_num_term(num_term);
    	while(d == docID){
    		new_doc.Add_pair(wordID,count);
    		if(!bowData.eof())
    			bowData >> docID >> wordID >> count;
    		else
    			break;
    	}
    	corpus.push_back(new_doc);
    }


    cout << "read " << corpus.size() << " Documents" << endl;;



	double alpha = 1;    double beta =1;

	vector< vector<int> > doc_topic_count(num_doc, vector<int>(num_topic,0));
	vector< vector<int> > term_topic_count(num_term, vector<int>(num_topic,0));

	vector < vector < vector<int > > > term_topic_thread ( MAX_THREAD, vector< vector<int> > (num_term,vector<int>(num_topic,0) ) ) ;
	vector<int>  topic_count(num_topic, 0);

	vector <  vector<int>  > topic_thread( MAX_THREAD , vector<int>(num_topic,0));

	// parameters
	vector < vector<double> > Theta (num_doc, vector<double>(num_topic,0) );
	vector < vector<double> > Phi (num_topic, vector<double>(num_term,0) );

	num_doc=20;
	
	/** initialization
	 *
	 */
	#pragma omp parallel for
	for (int d=0; d<num_doc; d++){
		corpus[d].Init_random_topic(num_topic);
	}


	for (int d = 0;  d< num_doc; d++ ){ //NOTE: the array index should be minutes one
	   Doc curr_doc = corpus[d];
	   vector<int> prob(num_topic);
	   vector<pair<int, int> >::iterator word_count_iter;
	   vector<pair<int, int> > bagofwords =curr_doc.Get_bagofwords();

	   int word_idx = 0;

	   for(word_count_iter= bagofwords.begin(); word_count_iter!=bagofwords.end(); ++word_count_iter){
			int word = word_count_iter ->first;
			int count = word_count_iter ->second;
//			cout << "wordID:" <<word<< " count: "<<count << endl;
			for (int c = 0; c < count ; c++){
				int topic =curr_doc.Get_word_topic(word_idx);
				doc_topic_count[d][topic]++;
				term_topic_count[word][topic]++;
				topic_count[topic]++;
      			word_idx++;
			}
	   }
	}

	#pragma omp parallel for
	for (int t = 0 ; t < MAX_THREAD; t++){
		term_topic_thread[t] = term_topic_count;
		topic_thread[t] = topic_count;
	}

	

//	for ( int k = 0; k < num_topic; k++)
//		cout << term_topic_count[0][k] <<" ";
//	cout << endl;


	/** Sampling
	 *
	 */
    double perplexities [max_iter];
	for(int iter =0 ; iter < max_iter ; iter ++) {
		/**
		* Read out parameters
		*/
		vector <int > doc_topic_sum (num_doc,0);
		#pragma omp parallel for
			for (int d =0; d< num_doc;d++){
				for (int k = 0 ; k < num_topic; k++)
					doc_topic_sum [d] += doc_topic_count [d][k];
			}

		#pragma omp parallel for
			for (int d = 0; d< num_doc;d++){
				for( int k = 0; k < num_topic; k++){
					Theta[d][k] = (doc_topic_count [d][k] +alpha )/( (double) doc_topic_sum [d] + num_topic *alpha);
				}
			}

		#pragma omp parallel for
			for (int t= 0; t< num_term; t++){
				for( int k = 0; k < num_topic; k++){
					Phi[k][t] = (term_topic_count [t][k] +beta ) /((double) topic_count [k] + num_term *beta);
				}
			}

		perplexities [iter] =cal_perplexity(corpus, Theta, Phi, num_doc,  num_topic);
		#pragma omp parallel for
		   for (int d = 0 ;  d<num_doc ; d++ ){
			   Doc curr_doc = corpus[d];
			   vector<int> prob(num_topic);
			   vector<pair<int, int> >::iterator word_count_iter;
			   vector<pair<int, int> > bagofwords =curr_doc.Get_bagofwords();

			   int  num_thread = omp_get_thread_num();
			   int word_idx = 0;
			   for(word_count_iter= bagofwords.begin(); word_count_iter!=bagofwords.end(); ++word_count_iter){
					int word = word_count_iter ->first;
					int count = word_count_iter ->second;
					for (int c = 0; c < count ; c++){
						// remove the current word
						int old_topic =curr_doc.Get_word_topic(word_idx);

						doc_topic_count[d][old_topic]--;
						int tid = omp_get_num_threads();
						 // be careful with num_thread = 0;

						term_topic_thread[tid %MAX_THREAD][word][old_topic]--;
						if(term_topic_thread[tid %MAX_THREAD][word][old_topic] <0)
							cout <<"Error" << endl;

						topic_thread[tid %MAX_THREAD][old_topic]--;
						// sample new topic
						int topic = curr_doc.Sample_topic (doc_topic_count[d], term_topic_thread[tid %MAX_THREAD][word], topic_thread[tid %MAX_THREAD],alpha, beta);
						curr_doc.Set_word_topic(topic,word_idx);
						doc_topic_count[d][topic] ++;
						term_topic_thread[tid %MAX_THREAD][word][topic] ++;
						topic_thread[tid %MAX_THREAD][topic] ++;
						word_idx++;
					}
			   }
			   corpus[d] = curr_doc;

		   }
		   // sync up

		   for ( int topic =0; topic < num_topic ; topic++ )
			  for (int word= 0 ; word < num_term; word++){
				   int temp=term_topic_count[word][topic];
				   term_topic_count[word][topic]=0;
				   for (int t = 0 ; t< MAX_THREAD ; t++ )
				   term_topic_count[word][topic] += term_topic_thread[t][word][topic] ;
			   term_topic_count[word][topic]=term_topic_count[word][topic]+(1- MAX_THREAD)*temp;
			   }

		   // combine the topics
		   for (int topic = 0; topic < num_topic ;topic++){
			   topic_count[topic] =0;
			   for (int word= 0 ; word < num_term; word++)
				   topic_count[topic] += term_topic_count[word][topic];
		   }


		   for (int t = 0 ; t< MAX_THREAD ; t++ ){
				topic_thread[t] = topic_count;
			    term_topic_thread[t] = term_topic_count;
		   }

	}

	/**
	 * Output the topics
	 */
	ofstream topic_term(topic_fname.c_str());
	ofstream doc_topic (doctopic_fname.c_str());
	ofstream perp (perp_fname.c_str());
	ifstream dict_stream(dic_fname.c_str());


	int output_size = 100;
	vector<string> dictionary(num_term);
	for (int t =0; t< num_term ; t++){
		string term;
		dict_stream >> term;
		dictionary.push_back(term);
	}
	for (int k =0; k < num_topic ; k++){
		vector<double> curr_topic = Phi[k];
		vector<int> idx(curr_topic.size());
		for(int i =0 ; i<idx.size(); i++) idx[i] = i;
		sort( idx.begin(), idx.end(), compare_index (curr_topic));
		for (int i=0; i< output_size;i++){
			topic_term << idx[i] <<":"<<dictionary[idx[i]] <<":"<<curr_topic[idx[i]] <<",";
		}

		topic_term << endl;
	}


	for(int d=0; d< num_doc ;d++){
		vector<double> curr_doc = Theta[d];
		for(int k =0 ; k < num_topic; k++){
			doc_topic << curr_doc[k] <<",";
		}
		doc_topic << endl;
	}

	for(int i=0 ; i< max_iter;i++){
		perp << perplexities[i] <<",";
	}
	perp << endl;





	return 0;
}



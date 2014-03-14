/*
 * Doc.h
 *
 *  Created on: Mar 6, 2014
 *      Author: roseyu
 */

#ifndef DOC_H_
#define DOC_H_


# include "myPLDA.h"

using namespace std;
class Doc {
private:
	int doc_ID;
	int num_word;
	int num_term;
	vector<pair<int, int> > bag_of_words;
//	vector< vector <int> > term_topic_count;
//	vector<int> topic_count;
	vector<int> word_topic;


public:
	Doc();

	virtual ~Doc();

     Doc(int doc_id){
			this->doc_ID = doc_id;
			this->num_word = 0;
//			this->term_topic_count.resize(num_term, vector<int>(num_topic,0));
//			this->topic_count.resize(num_topic,0);
	}

	void Set_docID(int docID){
		this->doc_ID = docID;
	}
	void Set_num_term (int value){
		this->num_term  = value;
	}


	void Set_word_topic(int topic, int word_idx){
		this->word_topic[word_idx] = topic;

	}

	int Get_word_topic(int word_idx){
		return this->word_topic[word_idx];
	}
	int Get_docID (){
		return this->doc_ID;
	}

	int Get_num_word(){
		return this->num_word;
	}

	vector<pair<int, int> >  Get_bagofwords(){
			return this->bag_of_words;
	}

	void Add_pair( int  wordID, int count);


	int Gen_random_topic(double prob[], int N , int num_topic);

	void Init_random_topic(int num_topic);

	int Sample_topic (vector<int> doc_topic_count, vector<int> term_topic_count, vector <int> topic_count, int alpha, int beta);





};

#endif /* DOC_H_ */

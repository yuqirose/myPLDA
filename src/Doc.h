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
	vector<pair<int, int> > bag_of_words;


public:
	Doc();

	virtual ~Doc();

     Doc(int doc_id){
			this->doc_ID = doc_id;
			this->num_word = 0;
	}

	void Set_docID(int docID){
		this->doc_ID = docID;
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

	int Sample_topic (vector<int> doc_topic_count, vector<int> term_topic_count, vector <int> topic_count, int alpha, int beta);




};

#endif /* DOC_H_ */

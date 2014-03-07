/*
 * Doc.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: roseyu
 */

#include "Doc.h"

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

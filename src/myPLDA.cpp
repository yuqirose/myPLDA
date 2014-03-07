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



	return 0;
}

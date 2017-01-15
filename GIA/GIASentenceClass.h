/*******************************************************************************
 *
 * File Name: GIASentenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1d4b 01-Nov-2011
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SENTENCE_CLASS
#define HEADER_GIA_SENTENCE_CLASS

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
using namespace std;

#define FEATURE_GRAMMATICAL_TENSE_DATE "date"
#define FEATURE_GRAMMATICAL_TENSE_PAST "past"
//#define FEATURE_GRAMMATICAL_COUNT ".c"

//#define DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS

class Relation
{
public:

	Relation(void);
	~Relation(void);

	string relationType;
	string relationArgument;
	int relationArgumentIndex;
	string relationFunction;
	int relationFunctionIndex;
	
	bool disabled;
	
	Relation * next;
};


class Feature
{
public:

	Feature(void);
	~Feature(void);

	int entityIndex;
	string word;
	string lemma;
	string type;
	string grammar;
	
	Feature * next;
	Feature * previous;	//used for reference lookup
};



class Sentence
{
public:

	Sentence(void);
	~Sentence(void);
	
	string sentenceText;		//not required - delete in future
	string constituentsText;	//not required - delete in future
	string featuresText;		//not required - delete in future
	string relationsText;		//not required - delete in future
	string linksText;		//not required - delete in future
	
	int maxNumberOfWordsInSentence;
		
	Relation * firstRelationInList;
	Feature * firstFeatureInList;
	
	Sentence * next;
	Sentence * previous;	//used for reference lookup

};

#endif

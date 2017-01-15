/*******************************************************************************
 *
 * File Name: GIASentenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1g6c 12-Feb-2012
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
#define FEATURE_TYPE_PUNCTUATION "punctuation"
#define FEATURE_TYPE_ADJECTIVE "adj"
#define FEATURE_TYPE_NOUN "noun"
#define FEATURE_TYPE_VERB "verb"
#define FEATURE_WORD_QUESTIONMARK "?"

#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY (0)
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (0)
#else
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY (1)
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (1)
#endif



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

	bool isQuestion;
};

#endif

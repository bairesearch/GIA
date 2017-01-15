/*******************************************************************************
 *
 * File Name: GIASentenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i4a 3-Apr-2012
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

#include "GIAglobalDefs.h"

#define FEATURE_GRAMMATICAL_TENSE_DATE "date"
#define FEATURE_GRAMMATICAL_TENSE_PAST "past"
#define FEATURE_TYPE_PUNCTUATION "punctuation"
#define FEATURE_TYPE_ADJECTIVE "adj"
#define FEATURE_TYPE_NOUN "noun"
#define FEATURE_TYPE_VERB "verb"
#define FEATURE_WORD_QUESTIONMARK "?"



//#define FEATURE_GRAMMATICAL_COUNT ".c"


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

	bool subjObjRelationAlreadyAdded;
		
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

#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
class Paragraph
{
public:

	Paragraph(void);
	~Paragraph(void);

	Sentence * firstSentenceInList;

	Paragraph * next;
	Paragraph * previous;	//used for reference lookup
};
#endif

#endif

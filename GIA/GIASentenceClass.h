/*******************************************************************************
 *
 * File Name: GIASentenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i14c 17-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
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
#define FEATURE_TYPE_PUNCTUATION "punctuation"
#define FEATURE_TYPE_ADJECTIVE "adj"
#define FEATURE_TYPE_NOUN "noun"
#define FEATURE_TYPE_VERB "verb"
#define FEATURE_WORD_QUESTIONMARK "?"



//#define FEATURE_GRAMMATICAL_COUNT ".c"



#ifdef GIA_USE_STANFORD_CORENLP
class StanfordCoreNLPMention
{
public:

	StanfordCoreNLPMention(void);
	~StanfordCoreNLPMention(void);

	bool representative;
	int sentence;
	int start;
	int end;
	int head;
	
	StanfordCoreNLPMention * next;
};

class StanfordCoreNLPCoreference
{
public:

	StanfordCoreNLPCoreference(void);
	~StanfordCoreNLPCoreference(void);

	StanfordCoreNLPMention * firstMentionInList;
	
	StanfordCoreNLPCoreference * next;
};

#endif


class Relation
{
public:

	Relation(void);
	~Relation(void);

	string relationType;
	string relationDependent;
	int relationDependentIndex;
	string relationGovernor;
	int relationGovernorIndex;
	
	bool disabled;

	#ifdef GIA_USE_RELEX
	bool subjObjRelationAlreadyAdded;	//Relex Only
	#endif
	
	#ifdef GIA_USE_STANFORD_CORENLP
	bool prepositionCombinationAlreadyCreatedTemp;
	#endif	
	
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
	
	#ifdef GIA_USE_RELEX
	string type;
	string grammar;
	#endif
	
	#ifdef GIA_USE_STANFORD_CORENLP
	int CharacterOffsetBegin;
	int CharacterOffsetEnd;
	string POS;
	string NER;
	string NormalizedNER;
	string Timex;
	#endif
	
	Feature * next;
	Feature * previous;	//used for reference lookup
};



class Sentence
{
public:

	Sentence(void);
	~Sentence(void);
	
	#ifdef GIA_USE_RELEX
	string sentenceText;		//not required - delete in future
	string constituentsText;	//not required - delete in future
	string featuresText;		//not required - delete in future
	string relationsText;		//not required - delete in future
	string linksText;		//not required - delete in future
	#endif

	#ifdef GIA_USE_STANFORD_CORENLP
	int sentenceIndex;
	StanfordCoreNLPCoreference * firstCoreferenceInList;
	#endif
		
	int maxNumberOfWordsInSentence;
		
	Relation * firstRelationInList;
	Feature * firstFeatureInList;
	
	Sentence * next;
	Sentence * previous;	//used for reference lookup

	bool isQuestion;
};

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

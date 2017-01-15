/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIASentenceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1q4f 14-October-2012
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
#include "GIAEntityNodeClass.h"
#include "GIAEntityConnectionClass.h"

//#define FEATURE_GRAMMATICAL_TENSE_DATE "date"
#define FEATURE_WORD_QUESTIONMARK "?"

//from http://wiki.opencog.org/w/Word_substances (intermediary - used for translation purposes into WordNet int wordNetPOS only)
#define FEATURE_RELEX_POS_TYPE_ADJECTIVE (1)
#define FEATURE_RELEX_POS_TYPE_ADVERB (2)
#define FEATURE_RELEX_POS_TYPE_CONJUNCTION (3)
#define FEATURE_RELEX_POS_TYPE_DETERMINATE (4)
#define FEATURE_RELEX_POS_TYPE_NOUN (5)
#define FEATURE_RELEX_POS_TYPE_PARTICLE (6)
#define FEATURE_RELEX_POS_TYPE_PREP (7)
#define FEATURE_RELEX_POS_TYPE_PUNCTUATION (8)
#define FEATURE_RELEX_POS_TYPE_VERB (9)
#define FEATURE_RELEX_POS_TYPE_WORD (10)
#define FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME "adj"
#define FEATURE_RELEX_POS_TYPE_ADVERB_NAME "adv"
#define FEATURE_RELEX_POS_TYPE_CONJUNCTION_NAME "conjunction"
#define FEATURE_RELEX_POS_TYPE_DETERMINATE_NAME "det"
#define FEATURE_RELEX_POS_TYPE_NOUN_NAME "noun"
#define FEATURE_RELEX_POS_TYPE_PARTICLE_NAME "particle"
#define FEATURE_RELEX_POS_TYPE_PREP_NAME "prep"
#define FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME "punctuation"
#define FEATURE_RELEX_POS_TYPE_VERB_NAME "verb"
#define FEATURE_RELEX_POS_TYPE_WORD_NAME "WORD"
#define FEATURE_RELEX_POS_NUMBER_OF_TYPES (11)
static string featureRelexPOSTypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_CONJUNCTION_NAME, FEATURE_RELEX_POS_TYPE_DETERMINATE_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_PARTICLE_NAME, FEATURE_RELEX_POS_TYPE_PREP_NAME, FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_WORD_NAME};


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


#ifdef GIA_USE_ADVANCED_REFERENCING
class GIAMention
{
public:

	GIAMention(void);
	~GIAMention(void);

	bool representative;
	long idActiveList;
	int entityIndex;	//ie "head"
	string entityName;
	bool intrasentenceReference;

	GIAMention * next;
};

class GIACoreference
{
public:

	GIACoreference(void);
	~GIACoreference(void);

	GIAMention * firstMentionInList;

	GIACoreference * next;
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

	#ifdef GIA_USE_ADVANCED_REFERENCING
	bool auxillaryIndicatesDifferentReferenceSet;
	bool rcmodIndicatesSameReferenceSet;
	#endif

	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	bool isNameTemp;
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
	#ifdef GIA_USE_LRP
	string wordWithLRPforNLPonly;
	#endif

	#ifdef GIA_USE_RELEX
	string type;
	string grammar;
	#endif

	int NER;
	#ifdef GIA_USE_STANFORD_CORENLP
	int CharacterOffsetBegin;
	int CharacterOffsetEnd;
	string stanfordPOS;
	string NormalizedNER;
	string Timex;
	#endif

	//derived variables:
	bool grammaticalIsDateOrTime;
	int grammaticalTense;
	bool grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];
	int grammaticalNumber;
	bool grammaticalIsDefinite;
	bool grammaticalIsProperNoun;
	int grammaticalGender;
	bool grammaticalIsPronoun;
	int grammaticalWordType;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	int grammaticalIsDefiniteIndexOfDeterminer;
	#endif

	bool alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp;	//#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES

	bool isPronounReference;

	bool entityDisabled;	//added 14 July 2012b

	bool mustSetIsSubstanceConceptBasedOnApposRelation;
	
	Feature * next;
	Feature * previous;	//used for reference lookup
};



class Sentence
{
public:

	Sentence(void);
	~Sentence(void);

	int sentenceIndex;

	#ifdef GIA_USE_RELEX
	string sentenceText;		//not required - delete in future
	string constituentsText;	//not required - delete in future
	string featuresText;		//not required - delete in future
	string relationsText;		//not required - delete in future
	string linksText;		//not required - delete in future
	#endif

	#ifdef GIA_USE_STANFORD_CORENLP
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

void copySentences(Sentence * sentenceToCopy, Sentence * newSentence);
void copyRelations(Relation * firstRelationInListToCopy, Relation * firstRelationInList);
void copyFeatures(Feature * firstFeatureInListToCopy, Feature * firstFeatureInList);
#ifdef GIA_USE_STANFORD_CORENLP
void copyStanfordCoreferences(StanfordCoreNLPCoreference * firstCoreferenceInListToCopy, StanfordCoreNLPCoreference * firstCoreferenceInList);
void copyStanfordMention(StanfordCoreNLPMention * firstMentionInListToCopy, StanfordCoreNLPMention * firstMentionInList);
#endif

#endif

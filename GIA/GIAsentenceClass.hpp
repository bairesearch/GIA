/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsentenceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k14d 12-May-2020
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Sentence Class
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SENTENCE_CLASS
#define HEADER_GIA_SENTENCE_CLASS

#include "GIAglobalDefs.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"

//#define FEATURE_GRAMMATICAL_TENSE_DATE "date"
#define FEATURE_WORD_QUESTIONMARK "?"

#define MAX_NUMBER_OF_RELATIONS_PER_SENTENCE (1000)
#define MAX_NUMBER_OF_SPECIAL_WORDS_PER_SENTENCE (100)
#define SENTENCE_FIRST_ARTIFICIAL_INDEX (0)	//after numberOfWordsInSentence (the size of the sentence) - OLD:10

//from http://wiki.opencog.org/w/Word_substances (intermediary - used for translation purposes into WordNet int grammaticalWordTypeTemp only)
#define FEATURE_RELEX_POS_TYPE_ADJECTIVE (1)
#define FEATURE_RELEX_POS_TYPE_ADVERB (2)
#define FEATURE_RELEX_POS_TYPE_CONJUNCTION (3)
#define FEATURE_RELEX_POS_TYPE_DETERMINER (4)
#define FEATURE_RELEX_POS_TYPE_NOUN (5)
#define FEATURE_RELEX_POS_TYPE_PARTICLE (6)
#define FEATURE_RELEX_POS_TYPE_PREP (7)
#define FEATURE_RELEX_POS_TYPE_PUNCTUATION (8)
#define FEATURE_RELEX_POS_TYPE_VERB (9)
#define FEATURE_RELEX_POS_TYPE_WORD (10)
#define FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME "adj"
#define FEATURE_RELEX_POS_TYPE_ADVERB_NAME "adv"
#define FEATURE_RELEX_POS_TYPE_CONJUNCTION_NAME "conjunction"
#define FEATURE_RELEX_POS_TYPE_DETERMINER_NAME "det"
#define FEATURE_RELEX_POS_TYPE_NOUN_NAME "noun"
#define FEATURE_RELEX_POS_TYPE_PARTICLE_NAME "particle"
#define FEATURE_RELEX_POS_TYPE_PREP_NAME "prep"
#define FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME "punctuation"
#define FEATURE_RELEX_POS_TYPE_VERB_NAME "verb"
#define FEATURE_RELEX_POS_TYPE_WORD_NAME "WORD"
#define FEATURE_RELEX_POS_NUMBER_OF_TYPES (11)
/*
add this "abbr" to POS types (eg Mr. -> wordnet noun)?
see relex/relex-1.4.0/data/relex-tagging-algs.txt
NB pronouns are "already tagged as nouns"
*/
static string featureRelexPOStypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_CONJUNCTION_NAME, FEATURE_RELEX_POS_TYPE_DETERMINER_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_PARTICLE_NAME, FEATURE_RELEX_POS_TYPE_PREP_NAME, FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_WORD_NAME};


//#define FEATURE_GRAMMATICAL_COUNT ".c"



#ifdef GIA_STANFORD_CORENLP
class GIAstanfordCoreNLPmention
{
public:

	GIAstanfordCoreNLPmention(void);
	~GIAstanfordCoreNLPmention(void);

	bool representative;
	int sentence;
	int start;
	int end;
	int head;

	GIAstanfordCoreNLPmention* next;
};

class GIAstanfordCoreNLPcoreference
{
public:

	GIAstanfordCoreNLPcoreference(void);
	~GIAstanfordCoreNLPcoreference(void);

	GIAstanfordCoreNLPmention* firstMentionInList;

	GIAstanfordCoreNLPcoreference* next;
};

#endif

//#ifdef GIA_ADVANCED_REFERENCING
//these classes are only used by GIA_ADVANCED_REFERENCING:
class GIAMention
{
public:

	GIAMention(void);
	~GIAMention(void);

	bool representative;
	int64_t idActiveList;
	int entityIndex;	//ie "head"
	string entityName;
	bool intrasentenceReference;

	GIAMention* next;
};

class GIAcoreference
{
public:

	GIAcoreference(void);
	~GIAcoreference(void);

	GIAMention* firstMentionInList;

	GIAcoreference* next;
};
//#endif





class GIArelation
{
public:

	GIArelation(void);
	~GIArelation(void);

	string relationType;
	int relationTypeIndex;
	string relationGovernor;
	int relationGovernorIndex;
	string relationDependent;
	int relationDependentIndex;
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	bool relationGovernorRevertedToOfficialLRPTemp;
	bool relationDependentRevertedToOfficialLRPTemp;
	#endif

	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
	string relationTypeNonInversed;
	int relationTypeIndexNonInversed;
	#endif

	bool disabled;
	//#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	bool disabledDuringLink;
	//#endif

	#ifdef GIA_RELEX
	bool subjObjRelationAlreadyAdded;	//Relex Only
	#endif

	#ifdef GIA_STANFORD_CORENLP
	bool prepositionCombinationAlreadyCreatedTemp;
	#endif

	bool auxiliaryIndicatesDifferentReferenceSet;
	bool rcmodIndicatesSameReferenceSet;

	#ifdef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	bool isNameTemp;
	#endif

	#ifdef GIA_SEM_REL_TRANSLATOR_COMMON
	bool sameReferenceSet;
	#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
	string corpusSpecialRelationGovernorIsQuery;
	string corpusSpecialRelationDependentIsQuery;
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	bool inverseRelation;
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
	bool inverseRelationSingle;
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
	bool relationTwoWay;
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	bool inverseRelationTwoWay;
	#endif
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	string semanticRelationFunctionConditionNewName;
	#endif

	GIArelation* next;
};


class GIAfeature
{
public:

	GIAfeature(void);
	~GIAfeature(void);

	int entityIndex;
	string word;
	string lemma;
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	string wordWithLRPforNLPonly;
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	bool featureRevertedToOfficialLRPTemp;	//not currently used
	#endif

	#ifdef GIA_RELEX
	string type;
	string grammar;
	#endif

	int NER;
	string stanfordPOS;
	#ifdef GIA_STANFORD_CORENLP
	int CharacterOffsetBegin;
	int CharacterOffsetEnd;
	string NormalizedNER;
	string Timex;
	#endif

	//derived variables:
	bool grammaticalIsDateOrTime;
	int grammaticalTense;
	bool grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];
	int grammaticalNumber;
	bool grammaticalIsDefinite;
	bool grammaticalIsIndefinitePlural;
	bool grammaticalIsProperNoun;
	int grammaticalGender;
	bool grammaticalIsPronoun;
	int grammaticalWordType;	//grammaticalWordTypeTemp
	#ifdef GIA_PREPROCESSOR_POS_TAGGER
	int GIAposType;
	#endif
	int grammaticalIndexOfDeterminer;
	bool previousWordInSentenceIsTo;
	#ifdef GIA_PREDETERMINERS
	int grammaticalPredeterminer;
	#endif

	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	bool alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp;	//#ifdef GIA_SYN_REL_TRANSLATOR_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	bool mustSetIsConceptBasedOnApposRelation;
	bool isPronounReference;
	#endif

	bool entityDisabled;	//added 14 July 2012b

	#ifdef GIA_SEM_REL_TRANSLATOR
	int GIAsemRelTranslatorPOStype;
	#endif

	#ifdef GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER
	bool determinerPotentiallySingularDetected;
	#endif

	GIAfeature* next;
	GIAfeature* previous;	//used for reference lookup
};



class GIAsentence
{
public:

	GIAsentence(void);
	~GIAsentence(void);

	int sentenceIndex;

	#ifdef GIA_RELEX
	string sentenceText;		//not required - delete in future
	string constituentsText;	//not required - delete in future
	string featuresText;		//not required - delete in future
	string relationsText;		//not required - delete in future
	string linksText;		//not required - delete in future
	#endif

	#ifdef GIA_STANFORD_CORENLP
	GIAstanfordCoreNLPcoreference* firstCoreferenceInList;
	#endif

	int numberOfWordsInSentence;

	GIArelation* firstRelationInList;
	GIAfeature* firstFeatureInList;
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIArelation* firstRelationInListOriginal;
	GIAfeature* firstFeatureInListOriginal;
	#endif
	
	GIAsentence* next;
	GIAsentence* previous;	//used for reference lookup

	bool isQuestion;

	bool semanticParserSuccessful;

	int relationshipEntityArtificialIndexCurrent;	//added 2g1a/25-August-2014
};

class GIAparagraph
{
public:

	GIAparagraph(void);
	~GIAparagraph(void);

	GIAsentence* firstSentenceInList;

	GIAparagraph* next;
	GIAparagraph* previous;	//used for reference lookup
};

class GIAsentenceClassClass
{
	public: void backupOriginalNLPsentenceContent(GIAsentence* firstSentenceInList);
	public: void copySentences(GIAsentence* firstSentenceInListToCopy, GIAsentence* firstSentenceInList);
		public: void copySentence(GIAsentence* sentenceToCopy, GIAsentence* newSentence);
			private: void copyRelations(GIArelation* firstRelationInListToCopy, GIArelation* firstRelationInList);
			private: void copyFeatures(GIAfeature* firstFeatureInListToCopy, GIAfeature* firstFeatureInList);
			#ifdef GIA_STANFORD_CORENLP
			private: void copyStanfordCoreferences(GIAstanfordCoreNLPcoreference* firstCoreferenceInListToCopy, GIAstanfordCoreNLPcoreference* firstCoreferenceInList);
				private: void copyStanfordMention(GIAstanfordCoreNLPmention* firstMentionInListToCopy, GIAstanfordCoreNLPmention* firstMentionInList);
			#endif

	public: int calculateNumberOfWordsInSentence(const GIAfeature* firstFeatureInList);
	
	int getMinIndexOfDynamicallyGeneratedEntity(GIAsentence* currentSentenceInList);
	int getMaxIndexOfDynamicallyGeneratedEntity(GIAsentence* currentSentenceInList);
	int getMinIndexOfDynamicallyGeneratedEntity(const int numberOfWordsInSentence);
	int getMaxIndexOfDynamicallyGeneratedEntity(const int numberOfWordsInSentence);
	bool relationIndexIsNormal(int relationIndex);
};

#endif

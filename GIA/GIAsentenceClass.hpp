/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
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
 * File Name: GIAsentenceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1a 26-February-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SENTENCE_CLASS
#define HEADER_GIA_SENTENCE_CLASS

#include "GIAglobalDefs.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"

//#define FEATURE_GRAMMATICAL_TENSE_DATE "date"
#define FEATURE_WORD_QUESTIONMARK "?"

#define MAX_NUMBER_OF_RELATIONS_PER_SENTENCE (1000)
#define MAX_NUMBER_OF_WORDS_PER_SENTENCE (1000)

//from http://wiki.opencog.org/w/Word_substances (intermediary - used for translation purposes into WordNet int grammaticalWordTypeTemp only)
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
/*
add this "abbr" to POS types (eg Mr. -> wordnet noun)?
see relex/relex-1.4.0/data/relex-tagging-algs.txt
NB pronouns are "already tagged as nouns"
*/
static string featureRelexPOStypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_CONJUNCTION_NAME, FEATURE_RELEX_POS_TYPE_DETERMINATE_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_PARTICLE_NAME, FEATURE_RELEX_POS_TYPE_PREP_NAME, FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_WORD_NAME};


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
	long idActiveList;
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
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
	int relationTypeIndex;
	#endif
	string relationGovernor;
	int relationGovernorIndex;
	string relationDependent;
	int relationDependentIndex;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	bool relationGovernorRevertedToOfficialLRPTemp;
	bool relationDependentRevertedToOfficialLRPTemp;
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
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

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	bool auxiliaryIndicatesDifferentReferenceSet;
	bool rcmodIndicatesSameReferenceSet;
	#endif

	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	bool isNameTemp;
	#endif

	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
	bool sameReferenceSet;
	#ifdef GIA2_SUPPORT_QUERIES
	string corpusSpecialRelationGovernorIsQuery;
	string corpusSpecialRelationDependentIsQuery;
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
	bool inverseRelation;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
	bool inverseRelationSingle;
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
	bool relationTwoWay;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	bool inverseRelationTwoWay;
	#endif
	#endif
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
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	string wordWithLRPforNLPonly;
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	bool featureRevertedToOfficialLRPTemp;	//not currently used
	#endif

	#ifdef GIA_RELEX
	string type;
	string grammar;
	#endif

	int NER;
	#ifdef GIA_STANFORD_CORENLP
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
	bool grammaticalIsIndefinitePlural;
	bool grammaticalIsProperNoun;
	int grammaticalGender;
	bool grammaticalIsPronoun;
	int grammaticalWordType;	//grammaticalWordTypeTemp
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	int grammaticalIndexOfDeterminer;
	#endif
	bool previousWordInSentenceIsTo;
	#ifdef GIA_PREDETERMINERS
	int grammaticalPredeterminer;
	#endif

	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	bool alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp;	//#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	bool mustSetIsConceptBasedOnApposRelation;
	bool isPronounReference;
	#endif

	bool entityDisabled;	//added 14 July 2012b

	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
	int GIAsemanticParserPOStype;
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

	int maxNumberOfWordsInSentence;

	GIArelation* firstRelationInList;
	GIAfeature* firstFeatureInList;

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
	public: void copySentences(GIAsentence* sentenceToCopy, GIAsentence* newSentence);
	private: void copyRelations(GIArelation* firstRelationInListToCopy, GIArelation* firstRelationInList);
	private: void copyFeatures(GIAfeature* firstFeatureInListToCopy, GIAfeature* firstFeatureInList);
#ifdef GIA_STANFORD_CORENLP
	private: void copyStanfordCoreferences(GIAstanfordCoreNLPcoreference* firstCoreferenceInListToCopy, GIAstanfordCoreNLPcoreference* firstCoreferenceInList);
	private: void copyStanfordMention(GIAstanfordCoreNLPmention* firstMentionInListToCopy, GIAstanfordCoreNLPmention* firstMentionInList);
#endif

	public: int calculateNumberOfWordsInSentence(const GIAfeature* firstFeatureInList);
};

#endif

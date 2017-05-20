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
 * File Name: GIAsentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b1b 19-May-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#include "GIAsentenceClass.hpp"


#ifdef GIA_STANFORD_CORENLP


GIAstanfordCoreNLPmention::GIAstanfordCoreNLPmention(void)
{
	representative = false;
	sentence = INT_DEFAULT_VALUE;
	start = INT_DEFAULT_VALUE;
	end = INT_DEFAULT_VALUE;
	head = INT_DEFAULT_VALUE;

	next = NULL;
}

GIAstanfordCoreNLPmention::~GIAstanfordCoreNLPmention(void)
{
	if(next != NULL)
	{
		delete next;
	}
}

GIAstanfordCoreNLPcoreference::GIAstanfordCoreNLPcoreference(void)
{
	firstMentionInList = new GIAstanfordCoreNLPmention();

	next = NULL;
}

GIAstanfordCoreNLPcoreference::~GIAstanfordCoreNLPcoreference(void)
{
	if(firstMentionInList != NULL)
	{
		delete firstMentionInList;
	}

	if(next != NULL)
	{
		delete next;
	}
}
#endif

//#ifdef GIA_ADVANCED_REFERENCING
//these classes are only used by GIA_ADVANCED_REFERENCING:
GIAMention::GIAMention(void)
{
	representative = false;
	idActiveList = INT_DEFAULT_VALUE;
	entityIndex = INT_DEFAULT_VALUE;	//ie, "head"
	entityName = "";
	intrasentenceReference = false;		//only applies to representative/source mentions (specifies whether reference source is contained within current sentence)

	next = NULL;
}

GIAMention::~GIAMention(void)
{
	if(next != NULL)
	{
		delete next;
	}
}

GIAcoreference::GIAcoreference(void)
{
	firstMentionInList = new GIAMention();

	next = NULL;
}

GIAcoreference::~GIAcoreference(void)
{
	if(firstMentionInList != NULL)
	{
		delete firstMentionInList;
	}

	if(next != NULL)
	{
		delete next;
	}
}
//#endif






GIArelation::GIArelation(void)
{
	relationType = "";
	relationTypeIndex = INT_DEFAULT_VALUE;
	relationDependent = "";
	relationDependentIndex = INT_DEFAULT_VALUE;
	relationGovernor = "";
	relationGovernorIndex = INT_DEFAULT_VALUE;
	#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
	relationGovernorRevertedToOfficialLRPTemp = false;
	relationDependentRevertedToOfficialLRPTemp = false;
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
	relationTypeNonInversed = "";
	relationTypeIndexNonInversed = INT_DEFAULT_VALUE;
	#endif

	disabled = false;
	//#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	disabledDuringLink = false;
	//#endif

	#ifdef GIA_RELEX
	subjObjRelationAlreadyAdded = false;
	#endif

	#ifdef GIA_STANFORD_CORENLP
	prepositionCombinationAlreadyCreatedTemp = false;
	#endif

	auxiliaryIndicatesDifferentReferenceSet = false;
	rcmodIndicatesSameReferenceSet = false;

	#ifdef GIA_SEMANTIC_PARSER
	sameReferenceSet = false;
	#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
	corpusSpecialRelationGovernorIsQuery = "";
	corpusSpecialRelationDependentIsQuery = "";
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
	inverseRelation = false;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
	inverseRelationSingle = false;
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
	relationTwoWay = false;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	inverseRelationTwoWay = false;
	#endif
	#endif
	#endif

	next = NULL;
}

GIArelation::~GIArelation(void)
{
	if(next != NULL)
	{
		delete next;
	}

}

GIAfeature::GIAfeature(void)
{
	entityIndex = 0;
	word = "";
	lemma = "";
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	wordWithLRPforNLPonly = "";
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	featureRevertedToOfficialLRPTemp = "";
	#endif

	#ifdef GIA_RELEX
	type = "";
	grammar = "";
	#endif

	NER = FEATURE_NER_UNDEFINED;
	#ifdef GIA_STANFORD_CORENLP
	CharacterOffsetBegin = INT_DEFAULT_VALUE;
	CharacterOffsetEnd = INT_DEFAULT_VALUE;
	stanfordPOS = "";
	NormalizedNER = "";
	Timex = "";
	#endif

	//derived variables:
	grammaticalIsDateOrTime = false;
	grammaticalTense = GRAMMATICAL_TENSE_UNDEFINED;
	for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
	{
		grammaticalTenseModifierArray[q] = false;
	}
	grammaticalNumber = GRAMMATICAL_NUMBER_UNDEFINED;
	grammaticalIsDefinite = false;
	grammaticalIsIndefinitePlural = false;
	grammaticalIsProperNoun = false;
	grammaticalGender = GRAMMATICAL_GENDER_UNDEFINED;
	grammaticalIsPronoun = false;
	grammaticalWordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	#ifdef GIA_PREDETERMINERS
	grammaticalPredeterminer = GRAMMATICAL_PREDETERMINER_UNDEFINED;
	#endif
	previousWordInSentenceIsTo = false;

	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;		//#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	mustSetIsConceptBasedOnApposRelation = false;
	isPronounReference = false;
	#endif

	entityDisabled = false;

	#ifdef GIA_SEMANTIC_PARSER
	GIAsemanticParserPOStype = 0;	//ie GIA_SEMANTIC_PARSER_POS_TYPE_UNDEFINED
	#endif

	#ifdef GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER
	determinerPotentiallySingularDetected = false;
	#endif

	next = NULL;
	previous = NULL;
}

GIAfeature::~GIAfeature(void)
{
	if(next != NULL)
	{
		delete next;
	}

}


GIAsentence::GIAsentence(void)
{
	#ifdef GIA_RELEX
	sentenceText = "";
	constituentsText = "";
	featuresText = "";
	relationsText = "";
	linksText = "";
	#endif

	sentenceIndex = GIA_SENTENCE_INDEX_UNDEFINED;
	#ifdef GIA_STANFORD_CORENLP
	firstCoreferenceInList = new GIAstanfordCoreNLPcoreference();
	#endif

	maxNumberOfWordsInSentence = 0;

	firstRelationInList = new GIArelation();	//auto constructor execution added 23 Feb 2012
	firstFeatureInList = new GIAfeature();	//auto constructor execution added 23 Feb 2012

	next = NULL;
	previous = NULL;

	isQuestion = false;

	semanticParserSuccessful = false;

	relationshipEntityArtificialIndexCurrent = SENTENCE_FIRST_ARTIFICIAL_INDEX;
}

GIAsentence::~GIAsentence(void)
{
	if(firstRelationInList != NULL)
	{
		delete firstRelationInList;
	}

	if(firstFeatureInList != NULL)	//added 23 Feb 2012
	{
		delete firstFeatureInList;
	}

	#ifdef GIA_STANFORD_CORENLP
	if(firstCoreferenceInList != NULL)	//added 21 Sept 2012
	{
		delete firstCoreferenceInList;
	}
	#endif

	if(next != NULL)
	{
		delete next;
	}

}


GIAparagraph::GIAparagraph(void)
{
	firstSentenceInList = new GIAsentence();

	next = NULL;
	previous = NULL;
}

GIAparagraph::~GIAparagraph(void)
{
	if(firstSentenceInList != NULL)
	{
		delete firstSentenceInList;
	}

	if(next != NULL)
	{
		delete next;
	}
}

void GIAsentenceClassClass::copySentences(GIAsentence* sentenceToCopy, GIAsentence* newSentence)
{
	newSentence->sentenceIndex = sentenceToCopy->sentenceIndex;

	#ifdef GIA_RELEX
	newSentence->sentenceText = sentenceToCopy->sentenceText;
	newSentence->constituentsText = sentenceToCopy->constituentsText;
	newSentence->featuresText = sentenceToCopy->featuresText;
	newSentence->relationsText = sentenceToCopy->relationsText;
	newSentence->linksText = sentenceToCopy->linksText;
	#endif

	#ifdef GIA_STANFORD_CORENLP
	this->copyStanfordCoreferences(sentenceToCopy->firstCoreferenceInList, newSentence->firstCoreferenceInList);	//changed 21 Sept 2012
	//newSentence->firstCoreferenceInList = sentenceToCopy->firstCoreferenceInList;
	#endif

	newSentence->maxNumberOfWordsInSentence = sentenceToCopy->maxNumberOfWordsInSentence;

	this->copyRelations(sentenceToCopy->firstRelationInList, newSentence->firstRelationInList);
	this->copyFeatures(sentenceToCopy->firstFeatureInList, newSentence->firstFeatureInList);

	newSentence->next = NULL;
	newSentence->previous = NULL;

	newSentence->isQuestion = sentenceToCopy->isQuestion;
}


void GIAsentenceClassClass::copyRelations(GIArelation* firstRelationInListToCopy, GIArelation* firstRelationInList)
{
	GIArelation* currentRelationToCopy = firstRelationInListToCopy;
	GIArelation* currentRelation = firstRelationInList;
	while(currentRelationToCopy->next != NULL)
	{

		currentRelation->relationType = currentRelationToCopy->relationType;
		currentRelation->relationTypeIndex = currentRelationToCopy->relationTypeIndex;
		currentRelation->relationDependent = currentRelationToCopy->relationDependent;
		currentRelation->relationDependentIndex = currentRelationToCopy->relationDependentIndex;
		currentRelation->relationGovernor = currentRelationToCopy->relationGovernor;
		currentRelation->relationGovernorIndex = currentRelationToCopy->relationGovernorIndex;


		#ifdef GIA_SEMANTIC_PARSER
		#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
		currentRelation->corpusSpecialRelationGovernorIsQuery = currentRelationToCopy->corpusSpecialRelationGovernorIsQuery;
		currentRelation->corpusSpecialRelationDependentIsQuery = currentRelationToCopy->corpusSpecialRelationDependentIsQuery;
		#endif
		#endif

		GIArelation* newRelation = new GIArelation();
		//newRelation->previous = currentRelation;
		currentRelation->next = newRelation;

		currentRelationToCopy = currentRelationToCopy->next;
		currentRelation = currentRelation->next;
	}
}

void GIAsentenceClassClass::copyFeatures(GIAfeature* firstFeatureInListToCopy, GIAfeature* firstFeatureInList)
{
	GIAfeature* currentFeatureToCopy = firstFeatureInListToCopy;
	GIAfeature* currentFeature = firstFeatureInList;
	while(currentFeatureToCopy->next != NULL)
	{

		currentFeature->entityIndex = currentFeatureToCopy->entityIndex;
		currentFeature->word = currentFeatureToCopy->word;
		currentFeature->lemma = currentFeatureToCopy->lemma;

		#ifdef GIA_RELEX
		currentFeature->type = currentFeatureToCopy->type;
		currentFeature->grammar = currentFeatureToCopy->grammar;
		#endif

		currentFeature->NER = currentFeatureToCopy->NER;
		#ifdef GIA_STANFORD_CORENLP
		currentFeature->CharacterOffsetBegin = currentFeatureToCopy->CharacterOffsetBegin;
		currentFeature->CharacterOffsetEnd = currentFeatureToCopy->CharacterOffsetEnd;
		currentFeature->stanfordPOS = currentFeatureToCopy->stanfordPOS;
		currentFeature->NormalizedNER = currentFeatureToCopy->NormalizedNER;
		currentFeature->Timex = currentFeatureToCopy->Timex;
		#endif


		GIAfeature* newFeature = new GIAfeature();
		newFeature->previous = currentFeature;
		currentFeature->next = newFeature;

		currentFeatureToCopy = currentFeatureToCopy->next;
		currentFeature = currentFeature->next;
	}
}


#ifdef GIA_STANFORD_CORENLP
void GIAsentenceClassClass::copyStanfordCoreferences(GIAstanfordCoreNLPcoreference* firstCoreferenceInListToCopy, GIAstanfordCoreNLPcoreference* firstCoreferenceInList)
{
	GIAstanfordCoreNLPcoreference* currentCoreferenceInListToCopy = firstCoreferenceInListToCopy;
	GIAstanfordCoreNLPcoreference* currentCoreferenceInList = firstCoreferenceInList;
	while(currentCoreferenceInListToCopy->next != NULL)
	{
		currentCoreferenceInList->firstMentionInList = new GIAstanfordCoreNLPmention();
		this->copyStanfordMention(currentCoreferenceInListToCopy->firstMentionInList, currentCoreferenceInList->firstMentionInList);


		GIAstanfordCoreNLPcoreference* newCoreference = new GIAstanfordCoreNLPcoreference();
		currentCoreferenceInList->next = newCoreference;

		currentCoreferenceInListToCopy = currentCoreferenceInListToCopy->next;
		currentCoreferenceInList = currentCoreferenceInList->next;
	}
}

void GIAsentenceClassClass::copyStanfordMention(GIAstanfordCoreNLPmention* firstMentionInListToCopy, GIAstanfordCoreNLPmention* firstMentionInList)
{
	GIAstanfordCoreNLPmention* currentMentionInListToCopy = firstMentionInListToCopy;
	GIAstanfordCoreNLPmention* currentMentionInList = firstMentionInList;
	while(currentMentionInListToCopy->next != NULL)
	{
		currentMentionInList->representative = currentMentionInListToCopy->representative;
		currentMentionInList->sentence = currentMentionInListToCopy->sentence;
		currentMentionInList->start = currentMentionInListToCopy->start;
		currentMentionInList->end = currentMentionInListToCopy->end;
		currentMentionInList->head = currentMentionInListToCopy->head;


		GIAstanfordCoreNLPmention* newMention = new GIAstanfordCoreNLPmention();
		currentMentionInList->next = newMention;

		currentMentionInListToCopy = currentMentionInListToCopy->next;
		currentMentionInList = currentMentionInList->next;
	}
}

int GIAsentenceClassClass::calculateNumberOfWordsInSentence(const GIAfeature* firstFeatureInList)
{
	int maxNumberOfWordsInSentence = 0;
	const GIAfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		maxNumberOfWordsInSentence++;
		currentFeatureInList = currentFeatureInList->next;
	}
	return maxNumberOfWordsInSentence;
}

#endif






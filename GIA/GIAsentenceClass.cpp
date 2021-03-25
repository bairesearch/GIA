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
 * File Name: GIAsentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3p4a 24-March-2021
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Sentence Class
 * /
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
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	relationGovernorRevertedToOfficialLRPTemp = false;
	relationDependentRevertedToOfficialLRPTemp = false;
	#endif

	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
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

	#ifdef GIA_SEM_REL_TRANSLATOR_COMMON
	sameReferenceSet = false;
	#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
	corpusSpecialRelationGovernorIsQuery = "";
	corpusSpecialRelationDependentIsQuery = "";
	#endif
	#endif

	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	inverseRelation = false;
	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
	inverseRelationSingle = false;
	#endif
	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
	relationTwoWay = false;
	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	inverseRelationTwoWay = false;
	#endif
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	semanticRelationFunctionConditionNewName = "";
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
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	wordWithLRPforNLPonly = "";
	#endif
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	featureRevertedToOfficialLRPTemp = "";
	#endif

	#ifdef GIA_RELEX
	type = "";
	grammar = "";
	#endif

	NER = FEATURE_NER_UNDEFINED;
	stanfordPOS = "";
	#ifdef GIA_STANFORD_CORENLP
	CharacterOffsetBegin = INT_DEFAULT_VALUE;
	CharacterOffsetEnd = INT_DEFAULT_VALUE;
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
	#ifdef LRP_PREPROCESSOR_POS_TAGGER
	GIAposType = 0;	//LRP_PREPROCESSOR_POS_TYPE_UNDEFINED
	#endif
	#ifdef GIA_PREDETERMINERS
	grammaticalPredeterminer = GRAMMATICAL_PREDETERMINER_UNDEFINED;
	#endif
	previousWordInSentenceIsTo = false;

	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;		//#ifdef GIA_SYN_REL_TRANSLATOR_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	mustSetIsConceptBasedOnApposRelation = false;
	isPronounReference = false;
	#endif

	entityDisabled = false;

	#ifdef GIA_SEM_REL_TRANSLATOR
	GIAsemRelTranslatorPOStype = 0;	//ie GIA_SEM_REL_TRANSLATOR_POS_TYPE_UNDEFINED
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

	numberOfWordsInSentence = 0;

	firstRelationInList = new GIArelation();	//auto constructor execution added 23 Feb 2012
	firstFeatureInList = new GIAfeature();	//auto constructor execution added 23 Feb 2012
	#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
	firstRelationInListOriginal = new GIArelation();
	firstFeatureInListOriginal = new GIAfeature();
	#endif
	
	next = NULL;
	previous = NULL;

	isQuestion = false;

	semanticParserSuccessful = false;

	relationshipEntityArtificialIndexCurrent = 0;
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
	
	#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
	if(firstRelationInListOriginal != NULL)
	{
		delete firstRelationInListOriginal;
	}
	if(firstFeatureInListOriginal != NULL)
	{
		delete firstFeatureInListOriginal;
	}
	#endif

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


void GIAsentenceClassClass::backupOriginalNLPsentenceContent(GIAsentence* firstSentenceInList)
{	
	GIAsentence* currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		copyRelations(currentSentenceInList->firstRelationInList, currentSentenceInList->firstRelationInListOriginal);
		copyFeatures(currentSentenceInList->firstFeatureInList, currentSentenceInList->firstFeatureInListOriginal);
		
		currentSentenceInList = currentSentenceInList->next;
	}
}

void GIAsentenceClassClass::copySentences(GIAsentence* firstSentenceInListToCopy, GIAsentence* firstSentenceInList)
{
	GIAsentence* currentSentenceToCopy = firstSentenceInListToCopy;
	GIAsentence* currentSentence = firstSentenceInList;
	while(currentSentenceToCopy->next != NULL)
	{
		copySentence(currentSentenceToCopy, currentSentence);
		
		GIAsentence* newSentence = new GIAsentence();
		currentSentence->next = newSentence;

		currentSentenceToCopy = currentSentenceToCopy->next;
		currentSentence = currentSentence->next;
	}
}

void GIAsentenceClassClass::copySentence(GIAsentence* sentenceToCopy, GIAsentence* newSentence)
{
	*newSentence = *sentenceToCopy;
	
	#ifdef GIA_STANFORD_CORENLP
	newSentence->firstCoreferenceInList = new GIAstanfordCoreNLPcoreference();
	copyStanfordCoreferences(sentenceToCopy->firstCoreferenceInList, newSentence->firstCoreferenceInList);	//changed 21 Sept 2012
	#endif

	newSentence->firstRelationInList = new GIArelation();
	copyRelations(sentenceToCopy->firstRelationInList, newSentence->firstRelationInList);
	newSentence->firstFeatureInList = new GIAfeature();
	copyFeatures(sentenceToCopy->firstFeatureInList, newSentence->firstFeatureInList);
	
	#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
	newSentence->firstRelationInListOriginal = new GIArelation();
	copyRelations(sentenceToCopy->firstRelationInListOriginal, newSentence->firstRelationInListOriginal);
	newSentence->firstFeatureInListOriginal = new GIAfeature();
	copyFeatures(sentenceToCopy->firstFeatureInListOriginal, newSentence->firstFeatureInListOriginal);
	#endif
	
	newSentence->next = NULL;
	newSentence->previous = NULL;
}

void GIAsentenceClassClass::copyRelations(GIArelation* firstRelationInListToCopy, GIArelation* firstRelationInList)
{
	GIArelation* currentRelationToCopy = firstRelationInListToCopy;
	GIArelation* currentRelation = firstRelationInList;
	while(currentRelationToCopy->next != NULL)
	{
		*currentRelation = *currentRelationToCopy;

		GIArelation* newRelation = new GIArelation();
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
		*currentFeature = *currentFeatureToCopy;
		
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
		copyStanfordMention(currentCoreferenceInListToCopy->firstMentionInList, currentCoreferenceInList->firstMentionInList);

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
		*currentMentionInList = *currentMentionInListToCopy;

		GIAstanfordCoreNLPmention* newMention = new GIAstanfordCoreNLPmention();
		currentMentionInList->next = newMention;

		currentMentionInListToCopy = currentMentionInListToCopy->next;
		currentMentionInList = currentMentionInList->next;
	}
}

int GIAsentenceClassClass::calculateNumberOfWordsInSentence(const GIAfeature* firstFeatureInList)
{
	int numberOfWordsInSentence = 0;
	const GIAfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		numberOfWordsInSentence++;
		currentFeatureInList = currentFeatureInList->next;
	}
	return numberOfWordsInSentence;
}

#endif



int GIAsentenceClassClass::getMinIndexOfDynamicallyGeneratedEntity(const GIAsentence* currentSentenceInList) 
{
	return getMinIndexOfDynamicallyGeneratedEntity(currentSentenceInList->numberOfWordsInSentence);
}

int GIAsentenceClassClass::getMaxIndexOfDynamicallyGeneratedEntity(const GIAsentence* currentSentenceInList) 
{
	return getMaxIndexOfDynamicallyGeneratedEntity(currentSentenceInList->numberOfWordsInSentence);
}

int GIAsentenceClassClass::getMinIndexOfDynamicallyGeneratedEntity(const int numberOfWordsInSentence) 
{
	int minIndexOfDynamicallyGeneratedEntity = numberOfWordsInSentence + LRP_NLP_START_ENTITY_INDEX;		//OLD: FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY
	return minIndexOfDynamicallyGeneratedEntity;	
}

int GIAsentenceClassClass::getMaxIndexOfDynamicallyGeneratedEntity(const int numberOfWordsInSentence) 
{
	int maxIndexOfDynamicallyGeneratedEntity = getMinIndexOfDynamicallyGeneratedEntity(numberOfWordsInSentence) + MAX_NUMBER_OF_SPECIAL_WORDS_PER_SENTENCE;
	return maxIndexOfDynamicallyGeneratedEntity;
}

bool GIAsentenceClassClass::relationIndexIsNormal(const int relationIndex) 
{
	//this function is designed to support the creation of semantic dependency relations with special governor/dependent feature indices (<LRP_NLP_START_ENTITY_INDEX, e.g. -2, -1, 0)
	bool relationIndexIsNormal = false;
	if(relationIndex >= LRP_NLP_START_ENTITY_INDEX)	//OLD: (currentRelation->relationDependentIndex > FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
	{
		relationIndexIsNormal = true;
	}
	return relationIndexIsNormal;
}






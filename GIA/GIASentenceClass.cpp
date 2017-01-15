/*******************************************************************************
 *
 * File Name: GIASentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m1a 20-June-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/



#include "GIASentenceClass.h"




#ifdef GIA_USE_STANFORD_CORENLP


StanfordCoreNLPMention::StanfordCoreNLPMention(void)
{	
	representative = false;
	sentence = -1;
	start = -1;
	end = -1;
	head = -1;
	
	next = NULL;
}

StanfordCoreNLPMention::~StanfordCoreNLPMention(void)
{
	if(next != NULL)
	{
		delete next;
	}
}


StanfordCoreNLPCoreference::StanfordCoreNLPCoreference(void)
{	
	firstMentionInList = new StanfordCoreNLPMention();
	
	next = NULL;
}

StanfordCoreNLPCoreference::~StanfordCoreNLPCoreference(void)
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

#ifdef GIA_USE_ADVANCED_REFERENCING
GIAMention::GIAMention(void)
{	
	representative = false;
	idActiveList = -1;
	entityIndex = -1;	//ie, "head"
	entityName = "";
		
	next = NULL;
}

GIAMention::~GIAMention(void)
{	
	if(next != NULL)
	{
		delete next;
	}
}

GIACoreference::GIACoreference(void)
{	
	firstMentionInList = new GIAMention();
	
	next = NULL;
}

GIACoreference::~GIACoreference(void)
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





Relation::Relation(void)
{
	relationType = "";
	relationDependent = "";
	relationDependentIndex = 0;
	relationGovernor = "";
	relationGovernorIndex = 0;
	
	disabled = false;

	#ifdef GIA_USE_RELEX
	subjObjRelationAlreadyAdded = false;	
	#endif

	#ifdef GIA_USE_STANFORD_CORENLP
	prepositionCombinationAlreadyCreatedTemp = false;
	#endif

	#ifdef GIA_USE_ADVANCED_REFERENCING
	auxillaryIndicatesDifferentReferenceSet = false;
	rcmodIndicatesSameReferenceSet = false;
	#endif
			
	next = NULL;
}

Relation::~Relation(void)
{
	if(next != NULL)
	{
		delete next;
	}

}

Feature::Feature(void)
{
	entityIndex = 0;
	word = "";
	lemma = "";
	
	#ifdef GIA_USE_RELEX
	type = "";
	grammar = "";
	#endif

	NER = FEATURE_NER_UNDEFINED; 
	#ifdef GIA_USE_STANFORD_CORENLP
	CharacterOffsetBegin = -1;
	CharacterOffsetEnd = -1;
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
	grammaticalIsProperNoun = false;
	grammaticalGender = GRAMMATICAL_GENDER_UNDEFINED;
	grammaticalIsPronoun = GRAMMATICAL_PRONOUN_UNDEFINED;
	grammaticalWordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	
	isPronounReference = false;
	
	next = NULL;
	previous = NULL;
}

Feature::~Feature(void)
{
	if(next != NULL)
	{
		delete next;
	}

}


Sentence::Sentence(void)
{
	#ifdef GIA_USE_RELEX
	sentenceText = "";
	constituentsText = "";
	featuresText = "";
	relationsText = "";
	linksText = "";
	#endif
	
	#ifdef GIA_USE_STANFORD_CORENLP
	sentenceIndex = -1;
	firstCoreferenceInList = new StanfordCoreNLPCoreference();
	#endif
	
	maxNumberOfWordsInSentence = 0;
	
	firstRelationInList = new Relation();	//auto constructor execution added 23 Feb 2012
	firstFeatureInList = new Feature();	//auto constructor execution added 23 Feb 2012
	
	next = NULL;
	previous = NULL;
	
	isQuestion = false;
}

Sentence::~Sentence(void)
{
	if(firstRelationInList != NULL)
	{
		delete firstRelationInList;
	}
	
	if(firstFeatureInList != NULL)	//added 23 Feb 2012
	{
		delete firstFeatureInList;
	}	

	if(next != NULL)
	{
		delete next;
	}

}


Paragraph::Paragraph(void)
{	
	firstSentenceInList = new Sentence();

	next = NULL;
	previous = NULL;
}

Paragraph::~Paragraph(void)
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

void copySentences(Sentence * sentenceToCopy, Sentence * newSentence)
{
	newSentence->sentenceIndex = sentenceToCopy->sentenceIndex;

	#ifdef GIA_USE_RELEX
	newSentence->sentenceText = sentenceToCopy->sentenceText;
	newSentence->constituentsText = sentenceToCopy->constituentsText;
	newSentence->featuresText = sentenceToCopy->featuresText;
	newSentence->relationsText = sentenceToCopy->relationsText;
	newSentence->linksText = sentenceToCopy->linksText;
	#endif	
	
	#ifdef GIA_USE_STANFORD_CORENLP
	newSentence->firstCoreferenceInList = sentenceToCopy->firstCoreferenceInList;
	#endif

	newSentence->maxNumberOfWordsInSentence = sentenceToCopy->maxNumberOfWordsInSentence;
		
	copyRelations(sentenceToCopy->firstRelationInList, newSentence->firstRelationInList);
	copyFeatures(sentenceToCopy->firstFeatureInList, newSentence->firstFeatureInList);
		
	newSentence->next = sentenceToCopy->next;
	newSentence->previous = sentenceToCopy->previous;
	
	newSentence->isQuestion = sentenceToCopy->isQuestion;
}


void copyRelations(Relation * firstRelationInListToCopy, Relation * firstRelationInList)
{
	Relation * currentRelationToCopy = firstRelationInListToCopy;
	Relation * currentRelation = firstRelationInList;
	while(currentRelationToCopy->next != NULL)
	{

		currentRelation->relationType = currentRelationToCopy->relationType;
		currentRelation->relationDependent = currentRelationToCopy->relationDependent;
		currentRelation->relationDependentIndex = currentRelationToCopy->relationDependentIndex;
		currentRelation->relationGovernor = currentRelationToCopy->relationGovernor;
		currentRelation->relationGovernorIndex = currentRelationToCopy->relationGovernorIndex;

		//cout << "copy relation:" << endl;
		//cout << currentRelation->relationType << "(" << currentRelation->relationGovernor << ", " << currentRelation->relationDependent << ")" << endl;	
						
		Relation * newRelation = new Relation();
		//newRelation->previous = currentRelation;
		currentRelation->next = newRelation;
				
		currentRelationToCopy = currentRelationToCopy->next;
		currentRelation = currentRelation->next;
	}
}

void copyFeatures(Feature * firstFeatureInListToCopy, Feature * firstFeatureInList)
{
	Feature * currentFeatureToCopy = firstFeatureInListToCopy;
	Feature * currentFeature = firstFeatureInList;
	while(currentFeatureToCopy->next != NULL)
	{	

		currentFeature->entityIndex = currentFeatureToCopy->entityIndex;
		currentFeature->word = currentFeatureToCopy->word;
		currentFeature->lemma = currentFeatureToCopy->lemma;
		
		#ifdef GIA_USE_RELEX
		currentFeature->type = currentFeatureToCopy->type;
		currentFeature->grammar = currentFeatureToCopy->grammar;		
		#endif

		currentFeature->NER = currentFeatureToCopy->NER;
		#ifdef GIA_USE_STANFORD_CORENLP
		currentFeature->CharacterOffsetBegin = currentFeatureToCopy->CharacterOffsetBegin;
		currentFeature->CharacterOffsetEnd = currentFeatureToCopy->CharacterOffsetEnd;
		currentFeature->stanfordPOS = currentFeatureToCopy->stanfordPOS;
		currentFeature->NormalizedNER = currentFeatureToCopy->NormalizedNER;
		currentFeature->Timex = currentFeatureToCopy->Timex;		
		#endif
		
		//cout << "copy feature:" << endl;
		//cout << currentFeature->lemma << endl;				
			
		Feature * newFeature = new Feature();
		newFeature->previous = currentFeature;		
		currentFeature->next = newFeature;
		
		currentFeatureToCopy = currentFeatureToCopy->next;
		currentFeature = currentFeature->next;
	}
}





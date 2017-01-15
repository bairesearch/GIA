/*******************************************************************************
 *
 * File Name: GIASentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i8a 10-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/



#include "GIASentenceClass.h"




#ifdef GIA_USE_STANFORD_CORENLP


StanfordCoreNLPMention::StanfordCoreNLPMention(void)
{	
	representative = -1;
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




Relation::Relation(void)
{
	relationType = "";
	relationArgument = "";
	relationArgumentIndex = 0;
	relationFunction = "";
	relationFunctionIndex = 0;
	
	disabled = false;

	#ifdef GIA_USE_RELEX
	subjObjRelationAlreadyAdded = false;	
	#endif

	#ifdef GIA_USE_STANFORD_CORENLP
	prepositionCombinationAlreadyCreatedTemp = false;
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

	#ifdef GIA_USE_STANFORD_CORENLP
	CharacterOffsetBegin = -1;
	CharacterOffsetEnd = -1;
	POS = "";
	NER = "";
	NormalizedNER = "";
	Timex = "";
	#endif
	
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




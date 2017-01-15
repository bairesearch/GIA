/*******************************************************************************
 *
 * File Name: GIASentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1g6c 12-Feb-2012
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 *
 *******************************************************************************/



#include "GIASentenceClass.h"



Relation::Relation(void)
{
	relationType = "";
	relationArgument = "";
	relationArgumentIndex = 0;
	relationFunction = "";
	relationFunctionIndex = 0;
	
	disabled = false;

	subjObjRelationAlreadyAdded = false;	
		
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
	type = "";
	grammar = "";

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
	sentenceText = "";
	constituentsText = "";
	featuresText = "";
	relationsText = "";
	linksText = "";
	
	maxNumberOfWordsInSentence = 0;
	
	firstRelationInList = new Relation();	//added 23 Feb 2012
	firstFeatureInList = new Feature();	//added 23 Feb 2012
	
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

#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS

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

#endif



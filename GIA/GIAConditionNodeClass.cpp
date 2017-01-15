/*******************************************************************************
 *
 * File Name: GIAConditionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
 *
 *******************************************************************************/


#include "GIAConditionNodeClass.h"



	
GIASharedConditionNode::GIASharedConditionNode(void)
{
	initialisedForPrinting = false;
	printed = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	
	conditionName = "";
	confidence = 1.0;

	conditionIsAction = false;
	conditionAction = NULL;
	conditionEntity = NULL;
	parentIsAction = false;
	parentAction = NULL;
	parentProperty = NULL;
		
	/*
	next = NULL;
	*/
}
GIASharedConditionNode::~GIASharedConditionNode(void)
{
	/*
	if(next != NULL)
	{
		delete next;
	}
	*/

}



//~? [eg past, wednesday, etc]
GIATimeConditionNode::GIATimeConditionNode(void)
{
	sharedCondition = new GIASharedConditionNode(); 
	
	tense = GRAMMATICAL_TENSE_UNDEFINED;
	//tense = tenseundefined;
	second = -1.0;
	hour = -1;
	day = dayundefined;
	month = monthundefined;
	year = YEAR_UNDEFINED;
	period = -1;
	totalTimeInSeconds = 0;
	
	/*
	next = NULL;
	*/
}
GIATimeConditionNode::~GIATimeConditionNode(void)
{
	delete sharedCondition;	
	/*
	if(next != NULL)
	{
		delete next;
	}
	*/
}


GIAPropertyConditionNode::GIAPropertyConditionNode(void)
{	
	sharedCondition = new GIASharedConditionNode();
	/*
	next = NULL;
	*/
}
GIAPropertyConditionNode::~GIAPropertyConditionNode(void)
{	
	delete sharedCondition;
	/*
	if(next != NULL)
	{
		delete next;
	}
	*/
}


GIAActionConditionNode::GIAActionConditionNode(void)
{
	sharedCondition = new GIASharedConditionNode();
	/*
	next = NULL;
	*/
}
GIAActionConditionNode::~GIAActionConditionNode(void)
{	
	delete sharedCondition;
	/*
	if(next != NULL)
	{
		delete next;
	}
	*/
}



GIALocationConditionNode::GIALocationConditionNode(void)
{
	sharedCondition = new GIASharedConditionNode();
	/*
	next = NULL;
	*/
}
GIALocationConditionNode::~GIALocationConditionNode(void)
{
	delete sharedCondition;	
	/*
	if(next != NULL)
	{
		delete next;
	}
	*/
}



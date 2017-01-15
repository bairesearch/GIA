/*******************************************************************************
 *
 * File Name: GIAConditionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b7a 01-Sept-11
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
	dayOfWeek = dayundefined;
	dayOfMonth = TIME_DAY_OF_MONTH_UNDEFINED;
	month = TIME_MONTH_UNDEFINED;
	//month = monthundefined;
	year = TIME_YEAR_UNDEFINED;
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

string generateDateTimeConditionName(int dayOfMonth, int month, long year)
{
	//cout << "as3" << endl;
	
	char dayOfMonthStringcharstar[20] = "";
	char monthStringcharstar[20] = "";
	char yearStringcharstar[20] = "";
	
	/*
	cout << "dayOfMonth = " << dayOfMonth << endl;
	cout << "month = " << month << endl;
	cout << "year = " << year << endl;
	*/
	
	if(dayOfMonth != TIME_DAY_OF_MONTH_UNDEFINED)
	{
		sprintf(dayOfMonthStringcharstar, "%d", dayOfMonth);
	}
	if(month != TIME_MONTH_UNDEFINED)
	{
		sprintf(monthStringcharstar, "%d", month);
	}
	if(year != TIME_YEAR_UNDEFINED)
	{
		sprintf(yearStringcharstar, "%d", year);
	}
	string dateTimeConditionName = "";
	
	if(TIME_DATE_DISPLAY_FORMAT == TIME_DATE_DISPLAY_FORMAT_AMERICAN)
	{
		dateTimeConditionName = dateTimeConditionName + monthStringcharstar + "/" + dayOfMonthStringcharstar + "/" + yearStringcharstar;
	}
	else if(TIME_DATE_DISPLAY_FORMAT == TIME_DATE_DISPLAY_FORMAT_STANDARD)
	{
		dateTimeConditionName = dateTimeConditionName + yearStringcharstar + "/" + monthStringcharstar + "/" + dayOfMonthStringcharstar;
	}
	else if(TIME_DATE_DISPLAY_FORMAT == TIME_DATE_DISPLAY_FORMAT_AUSTRALIAN)
	{
		dateTimeConditionName = dateTimeConditionName + dayOfMonthStringcharstar + "/" + monthStringcharstar + "/" + yearStringcharstar;
	}
	return dateTimeConditionName;
	//cout << "as4" << endl;
}





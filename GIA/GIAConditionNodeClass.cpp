/*******************************************************************************
 *
 * File Name: GIAConditionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l5b 03-June-2012
 *
 *******************************************************************************/


#include "GIAConditionNodeClass.h"





//~? [eg past, wednesday, etc]
GIATimeConditionNode::GIATimeConditionNode(void)
{	
	conditionName = "";
	tenseOnlyTimeCondition = false;
	tense = GRAMMATICAL_TENSE_UNDEFINED;
	//tense = tenseundefined;
	second = -1.0;
	hour = -1;
	dayOfWeek = TIME_DAY_OF_WEEK_UNDEFINED;
	dayOfMonth = TIME_DAY_OF_MONTH_UNDEFINED;
	month = TIME_MONTH_UNDEFINED;
	//month = monthundefined;
	year = TIME_YEAR_UNDEFINED;
	period = -1;
	totalTimeInSeconds = 0;
	
	isProgressive = false;
	
	idActiveListReorderdIDforXMLsave = 0;
	printX = 0;
	printY = 0;
}
GIATimeConditionNode::~GIATimeConditionNode(void)
{
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

long calculateTotalTimeInSeconds(int dayOfMonth, int month, long year)
{

}




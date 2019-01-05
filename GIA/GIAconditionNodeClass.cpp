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
 * File Name: GIAconditionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1e 24-April-2018
 * /
 *******************************************************************************/


#include "GIAconditionNodeClass.hpp"

//~? [eg past, wednesday, etc]
GIAtimeConditionNode::GIAtimeConditionNode(void)
{
	conditionName = "";
	tenseOnlyTimeCondition = false;
	tense = GRAMMATICAL_TENSE_UNDEFINED;
	//tense = tenseundefined;
	second = INT_DEFAULT_VALUE;
	hour = INT_DEFAULT_VALUE;
	dayOfWeek = TIME_DAY_OF_WEEK_UNDEFINED;
	dayOfMonth = TIME_DAY_OF_MONTH_UNDEFINED;
	month = TIME_MONTH_UNDEFINED;
	//month = monthundefined;
	year = TIME_YEAR_UNDEFINED;
	period = INT_DEFAULT_VALUE;
	totalTimeInSeconds = 0;

	isProgressive = false;

	idActiveListReorderdIDforXMLsave = 0;
	printX = 0;
	printY = 0;
	printCoordsAlreadyDefined = false;
}
GIAtimeConditionNode::~GIAtimeConditionNode(void)
{
}


string GIAconditionNodeClassClass::generateDateTimeConditionName(const int dayOfMonth, const int month, const int64_t year)
{
	string dayOfMonthString = "";
	string monthString = "";
	string yearString = "";

	/*
	cout << "dayOfMonth = " << dayOfMonth << endl;
	cout << "month = " << month << endl;
	cout << "year = " << year << endl;
	*/

	if(dayOfMonth != TIME_DAY_OF_MONTH_UNDEFINED)
	{
		dayOfMonthString = SHAREDvars.convertIntToString(dayOfMonth);
	}
	if(month != TIME_MONTH_UNDEFINED)
	{
		monthString = SHAREDvars.convertIntToString(month);
	}
	if(year != TIME_YEAR_UNDEFINED)
	{
		yearString = SHAREDvars.convertIntToString(year);
	}
	string dateTimeConditionName = "";

	if(TIME_DATE_DISPLAY_FORMAT == TIME_DATE_DISPLAY_FORMAT_AMERICAN)
	{
		dateTimeConditionName = dateTimeConditionName + monthString + "/" + dayOfMonthString + "/" + yearString;
	}
	else if(TIME_DATE_DISPLAY_FORMAT == TIME_DATE_DISPLAY_FORMAT_STANDARD)
	{
		dateTimeConditionName = dateTimeConditionName + yearString + "/" + monthString + "/" + dayOfMonthString;
	}
	else if(TIME_DATE_DISPLAY_FORMAT == TIME_DATE_DISPLAY_FORMAT_AUSTRALIAN)
	{
		dateTimeConditionName = dateTimeConditionName + dayOfMonthString + "/" + monthString + "/" + yearString;
	}
	return dateTimeConditionName;
}

int64_t calculateTotalTimeInSeconds(int dayOfMonth, int month, int64_t year)
{
	return 0;
}




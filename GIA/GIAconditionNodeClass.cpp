 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAconditionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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




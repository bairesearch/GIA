/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: GIAconditionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1s10d 05-July-2013
 *
 *******************************************************************************/


#include "GIAconditionNodeClass.h"





//~? [eg past, wednesday, etc]
GIAtimeConditionNode::GIAtimeConditionNode(void)
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
	printCoordsAlreadyDefined = false;
}
GIAtimeConditionNode::~GIAtimeConditionNode(void)
{
}


string generateDateTimeConditionName(int dayOfMonth, int month, long year)
{
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
}

long calculateTotalTimeInSeconds(int dayOfMonth, int month, long year)
{
	return 0;
}




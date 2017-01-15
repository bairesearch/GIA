/*******************************************************************************
 *
 * File Name: GIAConditionNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b8b 22-Sept-11
 *
 *******************************************************************************/


#ifndef HEADER_GIA_CONDITION_NODE_CLASS
#define HEADER_GIA_CONDITION_NODE_CLASS


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


class GIAEntityNode;
class GIAActionNode;
class GIAConditionNode;
class GIATimeConditionNode;
/*
class GIALocationConditionNode;
class GIAPropertyConditionNode;
class GIAActionConditionNode;
*/

#define CONDITION_NODE_NUMBER_OF_TYPES (4)
#define CONDITION_NODE_TYPE_UNDEFINED (0)
#define CONDITION_NODE_TYPE_TIME (1)
#define CONDITION_NODE_TYPE_LOCATION (2)
#define CONDITION_NODE_TYPE_ACTION (3)
#define CONDITION_NODE_TYPE_PROPERTY (4)

#define TIME_MONTH_JANUARY "January"
#define TIME_MONTH_FEBRUARY "February"
#define TIME_MONTH_MARCH "March"
#define TIME_MONTH_APRIL "April"
#define TIME_MONTH_MAY "May"
#define TIME_MONTH_JUNE "June"
#define TIME_MONTH_JULY "July"
#define TIME_MONTH_AUGUST "August"
#define TIME_MONTH_SEPTEMBER "September"
#define TIME_MONTH_OCTOBER "October"
#define TIME_MONTH_NOVEMBER "November"
#define TIME_MONTH_DECEMBER "December"
#define TIME_MONTH_NUMBER_OF_TYPES (12)


#define GRAMMATICAL_TENSE_UNDEFINED (0)

#define TIME_YEAR_UNDEFINED (-15000000000)	//before start of universe
#define TIME_DAY_OF_MONTH_UNDEFINED (-1)
#define TIME_MONTH_UNDEFINED (-1)	//OLD: 13th month

class GIAConditionNode
{
public:

	GIAConditionNode(void);
	~GIAConditionNode(void);
	
	bool initialisedForPrinting;	
	bool printed;
	int printX;
	int printY;
	int printXIndex;
	int printYIndex;
	int printTextX;
	int printTextY;
	
	double confidence;
	
	string conditionName;	//for debug purposes only? [record original language text?]
	
	//this MAY NEED to be replaced with a vector of action/property/location/time descriptive node pointers
	bool conditionIsAction;	//ie, strictly speaking, entity not action
	GIAActionNode * conditionAction;		//ie, this condition is defined by a (far off) action node
	GIAEntityNode * conditionEntity;		//ie, this condition [eg where] is defined by a (far off) entity/property node
		//entityWhenTheActionOccured / entityWhereTheActionOccured / etc
			//eg an entity named "tuesday" - this is not important
			//NB this can be a property also [eg the action occured at "a house", or the action occured at "Tom's house"]

	/*OLD;
	vector<GIAEntityNode*> firstProperty/Action/Location/TimeCondition;
	vector<GIAEntityNode*>::iterator firstProperty/Action/Location/TimeConditionIterator;
		//NB this property, condition, would generally be associated with the entity performing the action [or the entity upon which the action is applied?], but not necessarily [requires more thought] - it may depend upon how selfish the entity is
	*/
	
	
	bool parentIsAction;		//or parent is property
	GIAActionNode * parentAction;		//ie, this condition is a condition for the parent action
	GIAEntityNode * parentProperty;		//ie, this condition is a condition for the parent property
	

	int conditionType;	//added 25 Sept 11	
	GIATimeConditionNode * timeConditionNode;		//if conditionType == CONDITION_NODE_TYPE_TIME
	
	long id;
};

/*
class GIAConditionNodeContainer
{
public:

	GIAConditionNodeContainer(void);
	~GIAConditionNodeContainer(void);
	
	//flat tree structures are not used - this is only used for the semanticNet xml parse (read) process;		
	GIAConditionNode * node;
	GIAConditionNodeContainer * next;
	long id;	
	
}
*/



class GIATimeConditionNode
{
public:

	GIATimeConditionNode(void);
	~GIATimeConditionNode(void);
	
	//temporal coordinate system here - this needs to be replaced with a system time (or added to) 
	int tense;
	/*
	enum tense {tenseundefined=GRAMMATICAL_TENSE_UNDEFINED, present=GRAMMATICAL_TENSE_PRESENT, past=GRAMMATICAL_TENSE_PAST, future=GRAMMATICAL_TENSE_FUTURE};
	*/
	/*
	enum
	{
		tenseundefined, present, past, future 
	}tense;
	*/	
	double second;
	int hour;
	enum
	{
		monday, tuesday, wednesday, thursday, friday, saturday, sunday, dayundefined
	}dayOfWeek;
	/*
	enum
	{
		january, february, march, april, may, june, july, august, september, october, november, december, monthundefined
	}month;
	*/
	int month;
	int dayOfMonth;
	long year;	//in years < 0AD is negative
	double period;	//in seconds
	long totalTimeInSeconds;
	
};

#define TIME_DATE_DISPLAY_FORMAT_AMERICAN (1)
#define TIME_DATE_DISPLAY_FORMAT_STANDARD (2)
#define TIME_DATE_DISPLAY_FORMAT_AUSTRALIAN (3)
#define TIME_DATE_DISPLAY_FORMAT (TIME_DATE_DISPLAY_FORMAT_AUSTRALIAN)
string generateDateTimeConditionName(int dayOfMonth, int month, long year);


#endif


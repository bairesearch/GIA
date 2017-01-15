/*******************************************************************************
 *
 * File Name: GIAConditionNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
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
class GIASharedConditionNode;
class GIALocationConditionNode;
class GIATimeConditionNode;
class GIAPropertyConditionNode;
class GIAActionConditionNode;


#define TENSE_UNDEFINED 0
#define TENSE_PRESENT 1
#define TENSE_PAST 2
#define TENSE_FUTURE 3
#define TENSE_NUMBER_OF_TYPES (4)
extern string tenseNameArray[TENSE_NUMBER_OF_TYPES];
extern int tenseNameLengthsArray[TENSE_NUMBER_OF_TYPES];
//extern string * tenseNameArray;
//extern int * tenseNameLengthsArray;


#define YEAR_UNDEFINED (-15000000000)	//before start of universe


class GIASharedConditionNode
{
public:

	GIASharedConditionNode(void);
	~GIASharedConditionNode(void);
	
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

	/*
	vector<GIAEntityNode*> firstProperty/Action/Location/TimeCondition;
	vector<GIAEntityNode*>::iterator firstProperty/Action/Location/TimeConditionIterator;
		//NB this property, condition, would generally be associated with the entity performing the action [or the entity upon which the action is applied?], but not necessarily [requires more thought] - it may depend upon how selfish the entity is
	*/
	
	bool parentIsAction;		//or parent is property
	GIAActionNode * parentAction;		//ie, this condition is a condition for the parent action
	GIAEntityNode * parentProperty;		//ie, this condition is a condition for the parent property
};


class GIATimeConditionNode
{
public:

	GIATimeConditionNode(void);
	~GIATimeConditionNode(void);
	
	//temporal coordinate system here - this needs to be replaced with a system time (or added to) 
	int tense;
	/*
	enum tense {tenseundefined=TENSE_UNDEFINED, present=TENSE_PRESENT, past=TENSE_PAST, future=TENSE_FUTURE};
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
	}day;
	enum
	{
		january, february, march, april, may, june, july, august, september, october, november, december, monthundefined
	}month;
	long year;	//in years < 0AD is negative
	double period;	//in seconds
	long totalTimeInSeconds;
	
	GIASharedConditionNode * sharedCondition;
	
	/*vector used instead
	GIATimeConditionNode * next;
	*/
};


class GIALocationConditionNode
{
public:

	GIALocationConditionNode(void);
	~GIALocationConditionNode(void);
	
	//NEED to include some kind of absolute spatial coordinate system HERE
	GIASharedConditionNode * sharedCondition;
	
	/*vector used instead
	GIALocationConditionNode * next;
	*/
};


class GIAActionConditionNode
{
public:

	GIAActionConditionNode(void);
	~GIAActionConditionNode(void);
	
	//NEED to include some kind of absolute justification/discerment measurement system HERE

	//this MAY NEED to be replaced with a vector of action node pointers
	GIASharedConditionNode * sharedCondition;
	
	/*vector used instead 
	GIAReasonNode * next;
	*/
};


class GIAPropertyConditionNode
{
public:

	GIAPropertyConditionNode(void);
	~GIAPropertyConditionNode(void);
	
	//NEED to include some kind of absolute justification/discerment measurement system HERE
	GIASharedConditionNode * sharedCondition;
	
	/*vector used instead	
	GIAReasonNode * next;
	*/
};


#endif


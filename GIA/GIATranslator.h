/*******************************************************************************
 *
 * File Name: GIATranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR




#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


					
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAActionNodeClass.h"
#include "GIAConditionNodeClass.h"


#define GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME (1)

#define SUBJECT_INDEX (0)
#define OBJECT_INDEX (1)

#define MAX_NUMBER_OF_RELATIONS_PER_SENTENCE (50)
#define MAX_NUMBER_OF_WORDS_PER_SENTENCE (50)
		
#define SECONDS_IN_YEAR (365*24*60*60)

	//NB all of these cases/types need to be replaced with more complex grammar requirements (eg "on" can also mean "rides his bike on the road" [location], not just "rides his bike on tuesday" [time])

//properties (attached to either another property or a straight entity)						
#define RELATION_FUNCTION_COMPOSITION_1 "contains"	//eg x contains y
#define RELATION_FUNCTION_COMPOSITION_2 "comprises"
#define RELATION_FUNCTION_COMPOSITION_3 "has"						
#define RELATION_TYPE_ADJECTIVE "_amod"		//eg x is happy
#define RELATION_TYPE_ADJECTIVE_2 "_predadj"						
#define RELATION_TYPE_ADJECTIVE_3 "_advmod"
#define RELATION_TYPE_POSSESSIVE "_poss"	//eg his bike	[bike him]		/its bike

//straight entities (concepts)					
#define RELATION_FUNCTION_DEFINITION_1 "be"	//eg x is y


#define RELATION_TYPE_OBJECT "_obj"	//eg eats y	[? be y]
#define RELATION_TYPE_OBJECT_TO "to"	//eg to y	//NB "to" is currently interpreted as an object of an action
#define RELATION_TYPE_SUBJECT "_subj"	//eg x eats 	[? be x]

//action/property conditions
#define RELATION_TYPE_ON "on"		//eg rides on tuesday		[ride tuesday]		//this forms the action condition; "when"
#define RELATION_TYPE_AT "at"		//eg rides at the palace	[ride palace]	//this forms the action condition; "where"
#define RELATION_TYPE_WHEN "when"	//eg joe fires his bow when john drives fast.	[fire drive]	//this forms the action condition; "when" [not time, but in association with another action]
#define RELATION_TYPE_BECAUSE "because"

//action/property reasons [???]
#define RELATION_TYPE_SUCH_THAT "such"
#define RELATION_TYPE_SO "so"




/*
noun = joe
verb = rides
adjective = happy
*/

long maximumLong(long a, long b);

void convertSentenceRelationsIntoGIAnetworkNodes(vector<GIAEntityNode*> *indexOfEntityNodes, vector<string> *indexOfEntityNames, vector<GIATimeConditionNode*> *indexOfTimeNodes, vector<long> *indexOfTimeNumbers, Sentence * firstSentenceInList);

GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *indexOfEntityNodes, vector<string> *indexOfEntityNames, string * entityNodeName, bool * found, long * index, bool addIfNonexistant);
GIATimeConditionNode * findOrAddTimeNodeByNumber(vector<GIATimeConditionNode*> *indexOfTimeNodes, vector<long> *indexOfTimeNumbers, long * timeNodeNumber, bool * found, long * index, bool addIfNonexistant);

void addPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity);

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity);

void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);
GIAActionNode * addAction(GIAEntityNode * actionEntity);
void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity);
void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);
void addLocationConditionToAction(GIAActionNode * actionNode, GIAEntityNode * locationConditionEntity);
void addTimeConditionToAction(GIAActionNode * actionNode, GIAEntityNode * timeConditionEntity);
void addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity);
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity);
void addActionConditionToAction(GIAActionNode * actionNode, GIAActionNode * actionConditionActionNode);
void addPropertyConditionToAction(GIAActionNode * actionNode, GIAEntityNode * propertyConditionEntity);
void addActionConditionToProperty(GIAEntityNode * propertyNode, GIAActionNode * actionConditionActionNode);
void addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity);
	//property to property relationship - these they in actual fact represent different levels of detail in information to property to action / action to action nodes - direct property to property relationships are missing the action/connectivity information




long maxLong(long a, long b);

#endif

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


#define SUBJECT_INDEX (0)
#define OBJECT_INDEX (1)

#define GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME (1)
					
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAActionNodeClass.h"
#include "GIAConditionNodeClass.h"

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

/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
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
 * File Name: GIAtranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2f19a 23-July-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_OPERATIONS
#define HEADER_GIA_TRANSLATOR_OPERATIONS

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorDefs.h"










/*
noun = joe
verb = rides
adjective = happy
*/

void initialiseGIATranslatorForTexualContextOperations();

//double check that isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe and isAdjectiveNotConnectedToObjectOrSubject and are no longer required with GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(Relation * currentRelationInList, GIAentityNode * GIAentityNodeArray[], int relationGovernorIndex, int NLPdependencyRelationsType);
bool isAdjectiveNotConnectedToObjectOrSubject(Sentence * currentSentenceInList, Relation * currentRelationInList, int NLPdependencyRelationsType);								//Stanford Compatible
#endif

GIAentityNode * addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(GIAentityNode * thingEntity, GIAentityNode * propertyEntity, bool sameReferenceSet);
	GIAentityNode * connectPropertyToEntity(GIAentityNode * thingEntity, GIAentityNode * propertyEntity, bool sameReferenceSet);
	GIAentityNode * addOrConnectPropertyToEntity(GIAentityNode * thingEntity, GIAentityNode * propertyEntity, bool sameReferenceSet);	//WRONG: Not used anymore
GIAentityNode * addSubstanceToSubstanceDefinition(GIAentityNode * substanceEntity);
	GIAentityNode * addSubstance(GIAentityNode * entity);

void forwardInfoToNewSubstance(GIAentityNode * entity, GIAentityNode * newSubstance);

void addTenseOnlyTimeConditionToSubstance(GIAentityNode * substanceNode, int tense, bool isProgressive);

void addDefinitionToEntity(GIAentityNode * thingEntity, GIAentityNode * definitionEntity, bool sameReferenceSet);

GIAentityNode * addOrConnectActionToEntity(GIAentityNode * subjectEntity, GIAentityNode * objectEntity, GIAentityNode * actionEntity, bool sameReferenceSetSubject, bool sameReferenceSetObject);
GIAentityNode * addOrConnectActionToSubject(GIAentityNode * subjectEntity, GIAentityNode * actionEntity, bool sameReferenceSet);
GIAentityNode * addOrConnectActionToObject(GIAentityNode * objectEntity, GIAentityNode * actionEntity, bool sameReferenceSet);
	void connectActionInstanceToSubject(GIAentityNode * subjectEntity, GIAentityNode * newOrExistingAction, bool sameReferenceSet);
	void connectActionInstanceToObject(GIAentityNode * objectEntity, GIAentityNode * newOrExistingAction, bool sameReferenceSet);
GIAentityNode * addActionToActionDefinition(GIAentityNode * actionEntity);
GIAentityNode * addActionToActionDefinitionDefineSubstances(GIAentityNode * actionEntity);
	GIAentityNode * addAction(GIAentityNode * actionEntity);
	void upgradeSubstanceToAction(GIAentityNode * substance);
		void eraseSubstanceFromSubstanceList(GIAentityNode * existingEntity);

GIAentityNode * addOrConnectConditionToEntity(GIAentityNode * conditionSubjectEntity, GIAentityNode * conditionObjectEntity, GIAentityNode * conditionEntity, bool sameReferenceSet);
GIAentityNode * addOrConnectConditionToSubject(GIAentityNode * conditionSubjectEntity, GIAentityNode * conditionEntity, bool sameReferenceSet);
GIAentityNode * addOrConnectConditionToObject(GIAentityNode * conditionObjectEntity, GIAentityNode * conditionEntity, bool sameReferenceSet);
	void connectConditionInstanceToSubject(GIAentityNode * subjectEntity, GIAentityNode * newOrExistingCondition, bool sameReferenceSet);
	void connectConditionInstanceToObject(GIAentityNode * objectEntity, GIAentityNode * newOrExistingCondition, bool sameReferenceSet);
GIAentityNode * addConditionToConditionDefinition(GIAentityNode * conditionEntity);
	GIAentityNode * addCondition(GIAentityNode * conditionEntity);

#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
GIAentityNode * addOrConnectBeingDefinitionConditionToEntity(GIAentityNode * conditionSubjectEntity, GIAentityNode * conditionDefinitionNode, GIAentityNode * conditionEntity, bool negative, bool sameReferenceSet);
GIAentityNode * addOrConnectHavingPropertyConditionToEntity(GIAentityNode * conditionSubjectEntity, GIAentityNode * conditionSubstanceNode, GIAentityNode * conditionEntity, bool negative, bool sameReferenceSet);
#endif

string convertPrepositionToRelex(string * preposition, bool * prepositionFound);	//converts prep_preposition to preposition



void setTranslatorEntityNodesCompleteList(vector<GIAentityNode*> * newEntityNodesCompleteList);
//void setTranslatorConceptEntityNodesList(vector<GIAentityNode*> * newConceptEntityNodesList);
void setTranslatorSubstanceEntityNodesList(vector<GIAentityNode*> * newSubstanceEntityNodesList);
void setTranslatorActionEntityNodesList(vector<GIAentityNode*> * newActionEntityNodesList);
void setTranslatorConditionEntityNodesList(vector<GIAentityNode*> * newConditionEntityNodesList);

vector<GIAentityNode*> * getTranslatorEntityNodesCompleteList();
//vector<GIAentityNode*> * getTranslatorConceptEntityNodesList();
vector<GIAentityNode*> * getTranslatorSubstanceEntityNodesList();
vector<GIAentityNode*> * getTranslatorActionEntityNodesList();
vector<GIAentityNode*> * getTranslatorConditionEntityNodesList();

void setSaveNetwork(bool val);
bool getSaveNetwork();
#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
void setLinkPreestablishedReferencesGIA(bool val);
bool getLinkPreestablishedReferencesGIA();
#endif

bool getFoundComparisonVariable();
GIAentityNode* getComparisonVariableNode();
void setFoundComparisonVariable(bool value);
void setComparisonVariableNode(GIAentityNode* newComparisonVariableNode);

long * getCurrentEntityNodeIDinCompleteList();
long * getCurrentEntityNodeIDinConceptEntityNodesList();
long * getCurrentEntityNodeIDinSubstanceEntityNodesList();
long * getCurrentEntityNodeIDinActionEntityNodesList();
long * getCurrentEntityNodeIDinConditionEntityNodesList();

long * getCurrentEntityNodeIDinSentenceCompleteList();
long * getCurrentEntityNodeIDinSentenceConceptEntityNodesList();

#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
int getCurrentSentenceIndex();
void setCurrentSentenceIndex(int value);
#endif

void applyConceptEntityAlreadyExistsFunction(GIAentityNode * entity, bool entityAlreadyExistant, bool tempEntityEnabled);
void disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode * entity);
void disableEntity(GIAentityNode * entity);
void disableConceptEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode * entity);	//not used
void disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode * entity);	//not used

void recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap);
void recordConceptNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap);
void recordConceptNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap);

void convertRelexPOStypeToWordnetWordType(string * relexPOStype, int * grammaticalWordTypeTemp);
void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(string * POStag, string * relexPOStype, int * grammaticalWordTypeTemp);

void generateTempFeatureArray(Feature * firstFeatureInList, Feature * featureArrayTemp[]);	//used for intrafunction memory allocation purposes only

#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
//bool checkEntityHasSubstanceThatWasDeclaredInContextAndIsUnique(GIAentityNode * entityNode, int entityIndexTemp, int sentenceIndexTemp);		//NOT REQUIRED: redundant: this unique check is redundant considering if a concept entity has a substance that was declared in the immediate context, ie sentence, then the entity node being queried will be the substance itself (and not its concept)	//added 1j8a 10 May 2012
bool checkEntityHasSubstanceThatWasDeclaredInContext(GIAentityNode * entityNode);			//current textual context (eg current paragraph) 	//added 1j7d 9 May 2012
GIAentityNode * getEntitySubstanceThatWasDeclaredInContext(GIAentityNode * entityNode);			//current textual context (eg current paragraph) 	//added 1j7g 9 May 2012
#endif

#ifdef GIA_USE_ADVANCED_REFERENCING
bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, Relation * relation);
#endif

GIAentityNode * findOrAddEntityNodeByNameSimpleWrapperCondition(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int featureIndex, string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);
	GIAentityNode * findOrAddConceptEntityNodeByNameSimpleWrapper(string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);
		GIAentityNode * findOrAddConceptEntityNodeByNameSimpleWrapper(string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, bool tempEntityEnabled);

GIAentityConnection * writeVectorConnection(GIAentityNode * entityNode, GIAentityNode * entityNodeToAdd, int connectionType, bool sameReferenceSet);
	GIAentityConnection * findEntityNodePointerInVector(GIAentityNode * entityNode, GIAentityNode * entityNodeToFind, int connectionType, bool * foundNode);
	GIAentityConnection * findEntityNodeNameInVector(GIAentityNode * entityNode, string * entityNodeNameToFind, int connectionType, bool * foundNode);

long determineNextIdInstance(GIAentityNode * entity);

#ifdef GIA_USE_DATABASE
/*//replaced with optimised function findEntityNodesActiveListCompleteFastIndexDBactive()
bool entityInActiveListComplete(string * entityName, long idInstance);
bool entityInActiveListComplete(GIAentityNode * entity);
*/
void addInstanceEntityNodeToActiveLists(GIAentityNode * entity);
#endif

#ifdef GIA_SUPPORT_ALIASES
void mergeEntityNodesAddAlias(GIAentityNode * entityNode, GIAentityNode * entityNodeToMerge);
#endif

#endif

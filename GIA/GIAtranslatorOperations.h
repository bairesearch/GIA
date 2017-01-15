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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p3a 14-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_OPERATIONS
#define HEADER_GIA_TRANSLATOR_OPERATIONS

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorDefs.h"
#ifdef GIA_NLC_INTEGRATION
#include "NLCpreprocessorSentenceClass.h"
#endif









/*
noun = joe
verb = rides
adjective = happy
*/

void initialiseGIATranslatorForTexualContextOperations();

//double check that isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe and isAdjectiveNotConnectedToObjectOrSubject and are no longer required with GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(const GIArelation* currentRelationInList, GIAentityNode* GIAentityNodeArray[], int relationGovernorIndex, const int NLPdependencyRelationsType);
bool isAdjectiveNotConnectedToObjectOrSubject(const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInList, const int NLPdependencyRelationsType);								//Stanford Compatible
#endif

GIAentityNode* addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet);
	GIAentityNode* connectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet);
		GIAentityNode* connectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet, const GIAentityNode* auxHaveEntity);
	GIAentityNode* addOrConnectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet);
		GIAentityNode* addOrConnectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet, const GIAentityNode* auxHaveEntity);

GIAentityNode* addInstanceToInstanceDefinition(GIAentityNode* entity, int instanceType);
	GIAentityNode* addInstance(GIAentityNode* entity, const int instanceType);

void forwardInfoToNewSubstance(GIAentityNode* entity, GIAentityNode* newSubstance);

void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, const int tense, const bool isProgressive);

void addDefinitionToEntity(GIAentityNode* thingEntity, GIAentityNode* definitionEntity, bool sameReferenceSet);
#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
void addDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* thingEntity, GIAentityNode* definitionEntity, bool sameReferenceSet);
#endif

GIAentityNode* addOrConnectActionToEntity(GIAentityNode* subjectEntity, GIAentityNode* objectEntity, GIAentityNode* actionEntity, bool sameReferenceSet);
GIAentityNode* addOrConnectActionToSubject(GIAentityNode* subjectEntity, GIAentityNode* actionEntity, bool sameReferenceSet);
GIAentityNode* addOrConnectActionToObject(GIAentityNode* objectEntity, GIAentityNode* actionEntity, bool sameReferenceSet);
	void connectActionInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingAction, bool sameReferenceSet);
	void connectActionInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingAction, bool sameReferenceSet);

GIAentityNode* addOrConnectConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionObjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet);
GIAentityNode* addOrConnectConditionToSubject(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet);
GIAentityNode* addOrConnectConditionToObject(GIAentityNode* conditionObjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet);
	void connectConditionInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingCondition, bool sameReferenceSet);
	void connectConditionInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingCondition, bool sameReferenceSet);

#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
GIAentityNode* addOrConnectBeingDefinitionConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionDefinitionNode, GIAentityNode* conditionEntity, bool negative, bool sameReferenceSet);
GIAentityNode* addOrConnectHavingPropertyConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionSubstanceNode, GIAentityNode* conditionEntity, bool negative, bool sameReferenceSet);
#endif

string convertPrepositionToRelex(const string* preposition, bool* prepositionFound);	//converts prep_preposition to preposition



void setTranslatorEntityNodesCompleteList(vector<GIAentityNode*>* newEntityNodesCompleteList);
//void setTranslatorNetworkIndexEntityNodesList(vector<GIAentityNode*>* newNetworkIndexEntityNodesList);

vector<GIAentityNode*>* getTranslatorEntityNodesCompleteList();
//vector<GIAentityNode*>* getTranslatorNetworkIndexEntityNodesList();

void setSaveNetwork(const bool val);
bool getSaveNetwork();
#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
void setLinkPreestablishedReferencesGIA(const bool val);
bool getLinkPreestablishedReferencesGIA();
#endif

bool getFoundComparisonVariable();
GIAentityNode* getComparisonVariableNode();
void setFoundComparisonVariable(const bool value);
void setComparisonVariableNode(GIAentityNode* newComparisonVariableNode);

long* getCurrentEntityNodeIDinCompleteList();
long* getCurrentEntityNodeIDinNetworkIndexEntityNodesList();

long* getCurrentEntityNodeIDinSentenceCompleteList();
long* getCurrentEntityNodeIDinSentenceNetworkIndexEntityNodesList();

#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
int getCurrentSentenceIndex();
void setCurrentSentenceIndex(const int value);
#endif

void applyNetworkIndexEntityAlreadyExistsFunction(GIAentityNode* entity, const bool entityAlreadyExistant, const bool tempEntityEnabled);
void disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);
void disableEntity(GIAentityNode* entity);
void disableNetworkIndexEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);	//not used
void disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);	//not used

void recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
void recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
void recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);

void convertRelexPOStypeToWordnetWordType(const string* relexPOStype, int* grammaticalWordTypeTemp);
void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(const string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp);

void generateTempFeatureArray(GIAfeature* firstFeatureInList, GIAfeature* featureArrayTemp[]);	//used for intrafunction memory allocation purposes only

#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
//bool checkEntityHasSubstanceThatWasDeclaredInContextAndIsUnique(GIAentityNode* entityNode, int entityIndexTemp, int sentenceIndexTemp);		//NOT REQUIRED: redundant: this unique check is redundant considering if a networkIndex entity has a substance that was declared in the immediate context, ie sentence, then the entity node being queried will be the substance itself (and not its networkIndex)	//added 1j8a 10 May 2012
bool checkEntityHasSubstanceThatWasDeclaredInContext(const GIAentityNode* entityNode);			//current textual context (eg current paragraph) 	//added 1j7d 9 May 2012
GIAentityNode* getEntitySubstanceThatWasDeclaredInContext(GIAentityNode* entityNode);			//current textual context (eg current paragraph) 	//added 1j7g 9 May 2012
#endif

#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, const GIArelation* relation);
#endif

GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperCondition(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int featureIndex, string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);
	GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);
		GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const bool tempEntityEnabled);

GIAentityConnection* writeVectorConnection(GIAentityNode* entityNode, GIAentityNode* entityNodeToAdd, const int connectionType, bool sameReferenceSet);
	GIAentityConnection* findEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, bool* foundNode);
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE
	GIAentityConnection* findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, bool* foundNode, const int sentenceIndex);
	#endif
	GIAentityConnection* findEntityNodeNameInVector(GIAentityNode* entityNode, const string* entityNodeNameToFind, const int connectionType, bool* foundNode);

long determineNextIdInstance(GIAentityNode* entity);

#ifdef GIA_DATABASE
/*//replaced with optimised function findEntityNodesActiveListCompleteFastIndexDBactive()
bool entityInActiveListComplete(string* entityName, long idInstance);
bool entityInActiveListComplete(GIAentityNode* entity);
*/
void addInstanceEntityNodeToActiveLists(GIAentityNode* entity);
#endif

#ifdef GIA_ALIASES
void mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge);
#endif

GIAentityNode* getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNode* instanceEntity);

#ifdef GIA_NLC_INTEGRATION
NLCsentence* getFirstNLCsentenceInListGIA();
void setFirstNLCsentenceInListGIA(NLCsentence* firstNLCsentenceInListNew);
#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
bool checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA(const GIAentityNode* indefiniteEntity, const GIAentityNode* definiteEntity, int* indentationDifferenceFound);
#endif
#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
bool checkIfSentenceIsMathTextParsablePhrase(const GIAsentence* currentSentenceInList);
#endif
#endif

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
#ifdef GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES
GIAentityNode* createNewNonspecificConcept(GIAentityNode* networkIndexEntity);
GIAentityNode* getNonspecificConceptEntityFromNetworkIndex(GIAentityNode* networkIndexEntity);
GIAentityNode* getNonspecificConceptEntityFromInstance(GIAentityNode* instanceEntity);
	bool isNonspecificConceptEntity(GIAentityNode* entity);
#endif
#endif

#endif

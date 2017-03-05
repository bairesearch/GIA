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
 * File Name: GIAtranslatorOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4c 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_OPERATIONS
#define HEADER_GIA_TRANSLATOR_OPERATIONS

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorDefs.hpp"
#ifdef GIA_NLC_INTEGRATION
#include "NLCpreprocessorSentenceClass.hpp"
#endif
#include "GIAdatabase.hpp"
#include "GIAlrp.hpp"
#include "GIAsemanticParserOperations.hpp"










/*
noun = joe
verb = rides
adjective = happy
*/

class GIAtranslatorOperationsClass
{
	private: GIAlrpClass GIAlrp;
	private: GIAdatabaseClass GIAdatabase;
	public: void initialiseGIATranslatorForTexualContextOperations();

//double check that isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe and isAdjectiveNotConnectedToObjectOrSubject and are no longer required with GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	public: bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(const GIArelation* currentRelationInList, GIAentityNode* GIAentityNodeArray[], int relationGovernorIndex, const int NLPdependencyRelationsType);
	public: bool isAdjectiveNotConnectedToObjectOrSubject(const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInList, const int NLPdependencyRelationsType);								//Stanford Compatible
#endif

	public: GIAentityNode* addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet);
		private: GIAentityNode* connectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet);
			private: GIAentityNode* connectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet, const GIAentityNode* auxHaveEntity);
		public: GIAentityNode* addOrConnectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet);
			public: GIAentityNode* addOrConnectPropertyToEntity(GIAentityNode* thingEntity, GIAentityNode* propertyEntity, bool sameReferenceSet, const GIAentityNode* auxHaveEntity);

	public: GIAentityNode* addInstanceToInstanceDefinition(GIAentityNode* entity, int instanceType);
		private: GIAentityNode* addInstance(GIAentityNode* entity, const int instanceType);

	private: void forwardInfoToNewSubstance(GIAentityNode* entity, GIAentityNode* newSubstance);

	public: void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, const int tense, const bool isProgressive);

	public: void addDefinitionToEntity(GIAentityNode* thingEntity, GIAentityNode* definitionEntity, bool sameReferenceSet);
#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: void addDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* thingEntity, GIAentityNode* definitionEntity, bool sameReferenceSet);
#endif

	public: GIAentityNode* addOrConnectActionToEntity(GIAentityNode* subjectEntity, GIAentityNode* objectEntity, GIAentityNode* actionEntity, bool sameReferenceSet);
	public: GIAentityNode* addOrConnectActionToSubject(GIAentityNode* subjectEntity, GIAentityNode* actionEntity, bool sameReferenceSet);
	public: GIAentityNode* addOrConnectActionToObject(GIAentityNode* objectEntity, GIAentityNode* actionEntity, bool sameReferenceSet);
		private: void connectActionInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingAction, bool sameReferenceSet);
		private: void connectActionInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingAction, bool sameReferenceSet);

	public: GIAentityNode* addOrConnectConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionObjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet);
	public: GIAentityNode* addOrConnectConditionToSubject(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet);
	public: GIAentityNode* addOrConnectConditionToObject(GIAentityNode* conditionObjectEntity, GIAentityNode* conditionEntity, bool sameReferenceSet);
		public: void connectConditionInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingCondition, bool sameReferenceSet);
		public: void connectConditionInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingCondition, bool sameReferenceSet);

#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	public: GIAentityNode* addOrConnectBeingDefinitionConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionDefinitionNode, GIAentityNode* conditionEntity, bool negative, bool sameReferenceSet);
	public: GIAentityNode* addOrConnectHavingPropertyConditionToEntity(GIAentityNode* conditionSubjectEntity, GIAentityNode* conditionSubstanceNode, GIAentityNode* conditionEntity, bool negative, bool sameReferenceSet);
#endif

	public: string convertPrepositionToRelex(const string* preposition, bool* prepositionFound);	//converts prep_preposition to preposition



	public: void setTranslatorEntityNodesCompleteList(vector<GIAentityNode*>* newEntityNodesCompleteList);
//void setTranslatorNetworkIndexEntityNodesList(vector<GIAentityNode*>* newNetworkIndexEntityNodesList);

	public: vector<GIAentityNode*>* getTranslatorEntityNodesCompleteList();
//vector<GIAentityNode*>* getTranslatorNetworkIndexEntityNodesList();

	public: void setSaveNetwork(const bool val);
	public: bool getSaveNetwork();
#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
	public: void setLinkPreestablishedReferencesGIA(const bool val);
	private: bool getLinkPreestablishedReferencesGIA();
#endif

	public: bool getFoundComparisonVariable();
	public: GIAentityNode* getComparisonVariableNode();
	public: void setFoundComparisonVariable(const bool value);
	public: void setComparisonVariableNode(GIAentityNode* newComparisonVariableNode);

	public: long* getCurrentEntityNodeIDinCompleteList();
	public: long* getCurrentEntityNodeIDinNetworkIndexEntityNodesList();

	public: long* getCurrentEntityNodeIDinSentenceCompleteList();
	public: long* getCurrentEntityNodeIDinSentenceNetworkIndexEntityNodesList();

#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
	private: int getCurrentSentenceIndex();
	public: void setCurrentSentenceIndex(const int value);
#endif

	public: void applyNetworkIndexEntityAlreadyExistsFunction(GIAentityNode* entity, const bool entityAlreadyExistant, const bool tempEntityEnabled);
	public: void disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);
	public: void disableEntity(GIAentityNode* entity);
	private: void disableNetworkIndexEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);	//not used
	public: void disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);	//not used

	public: void recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);

	public: void convertRelexPOStypeToWordnetWordType(const string* relexPOStype, int* grammaticalWordTypeTemp);
	public: void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(const string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp);

	public: void generateTempFeatureArray(GIAfeature* firstFeatureInList, GIAfeature* featureArrayTemp[]);	//used for intrafunction memory allocation purposes only

#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
//bool checkEntityHasSubstanceThatWasDeclaredInContextAndIsUnique(GIAentityNode* entityNode, int entityIndexTemp, int sentenceIndexTemp);		//NOT REQUIRED: redundant: this unique check is redundant considering if a networkIndex entity has a substance that was declared in the immediate context, ie sentence, then the entity node being queried will be the substance itself (and not its networkIndex)	//added 1j8a 10 May 2012
	private: bool checkEntityHasSubstanceThatWasDeclaredInContext(const GIAentityNode* entityNode);			//current textual context (eg current paragraph) 	//added 1j7d 9 May 2012
	private: GIAentityNode* getEntitySubstanceThatWasDeclaredInContext(GIAentityNode* entityNode);			//current textual context (eg current paragraph) 	//added 1j7g 9 May 2012
#endif

#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	public: bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, const GIArelation* relation);
#endif

	public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperCondition(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int featureIndex, string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);
		public: GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);
			public: GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const bool tempEntityEnabled);

	public: GIAentityConnection* writeVectorConnection(GIAentityNode* entityNode, GIAentityNode* entityNodeToAdd, const int connectionType, bool sameReferenceSet);
		public: GIAentityConnection* findEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, bool* foundNode);
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE
		private: GIAentityConnection* findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, bool* foundNode, const int sentenceIndex);
	#endif
		private: GIAentityConnection* findEntityNodeNameInVector(GIAentityNode* entityNode, const string* entityNodeNameToFind, const int connectionType, bool* foundNode);

	private: long determineNextIdInstance(GIAentityNode* entity);

#ifdef GIA_DATABASE
/*//replaced with optimised function findEntityNodesActiveListCompleteFastIndexDBactive()
bool entityInActiveListComplete(string* entityName, long idInstance);
bool entityInActiveListComplete(GIAentityNode* entity);
*/
	public: void addInstanceEntityNodeToActiveLists(GIAentityNode* entity);
#endif

#ifdef GIA_ALIASES
	public: void mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge);
#endif

	public: GIAentityNode* getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNode* instanceEntity);

#ifdef GIA_NLC_INTEGRATION
	private: NLCsentence* getFirstNLCsentenceInListGIA();
	public: void setFirstNLCsentenceInListGIA(NLCsentence* firstNLCsentenceInListNew);
#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
	public: bool checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA(const GIAentityNode* indefiniteEntity, const GIAentityNode* definiteEntity, int* indentationDifferenceFound);
#endif
#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	public: bool checkIfSentenceIsMathTextParsablePhrase(const GIAsentence* currentSentenceInList);
#endif
#endif

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
#ifdef GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES
	public: GIAentityNode* createNewNonspecificConcept(GIAentityNode* networkIndexEntity);
	public: GIAentityNode* getNonspecificConceptEntityFromNetworkIndex(GIAentityNode* networkIndexEntity);
	public: GIAentityNode* getNonspecificConceptEntityFromInstance(GIAentityNode* instanceEntity);
		private: bool isNonspecificConceptEntity(GIAentityNode* entity);
#endif
#endif
};


#endif

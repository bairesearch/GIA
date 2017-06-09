/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * Project Version: 3c1c 01-June-2017
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
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
#include "GIApreprocessorMultiwordReduction.hpp"
#endif
#ifdef GIA_PREPROCESSOR_SENTENCE
#include "GIApreprocessorSentenceClass.hpp"
#endif






class GIAtranslatorVariablesClass
{
public:

	GIAtranslatorVariablesClass(void);
	~GIAtranslatorVariablesClass(void);
	
	//bool parsedSentences;	//NOT USED; designed to distingush between when parsing a specific sentence and pre/post processing stage (ie whether 
	
	//nlp variables;
	bool isQuery;
	bool NLPrelexCompatibilityMode;
	int maxNumberSentences;

	//nlp/sentence variables;
	int NLPfeatureParser;
	int NLPdependencyRelationsParser;
	int NLPdependencyRelationsType;
	bool NLPassumePreCollapsedStanfordRelations;
	#ifdef GIA_NLP_CLIENT_SERVER
	bool NLPclient;
	#endif
	bool parseGIA2file;

	//network variables;
	unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes;
	vector<GIAentityNode*>* entityNodesActiveListComplete;
	long currentEntityNodeIDInNetworkIndexEntityNodesList;
	long currentEntityNodeIDInCompleteList;
	map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences;
	const unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList;

	GIAparagraph* firstParagraphInList;
	GIAsentence* firstSentenceInList;
	
	//temporary sentence parsing variables (!parsedSentences only):
	bool saveNetwork;
	GIAsentence* currentSentenceInList;
	vector<bool>* GIAentityNodeArrayFilled;
	vector<GIAentityNode*>* GIAnetworkIndexNodeArray;
	vector<GIAentityNode*>* GIAfeatureTempEntityNodeArray;
	vector<GIAentityNode*>* GIAentityNodeArray;
	
	vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList;
	unordered_map<long, GIAtimeConditionNode*>* sentenceTimeConditionNodesList;
	
	vector<GIAfeature*>* featureArrayTemp;	//only used at start of translator

	int sentenceIndex;	//if parsedSentences only
	
	#ifdef GIA_PREPROCESSOR
	#ifdef USE_NLC
	GIApreprocessorSentence* firstNLCpreprepreprocessorSentenceInList;
	#endif
	GIApreprocessorSentence* firstGIApreprocessorSentenceInList;
	#endif
	#ifdef USE_NLCI
	string nlcGeneratedCode;
	#endif
	
	#ifdef USE_CE	
	CECodeextension* firstCodeextensionInHeirachy;
	vector<CECodeextension*>* codeextensionsList;
	bool useCodeextensionsHeirachy;
	#endif
	
};


/*
noun = joe
verb = rides
adjective = happy
*/

class GIAtranslatorOperationsClass
{
	private: GIAsentenceClassClass GIAsentenceClass;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	#endif
	private: GIAdatabaseClass GIAdatabase;
	#ifdef GIA_NLC_INTEGRATION
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	private: NLCpreprocessorSentenceClassClass NLCpreprocessorSentenceClass;
	#endif
	#endif
	private: GIAentityNodeClassClass GIAentityNodeClass;

	
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: bool connectionIsAlias(const GIAentityConnection* connection);
	#endif
	public: bool connectionIsRelationship(GIAentityConnection* connection);
	public: int generateConnectionType(int relationshipEntityType);
	public: int generateConnectionTypeReverse(int relationshipEntityType);	
	public: int generateInvertedConnectionType(GIAentityNode* entity, const int connectionType);	
	
	public: GIAentityNode* getPropertyRelationshipTargetEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getDefinitionRelationshipTargetEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getPropertyRelationshipObjectEntity(GIAentityConnection* connection);	
	public: GIAentityNode* getPropertyRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
	public: GIAentityNode* getDefinitionRelationshipObjectEntity(GIAentityConnection* connection);
	public: GIAentityNode* getDefinitionRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
		public: GIAentityNode* getRelationshipObjectEntity(GIAentityConnection* connection);
		GIAentityNode* getRelationshipObjectEntity(GIAentityNode* relationshipEntity);
		public: GIAentityNode* getRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
		GIAentityNode* getRelationshipSubjectEntity(GIAentityNode* relationshipEntity);

		
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
	public: bool connectPropertyToEntity(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDefinitionToEntity(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: bool connectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	#endif
	public: bool connectBeingDefinitionToEntity(GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectHavingPropertyToEntity(GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	#else
	public: bool connectDirectPropertyToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* auxHaveEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDirectDefinitionToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* auxHaveEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDirectPropertyToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDirectDefinitionToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: bool connectDirectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	#endif
	public: bool connectDirectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* auxHaveEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables, bool markConnectionAsAlias);
	#endif

	public: bool connectActionToEntity(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectActionToSubject(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectActionToObject(GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectConditionToEntity(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectConditionToSubject(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectConditionToObject(GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);

	
	private: bool connectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	private: bool connectRelationshipToSubject(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	private: bool connectRelationshipToObject(GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	private: GIAentityNode* useExistingRelationshipIfExistant(GIAentityNode* newOrExistingRelationship, GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, const int relationshipEntityType);
	#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
	private: void setRelationshipObjectToSubstanceIfNecessary(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType);
	#endif
	private: void connectRelationshipInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingRelationship, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	private: void connectRelationshipInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingRelationship, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	public: void connectEntities(GIAentityNode* entity1, GIAentityNode* entity2, const int connectionType, const int connectionTypeInverse, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);


	//double check that isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe and isAdjectiveNotConnectedToObjectOrSubject and are no longer required with GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	public: bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(const GIArelation* currentRelationInList, GIAentityNode* GIAentityNodeArray[], int relationGovernorIndex, const int NLPdependencyRelationsType);
	public: bool isAdjectiveNotConnectedToObjectOrSubject(const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInList, const int NLPdependencyRelationsType);								//Stanford Compatible
	#endif
	
	public: GIAentityNode* addInstanceToInstanceDefinition(GIAentityNode* entity, int instanceType, GIAtranslatorVariablesClass* translatorVariables);
		private: GIAentityNode* addInstance(GIAentityNode* entity, const int instanceType, GIAtranslatorVariablesClass* translatorVariables);
	private: void forwardInfoToNewSubstance(GIAentityNode* entity, GIAentityNode* newSubstance);
	public: void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, const int tense, const bool isProgressive);


	public: string convertPrepositionToRelex(const string* preposition, bool* prepositionFound);	//converts prep_preposition to preposition

	public: bool getFoundComparisonVariable();
	public: GIAentityNode* getComparisonVariableNode();
	public: void setFoundComparisonVariable(const bool value);
	public: void setComparisonVariableNode(GIAentityNode* newComparisonVariableNode);

	public: void applyNetworkIndexEntityAlreadyExistsFunction(GIAentityNode* entity, const bool entityAlreadyExistant, const bool tempEntityEnabled);
	public: void disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);
	public: void disableEntity(GIAentityNode* entity);
	private: void disableNetworkIndexEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);	//not used
	public: void disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);
	public: void disableInstanceAndNetworkIndexEntity(GIAentityNode* entity);

	public: void recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);

	public: void convertRelexPOStypeToWordnetWordType(const string* relexPOStype, int* grammaticalWordTypeTemp);
	public: void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(const string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp);

	public: void generateTempFeatureArray(GIAtranslatorVariablesClass* translatorVariables, vector<GIAfeature*>* featureArrayTemp);	//used for intrafunction memory allocation purposes only

	public: bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, const GIArelation* relation);

	public: GIAentityNode* addPropertyRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* addDefinitionRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* addConditionRelationshipArtificialToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const string conditionName, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
		public: GIAentityNode* addRelationshipArtificialToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipObjectEntity, GIAtranslatorVariablesClass* translatorVariables);
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAtranslatorVariablesClass* translatorVariables);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(GIAtranslatorVariablesClass* translatorVariables);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(GIAtranslatorVariablesClass* translatorVariables);
			#endif
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
				public: bool findExistingRelationshipInSentenceEntityArray(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, GIAentityNode** relationshipEntity, GIAtranslatorVariablesClass* translatorVariables);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificial(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
					public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(int featureIndex, const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables);
					public: GIAentityNode* findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship(int featureIndex, const string* entityNodeName, GIAtranslatorVariablesClass* translatorVariables);

	public: GIAentityNode* addPropertyRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
	public: GIAentityNode* addDefinitionRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
		public: GIAentityNode* addRelationshipArtificialToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificial2(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
					public: GIAentityNode* findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship2(const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
			#endif
			
	public: GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables);
		public: GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables, const bool tempEntityEnabled);

	public: GIAentityConnection* writeVectorConnection(GIAentityNode* entityNode, GIAentityNode* entityNodeToAdd, const int connectionType, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
		public: bool findEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, GIAentityConnection** connectionFound);
		public: bool findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex);
		private: bool findEntityNodeNameInVector(GIAentityNode* entityNode, const string* entityNodeNameToFind, const int connectionType, GIAentityConnection** connectionFound);

	private: long determineNextIdInstance(GIAentityNode* entity);

	#ifdef GIA_DATABASE
	/*//replaced with optimised function findEntityNodesActiveListCompleteFastIndexDBactive()
	bool entityInActiveListComplete(string* entityName, long idInstance);
	bool entityInActiveListComplete(GIAentityNode* entity);
	*/
	public: void addInstanceEntityNodeToActiveLists(GIAentityNode* entity, GIAtranslatorVariablesClass* translatorVariables);
	#endif

	#ifdef GIA_ALIASES
	public: bool mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge, GIAtranslatorVariablesClass* translatorVariables);
	#endif

	public: GIAentityNode* getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNode* instanceEntity);

	#ifdef GIA_NLC_INTEGRATION
	private: NLCpreprocessorSentence* getFirstNLCsentenceInListGIA();
	public: void setFirstNLCsentenceInListGIA(NLCpreprocessorSentence* firstNLCsentenceInListNew);
	#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
	public: bool checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA(const GIAentityNode* indefiniteEntity, const GIAentityNode* definiteEntity, int* indentationDifferenceFound);
	#endif
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	public: bool checkIfSentenceIsMathTextParsablePhrase(const GIAsentence* currentSentenceInList);
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	public: GIAentityNode* createNewNonspecificConcept(GIAentityNode* networkIndexEntity, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* getNonspecificConceptEntityFromNetworkIndex(GIAentityNode* networkIndexEntity);
	public: GIAentityNode* getNonspecificConceptEntityFromInstance(GIAentityNode* instanceEntity);
		private: bool isNonspecificConceptEntity(GIAentityNode* entity);
	#endif
	
	public: void printEntity(GIAentityNode* entity);
};


#endif

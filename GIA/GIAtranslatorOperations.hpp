/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslatorOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3p6b 07-August-2021
 * Requirements: requires text parsed by X Parser
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_OPERATIONS
#define HEADER_GIA_TRANSLATOR_OPERATIONS

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAsynRelTranslatorDefs.hpp"
#ifdef GIA_NLC_INTEGRATION
#include "NLCpreprocessorSentenceClass.hpp"
#endif
#include "GIAdatabase.hpp"
#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
#include "LRPpreprocessorWordIdentification.hpp"
#endif
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
#include "LRPpreprocessorSentenceClass.hpp"
#endif
#ifdef GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE
#include "SANIrules.hpp"
#endif
#ifdef GIA_NEURAL_NETWORK
#include "ANNneuronClass.hpp"
#endif
#include "SHAREDvars.hpp"


#ifdef GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE
	#define GIA_ENTITY_TYPE_LOGIC_REFERENCE (GIA_ENTITY_TYPE_ACTION)
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
	string NLPexeFolderArray[GIA_NLP_PARSER_NUMBER_OF_TYPES];
	int NLPfeatureParser;
	int NLPdependencyRelationsParser;
	int NLPdependencyRelationsType;
	bool NLPassumePreCollapsedStanfordRelations;
	#ifdef GIA_NLP_CLIENT_SERVER
	bool NLPclient;
	#endif
	bool parseGIA2file;
	bool linkPreestablishedReferencesGIA;
	
	//network variables;
	unordered_map<string,GIAentityNode*>* entityNodesActiveListNetworkIndexes;
	vector<GIAentityNode*>* entityNodesActiveListComplete;
	int64_t currentEntityNodeIDInNetworkIndexEntityNodesList;
	int64_t currentEntityNodeIDInCompleteList;
	map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences;
	unordered_map<int64_t, GIAtimeConditionNode*>* timeConditionNodesActiveList;

	GIAsentence* firstSentenceInList;
	
	//temporary sentence parsing variables (!parsedSentences only):
	bool saveNetwork;
	vector<bool>* GIAentityNodeArrayFilled;
	vector<GIAentityNode*>* GIAnetworkIndexNodeArray;
	vector<GIAentityNode*>* GIAentityNodeArray;
	vector<GIAentityNode*>* GIAfeatureTempEntityNodeArray;
	GIAsentence* currentSentenceInList;	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	LRPpreprocessorSentence* currentPreprocessorSentenceInList;
	#endif
	
	vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList;
	unordered_map<int64_t, GIAtimeConditionNode*>* sentenceTimeConditionNodesList;
	
	vector<GIAfeature*>* featureArrayTemp;	//only used at start of translator

	int sentenceIndex;	//if parsedSentences only
	
	#ifdef LRP_PREPROCESSOR
	#ifdef USE_NLC
	LRPtranslatorVariablesClass NLCpreprepreprocessorTranslatorVariables;
	#endif
	LRPtranslatorVariablesClass LRPpreprocessorTranslatorVariables;
	#endif
	#ifdef USE_NLCI
	string nlcGeneratedCode;
	#endif
	#ifdef USE_GIAI
	string giaQueryAnswer;
	#endif

	
	#ifdef GIA_NEURAL_NETWORK
	ANNtranslatorVariablesClass ANNtranslatorVariables;
	#endif
};


/*
noun = joe
verb = rides
adjective = happy
*/

class GIAtranslatorOperationsClass
{
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	public: GIAtranslatorOperationsClass(void);
	public: ~GIAtranslatorOperationsClass(void);
	#endif
	
	private: GIAsentenceClassClass GIAsentenceClass;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#endif
	private: GIAdatabaseClass GIAdatabase;
	#ifdef GIA_NLC_INTEGRATION
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	private: NLCpreprocessorSentenceClassClass NLCpreprocessorSentenceClass;
	#endif
	#endif
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAconditionNodeClassClass GIAconditionNodeClass;
	private: SHAREDvarsClass SHAREDvars;

	
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: bool connectionIsAlias(const GIAentityConnection* connection);
	#endif
	public: bool connectionTargetIsRelationship(const GIAentityConnection* connection);
	//public: bool connectionIsRelationship(GIAentityConnection* connection);
	public: int generateConnectionType(const int relationshipEntityType);
	public: int generateConnectionTypeReverse(const int relationshipEntityType);	
	public: int generateInvertedConnectionType(const GIAentityNode* entity, const int connectionType);	
	
	public: GIAentityNode* getPropertyRelationshipTargetEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getPropertyRelationshipSourceEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getDefinitionRelationshipTargetEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getDefinitionRelationshipSourceEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getPropertyRelationshipObjectEntity(GIAentityConnection* relationshipConnection);	
	public: GIAentityNode* getPropertyRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
	public: GIAentityNode* getDefinitionRelationshipObjectEntity(GIAentityConnection* relationshipConnection);
	public: GIAentityNode* getDefinitionRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
		public: GIAentityNode* getRelationshipObjectEntity(GIAentityConnection* relationshipConnection);
			public: GIAentityNode* getRelationshipObjectEntity(GIAentityNode* relationshipEntity);
				public: GIAentityConnection* getRelationshipObjectEntityConnection(GIAentityNode* relationshipEntity);
		public: GIAentityNode* getRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
			public: GIAentityNode* getRelationshipSubjectEntity(GIAentityNode* relationshipEntity);
				public: GIAentityConnection* getRelationshipSubjectEntityConnection(GIAentityNode* relationshipEntity);
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
	public: bool connectPropertyToEntity(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDefinitionToEntity(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_ALIASES
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: bool connectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif
	public: bool connectBeingDefinitionToEntity(GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectHavingPropertyToEntity(GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	#else
	public: bool connectDirectPropertyToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const GIAentityNode* auxHaveEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDirectDefinitionToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const GIAentityNode* auxHaveEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDirectPropertyToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDirectDefinitionToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: bool connectDirectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	public: bool connectDirectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const GIAentityNode* auxHaveEntity, bool sameReferenceSet, const int relationshipEntityType, const GIAtranslatorVariablesClass* translatorVariables, const bool markConnectionAsAlias);
	#endif

	public: bool connectActionToEntity(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectConditionToEntity(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);

	public: bool connectActionToSubject(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectActionToObject(GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectConditionToSubject(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectConditionToObject(GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDefinitionToSubject(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectDefinitionToObject(GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectPropertyToSubject(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	public: bool connectPropertyToObject(GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
	
	private: bool connectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, int relationshipEntityType, const GIAtranslatorVariablesClass* translatorVariables);
	private: bool connectRelationshipToSubject(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, int relationshipEntityType, const GIAtranslatorVariablesClass* translatorVariables);
	private: bool connectRelationshipToObject(GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, int relationshipEntityType, const GIAtranslatorVariablesClass* translatorVariables);
	private: GIAentityNode* useExistingRelationshipIfExistant(GIAentityNode* newOrExistingRelationship, GIAentityNode* relationshipSubjectEntity, const GIAentityNode* relationshipObjectEntity, const GIAentityNode* relationshipEntity, const int relationshipEntityType);
	#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
	private: void setRelationshipObjectToSubstanceIfNecessary(const GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType);
	#endif
	public: void connectRelationshipInstanceToSubject(GIAentityNode* subjectEntity, GIAentityNode* newOrExistingRelationship, bool sameReferenceSet, const int relationshipEntityType, const GIAtranslatorVariablesClass* translatorVariables);
	public: void connectRelationshipInstanceToObject(GIAentityNode* objectEntity, GIAentityNode* newOrExistingRelationship, bool sameReferenceSet, const int relationshipEntityType, const GIAtranslatorVariablesClass* translatorVariables);
	public: void connectEntities(GIAentityNode* entity1, GIAentityNode* entity2, int connectionType, int connectionTypeInverse, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);


	//double check that isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe and isAdjectiveNotConnectedToObjectOrSubject and are no longer required with GIA_SYN_REL_TRANSLATOR_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifndef GIA_SYN_REL_TRANSLATOR_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	public: bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(const GIArelation* currentRelationInList, GIAentityNode* GIAentityNodeArray[], int relationGovernorIndex, const int NLPdependencyRelationsType);
	public: bool isAdjectiveNotConnectedToObjectOrSubject(const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInList, const int NLPdependencyRelationsType);								//Stanford Compatible
	#endif
	
	public: GIAentityNode* addInstanceToInstanceDefinition(GIAentityNode* entity, int instanceType, GIAtranslatorVariablesClass* translatorVariables);
		private: GIAentityNode* addInstance(GIAentityNode* entity, int instanceType, GIAtranslatorVariablesClass* translatorVariables);
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

	public: void recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string,GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string,GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string,GIAentityNode*>* networkIndexEntityNodesListMap);

	public: void convertRelexPOStypeToWordnetWordType(const string* relexPOStype, int* grammaticalWordTypeTemp, const bool grammaticallyStrict);
	public: void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(const string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp, const bool grammaticallyStrict);

	public: void generateTempFeatureArray(GIAtranslatorVariablesClass* translatorVariables, vector<GIAfeature*>* featureArrayTemp);	//used for intrafunction memory allocation purposes only

	public: bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, const GIArelation* relation);

	public: GIAentityNode* addPropertyRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* addDefinitionRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* addConditionRelationshipArtificialToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, string conditionName, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
		public: GIAentityNode* addRelationshipArtificialToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(GIAentityNode* propertyRelationshipSubjectEntity, const GIAentityNode* propertyRelationshipObjectEntity, GIAtranslatorVariablesClass* translatorVariables);
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(GIAentityNode* definitionRelationshipSubjectEntity, const GIAentityNode* definitionRelationshipObjectEntity, GIAtranslatorVariablesClass* translatorVariables);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(GIAtranslatorVariablesClass* translatorVariables);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(GIAtranslatorVariablesClass* translatorVariables);
			#endif
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialCondition(GIAentityNode* propertyRelationshipSubjectEntity, const GIAentityNode* propertyRelationshipObjectEntity, string conditionName, GIAtranslatorVariablesClass* translatorVariables);	//CHECKTHIS
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(GIAentityNode* relationshipSubjectEntity, const GIAentityNode* relationshipObjectEntity, int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
				public: bool findExistingRelationshipInSentenceEntityArray(GIAentityNode* relationshipSubjectEntity, const GIAentityNode* relationshipObjectEntity, const int relationshipEntityType, const string relationshipEntityName, constEffective GIAentityNode** relationshipEntity, const GIAtranslatorVariablesClass* translatorVariables);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificial(int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
				//public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipConditionArtificial(GIAtranslatorVariablesClass* translatorVariables, string conditionName);
					public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(int featureIndex, string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables);
					public: GIAentityNode* findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship(int featureIndex, string* entityNodeName, GIAtranslatorVariablesClass* translatorVariables);

	public: GIAentityNode* addPropertyRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
	public: GIAentityNode* addDefinitionRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
		public: GIAentityNode* addRelationshipArtificialToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial2(GIAentityNode* relationshipSubjectEntity, const GIAentityNode* relationshipObjectEntity, int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
				public: GIAentityNode* addEntityNodeByNameSimpleWrapperRelationshipArtificial2(int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
					public: GIAentityNode* findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship2(string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
			#endif
			
	public: GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables);
		public: GIAentityNode* findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables, const bool tempEntityEnabled);

	public: GIAentityConnection* writeVectorConnection(GIAentityNode* entityNode, GIAentityNode* entityNodeToAdd, int connectionType, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
		public: bool findEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, GIAentityConnection** connectionFound);
		public: bool findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, const GIAentityNode* entityNodeToFind, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex);
		private: bool findEntityNodeNameInVector(GIAentityNode* entityNode, const string* entityNodeNameToFind, const int connectionType, GIAentityConnection** connectionFound);

	private: int64_t determineNextIdInstance(GIAentityNode* entity);

	#ifdef GIA_DATABASE
	/*//replaced with optimised function findEntityNodesActiveListCompleteFastIndexDBactive()
	bool entityInActiveListComplete(string* entityName, int64_t idInstance);
	bool entityInActiveListComplete(GIAentityNode* entity);
	*/
	public: void addInstanceEntityNodeToActiveLists(GIAentityNode* entity, GIAtranslatorVariablesClass* translatorVariables);
	#endif

	#ifdef GIA_ALIASES
	#ifndef GIA_DISABLE_ALIAS_ENTITY_MERGING
	public: bool mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge, const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif

	public: GIAentityNode* getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNode* instanceEntity);

	#ifdef GIA_NLC_INTEGRATION
	private: NLCpreprocessorSentence* getFirstNLCsentenceInListGIA();
	public: void setFirstNLCsentenceInListGIA(constEffective NLCpreprocessorSentence* firstNLCsentenceInListNew);
	#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
	public: bool checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA(const GIAentityNode* indefiniteEntity, const GIAentityNode* definiteEntity, int* indentationDifferenceFound);
	#endif
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	public: bool checkIfSentenceIsMathTextParsablePhrase(const int sentenceIndex);
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	public: GIAentityNode* createNewNonspecificConcept(GIAentityNode* networkIndexEntity, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* getNonspecificConceptEntityFromNetworkIndex(GIAentityNode* networkIndexEntity);
	public: GIAentityNode* getNonspecificConceptEntityFromInstance(GIAentityNode* instanceEntity);
		private: bool isNonspecificConceptEntity(GIAentityNode* entity);
	#endif
	
	public: void printEntity(GIAentityNode* entity);

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	private: void createParentsOfSubclassEntities(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	private: void createAdditionalSubclassEntities(GIAtranslatorVariablesClass* translatorVariables);
	#endif
		private: void linkSubclassEntitiesWithParentClassEntities(GIAentityNode* subclassNetworkIndexEntity, GIAentityNode* parentClassNetworkIndexEntity, const bool linkAsAlias, const bool onlyLinkIfSubclassOrParentConceptNotFound, GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
	private: void detectUserDeclaredSubclassEntities(const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif
		
	#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
	void createShortcutsToConceptEntities(GIAtranslatorVariablesClass* translatorVariables);
		bool hasSameReferenceSetConnections(GIAentityNode* entity, const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	
	public: bool connectDefinitionAliasWrapper(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode** entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, bool sameReferenceSet);
	public: bool connectPrenominalModifierWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, bool sameReferenceSet);
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	public: bool connectMultiwordAuxiliaryWrapper(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet);		
	#endif
	public: bool connectMultiwordPrepositionWrapper(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet);		
	public: bool connectMultiwordAliasWrapper(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet);
		//private: GIAentityNode* createNewRelationshipEntitySemantic(int relationshipEntityType, string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
		private: bool connectMultiwordCollapse(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet);
		private: bool connectMultiwordCollapseReverse(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet);
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_COLLAPSE_ALIASES_ONLY
			public: string* getFirstAlias(GIAentityNode* entity);
			#endif
			private: void passMultiwordGrammaticalParameters(GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2);
	public: bool connectMultiwordDate(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, const GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet);
		public: bool addTimeConditionProperty(GIAtimeConditionNode* timeConditionNode, const GIAentityNode* entity);
	public: bool connectQuantityToEntity(const GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, const GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet);
	public: bool connectMeasureToEntity(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, bool sameReferenceSet);
	public: bool connectMeasurePerToEntity(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, bool sameReferenceSet);

	#ifdef GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE
	public: bool connectLogicReferenceConjunction(const GIAtranslatorVariablesClass* translatorVariables, string logicReferenceClassType, GIAentityNode* targetEntity, GIAentityNode* logicReferenceEntity, bool sameReferenceSet);
	public: bool connectLogicReferenceConclusion(const GIAtranslatorVariablesClass* translatorVariables, string logicReferenceClassType, GIAentityNode* targetEntity, GIAentityNode* logicReferenceEntity, bool sameReferenceSet);
	public: bool connectLogicReference(const GIAtranslatorVariablesClass* translatorVariables, int logicReferenceClass, string logicReferenceClassType, GIAentityNode* sourceEntity, GIAentityNode* targetEntity, GIAentityNode* logicReferenceEntity, bool sameReferenceSet);
		public: void connectLogicReferenceRelationshipToTarget(GIAentityNode* relationship, GIAentityNode* targetEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
		public: void connectLogicReferenceRelationshipToSource(GIAentityNode* relationship, GIAentityNode* sourceEntity, bool sameReferenceSet, const GIAtranslatorVariablesClass* translatorVariables);
			private: int generateConnectionTypeTargetToLogicReferenceRelationship(const GIAentityNode* relationship);
			private: int generateConnectionTypeSourceToLogicReferenceRelationship(const GIAentityNode* relationship);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	public: int getEntityArrayMaxIndex(const GIAtranslatorVariablesClass* translatorVariables);
	#else
	public: int getEntityArrayMaxIndex(const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	
	public: GIAentityConnection* getConnection(GIAentityNode* entity, const GIAentityNode* relationshipEntity);
	
	public: int getCurrentSentenceIndex(const GIAtranslatorVariablesClass* translatorVariables);

	public: void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, const string auxiliaryOrCopulaString);
	
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	public: bool semanticRelationReturnConnectionDynamic;
	#endif

};


#endif

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
 * Project Version: 3a1d 26-February-2017
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
#include "GIApreprocessorMultiwordReduction.hpp"
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
	bool* GIAentityNodeArrayFilled;
	GIAentityNode** GIAnetworkIndexNodeArray;
	GIAentityNode** GIAfeatureTempEntityNodeArray;
	GIAentityNode** GIAentityNodeArray;
	
	vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList;
	unordered_map<long, GIAtimeConditionNode*>* sentenceTimeConditionNodesList;
	
	GIAfeature** featureArrayTemp;	//only used at start of translator

	int sentenceIndex;	//if parsedSentences only
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	GIApreprocessorSentence* firstGIApreprocessorSentenceInList;
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
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIAdatabaseClass GIAdatabase;
#ifdef GIA_NLC_INTEGRATION
#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	private: NLCpreprocessorSentenceClassClass NLCpreprocessorSentenceClass;
#endif
#endif
	
	/*
	public: GIAentityNode* getPropertyRelationshipObjectEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getPropertyRelationshipSubjectEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getDefinitionRelationshipObjectEntity(GIAentityNode* relationshipEntity);
	public: GIAentityNode* getDefinitionRelationshipSubjectEntity(GIAentityNode* relationshipEntity);
	*/
	public: GIAentityNode* getPropertyRelationshipObjectEntity(GIAentityConnection* connection);	
	public: GIAentityNode* getPropertyRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
	public: GIAentityNode* getDefinitionRelationshipObjectEntity(GIAentityConnection* connection);
	public: GIAentityNode* getDefinitionRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
		public: GIAentityNode* getRelationshipObjectEntity(GIAentityConnection* connection);
		GIAentityNode* getRelationshipObjectEntity(GIAentityNode* relationshipEntity);
		public: GIAentityNode* getRelationshipSubjectEntity(GIAentityConnection* relationshipConnectionReverse);
		GIAentityNode* getRelationshipSubjectEntity(GIAentityNode* relationshipEntity);
			
	GIAentityNode* connectPropertyToEntity(GIAentityNode* propertyRelationshipSubjectEntity, GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	GIAentityNode* connectDefinitionToEntity(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	GIAentityNode* connectDefinitionToEntityMarkConnectionAsAlias(GIAentityNode* definitionRelationshipSubjectEntity, GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	#endif

	public: GIAentityNode* connectActionToEntity(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* connectActionToSubject(GIAentityNode* actionRelationshipSubjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* connectActionToObject(GIAentityNode* actionRelationshipObjectEntity, GIAentityNode* actionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* connectConditionToEntity(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* connectConditionToSubject(GIAentityNode* conditionRelationshipSubjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* connectConditionToObject(GIAentityNode* conditionRelationshipObjectEntity, GIAentityNode* conditionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* connectBeingDefinitionToEntity(GIAentityNode* definitionRelationshipObjectEntity, GIAentityNode* definitionRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* connectHavingPropertyToEntity(GIAentityNode* propertyRelationshipObjectEntity, GIAentityNode* propertyRelationshipEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);

	private: GIAentityNode* addOrConnectRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	private: GIAentityNode* addOrConnectRelationshipToSubject(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	private: GIAentityNode* addOrConnectRelationshipToObject(GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, bool sameReferenceSet, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
	private: GIAentityNode* useExistingRelationshipIfExistant(GIAentityNode* newOrExistingRelationship, GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity, const int relationshipEntityType);
	#ifdef GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS
	private: void setRelationshipObjectToSubstanceIfNecessary(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, const int relationshipEntityType);
	#endif
	public: int generateConnectionType(int relationshipEntityType);
	public: int generateConnectionTypeReverse(int relationshipEntityType);
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
	public: void disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNode* entity);	//not used

	public: void recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
	public: void recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);

	public: void convertRelexPOStypeToWordnetWordType(const string* relexPOStype, int* grammaticalWordTypeTemp);
	public: void convertStanfordPOStagToRelexPOStypeAndWordnetWordType(const string* POStag, string* relexPOStype, int* grammaticalWordTypeTemp);

	public: void generateTempFeatureArray(GIAfeature* firstFeatureInList, GIAfeature* featureArrayTemp[]);	//used for intrafunction memory allocation purposes only

#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	public: bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, const GIArelation* relation);
#endif

	public: GIAentityNode* addPropertyRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* addDefinitionRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
		public: GIAentityNode* addRelationshipToEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables, bool isArtificial);
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(GIAtranslatorVariablesClass* translatorVariables);
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(GIAtranslatorVariablesClass* translatorVariables);
				public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationship(const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables, bool isArtificial);
					public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial(const int relationshipEntityIndex, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
	public: GIAentityNode* addPropertyRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
	public: GIAentityNode* addDefinitionRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed);
		public: GIAentityNode* addRelationshipToEntity2(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, bool sameReferenceSet, const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed, bool isArtificial);
			public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationship2(const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariablesSentencesParsed, bool isArtificial);
			
	public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(int featureIndex, const string* entityNodeName, bool* entityAlreadyExistant, GIAtranslatorVariablesClass* translatorVariables);
		public: GIAentityNode* findOrAddEntityNodeByNameSimpleWrapperRelationship(int featureIndex, const string* entityNodeName, GIAtranslatorVariablesClass* translatorVariables);
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
	public: void mergeEntityNodesAddAlias(GIAentityNode* entityNode, GIAentityNode* entityNodeToMerge, GIAtranslatorVariablesClass* translatorVariables);
#endif

	public: GIAentityNode* getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNode* instanceEntity);
	int generateInvertedConnection(GIAentityNode* entity, const int connectionType);

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
};


#endif

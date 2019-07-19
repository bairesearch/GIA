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
 * File Name: GIAtxtRelTranslatorRulesGroupClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3i3c 24-June-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Rules
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_RULES_GROUP_CLASS
#define HEADER_GIA_TXT_REL_TRANSLATOR_RULES_GROUP_CLASS

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorWordClass.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#endif



#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED (1)	//type 1

#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED (3)	//type 2
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED (4)	//type 2

#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE (5)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF (6)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED (9)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE (10)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET (11)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED (12)	//type 3
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET (13)	//type 3

#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED (7)	//type 4
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY (8)	//type 4



//TODO: rename COMPONENT to GROUP TYPE:
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN (0)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_LOGICREFERENCESET (1)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_REFERENCESET (2)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SUBREFERENCESET (3)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE (4)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES (5)
static string GIAtxtRelTranslatorRulesGroupsComponentReferenceSetTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSet", "referenceSet", "subReferenceSet", "sentence"};


#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_UNKNOWN (0)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS (1)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS (2)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETS (3)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETPARTS (4)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETAPPENDS (5)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETSOPTIONAL (6)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS (7)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS (8)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS (9)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES (10)	//this is just a selection of prominent groupTypes (it doesn't include all of them)
static string GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSets", "referenceSets", "subReferenceSets", "subReferenceSetsPart", "subReferenceSetsAppend", "logicReferenceSetsOptional", "statements", "questions", "subjects"};




class GIAtxtRelTranslatorDebug
{
public:
	GIAtxtRelTranslatorDebug(void);
	~GIAtxtRelTranslatorDebug(void);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	int activationMemoryGroupArrayIndex;
	int activationMemoryGroupArraySize;
	#endif
	bool firstComponentActive;
	bool secondComponentActive;
};



class GIAtxtRelTranslatorParserForwardPropogationSignalData
{
public:
	GIAtxtRelTranslatorParserForwardPropogationSignalData(void);
	~GIAtxtRelTranslatorParserForwardPropogationSignalData(void);
	
	GIAentityNode* semanticRelationReturnEntity;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	GIAentityNode* semanticRelationReturnEntitySubject;
	GIAentityNode* semanticRelationReturnEntityDelimiter;
	string semanticRelationReturnFunctionNameRecord;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	vector<GIAentityNode*> semanticRelationReturnFunctionEntityArray;
	#endif
};

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
class GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData(void);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordNounVariantType;
	int wordVerbVariantType;
	#endif	
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	bool foundPreviousActiveWord;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	GIAtxtRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;	//semantic relation function parameters
	#endif
};

class GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData(void);
		
	 //word specific variables:
	GIApreprocessorPlainTextWord* wordReference;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	int wordPOStype;	
	#endif
	
	int w;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool expectToSeeConnectionWithPreviousWordTrace;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnections;
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool foundPreviousActiveWord;	//debug only
	#endif
};
#endif


class GIAtxtRelTranslatorRulesComponentNeuralNetwork;
class GIAtxtRelTranslatorRulesComponentParseTree;
class GIAtxtRelTranslatorRulesGroupActivationMemory;
class GIAtxtRelTranslatorRulesGroupParseTree;


class GIAtxtRelTranslatorRulesGroup
{
public:
	GIAtxtRelTranslatorRulesGroup(void);
	~GIAtxtRelTranslatorRulesGroup(void);

	//shared variables;	
	string groupName;
	string groupTypeName;
	int groupTypeReferenceSetType;
	vector<string> semanticRelationFunctionName;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	string semanticRelationFunctionConditionNewName;
	#endif				
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
	int groupWeight;
	#endif
				
	//GIAtxtRelTranslatorRulesGroup variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	string previousWordPOStype;
	string existsPreceedingWordPOStype;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_INVERSE_NEURAL_NETWORK
	int numberOfConsecutiveTimesPreviousGroupType;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordVerbVariantTypeDerived;
	int wordNounVariantTypeDerived;	
	#endif
	#endif	
};



class GIAtxtRelTranslatorRulesGroupNeuralNetwork: public GIAtxtRelTranslatorRulesGroup
{
public:
	GIAtxtRelTranslatorRulesGroupNeuralNetwork(void);
	~GIAtxtRelTranslatorRulesGroupNeuralNetwork(void);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	public: void initiateANNneuron(const string groupName);
	#endif
	
	vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> components;	

	//GIAtxtRelTranslatorRulesGroupNeuralNetwork variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_RESET
	bool nonResetActivationFoundDuringPreprocess;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
	bool newActivationFoundDuringPreprocess;
	#endif
	#endif
	vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*> ANNfrontGroupConnectionList;	//for IO only
	vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*> ANNbackGroupConnectionList;	//for IO only
	vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> ANNfrontComponentConnectionList;
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* next;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	ANNneuron* neuronReference;
	bool neuronDisplayPositionSet;
	int neuronDisplayPositionX;
	int neuronDisplayPositionY;
	bool neuronConnectivitySet;
	#endif
	string GIAtokenLayerName;
	string GIAtokenLayerClassName;
	string GIAtokenLayerClassTypeName;
	string GIAtokenLayerClassTypeInstanceName;
	bool neuronGenerated;
	bool neuronPropagated;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	bool neuronProcessed;
	#endif
	bool neuronPropagatedSave;
	bool neuronPreviousWordPOStypeTested;	//NOT USED
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalData;
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//temporary	//only update semanticRelationReturnEntityForwardPropogationSignalData upon complete/group activation (prevents ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData from being invalidated by partial reactivations of a group; when accessed by an unrelated instance of GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwor) 	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO_WEAK3
	bool solidified;
	#endif
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	vector<GIAtxtRelTranslatorRulesGroupActivationMemory*> activationMemoryGroupArray;
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	vector<GIAtxtRelTranslatorRulesGroupParseTree*> currentParseTreeGroupArray;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_NEW
	GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp;	//used fill in currentParseTreeGroupTemp components before adding currentParseTreeGroup to parseTreeGroup network
	#endif
	//#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT
	bool neuronActive;	//interpretation: all components are active (unless they are optional / unused or cases)
	//#endif	
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REMOVE_LAST_OPTIONAL_COMPONENTS
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* optionalComponentsWrapperGroup;
	#endif
	#endif	

};


class GIAtxtRelTranslatorRulesGroupActivationMemory: public GIAtxtRelTranslatorRulesGroupNeuralNetwork
{
public:
	GIAtxtRelTranslatorRulesGroupActivationMemory(void);
	~GIAtxtRelTranslatorRulesGroupActivationMemory(void);

	
	//vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> components;	

	//GIAtxtRelTranslatorRulesGroupActivationMemory variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
	int numberWordsInSet;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalData;
	#endif
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData forwardPropogationWordData;
	int layer;
	bool inputGroupString;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig;	//for memoryActivationGroups only
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	bool pointArrayGenerated;
	bool firstLevelActivationPointAdded;
	vector<GIAtxtRelTranslatorRulesGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupActivationPointArray;
	#endif
	bool wordGroupNeuron;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
	int lastWordIndexActivated;
	#endif
	#endif
	#endif
	#endif

};


class GIAtxtRelTranslatorRulesGroupParseTree: public GIAtxtRelTranslatorRulesGroup
{
public:
	GIAtxtRelTranslatorRulesGroupParseTree(void);
	~GIAtxtRelTranslatorRulesGroupParseTree(void);
		
	vector<GIAtxtRelTranslatorRulesComponentParseTree*> components;	

	//GIAtxtRelTranslatorRulesGroupParseTree variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	vector<GIAtxtRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupActivationPointArray;	
	#endif	
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalData;
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//temporary	//only update semanticRelationReturnEntityForwardPropogationSignalData upon complete/group activation (prevents ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData from being invalidated by partial reactivations of a group; when accessed by an unrelated instance of GIAtxtRelTranslatorNeuralNetworkParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwor) 	
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* parseTreeGroupRefReverse;
	int parseTreeGroupRefReverseComponentIndex;
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO
	bool solidified;
	#endif
	#endif
	bool neuronTraced;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	bool neuronActive;	//interpretation: all components are active (unless they are optional / unused or cases)
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	int parseTreeMinWordIndex;	//sentenceIndex
	int parseTreeMaxWordIndex;	//sentenceIndex
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
	double parseTreeMaxWeight;	//NOT USED
	double parseTreeMinWeight;
	double parseTreeTotalWeight;	//NOT USED
	#endif
	#endif
	#endif

};







class GIAtxtRelTranslatorRulesGroupType
{
public:
	GIAtxtRelTranslatorRulesGroupType(void);
	~GIAtxtRelTranslatorRulesGroupType(void);

	string groupTypeName;
	int referenceSetType;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	bool neuronDisplayPositionSet;
	int neuronDisplayPositionX;
	int neuronDisplayPositionY;
	int groupTypeXindex;
	int groupMaxY;
	#endif
		
	vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*> groups;
};



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	
class GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData(void);
	
	bool generateActivationPointArray;
	bool connectToPreviousActivationGroup;
	vector<GIAtxtRelTranslatorRulesGroupActivationMemory*>* activationPathWordFirstActivationMemoryGroupActivationPointArray;
	vector<GIAtxtRelTranslatorRulesGroupParseTree*>* activationPathWordFirstParseTreeGroupActivationPointArray;
	int layer;	
	int activationPathW;
};

class GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData
{
public:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void);
	~GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void);
		
	//sentence specific variables:
	bool toplevelGroupActivationFound;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	bool parserEnabled;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_PERFORMANCE
	int performance;
	#endif
	vector<GIApreprocessorPlainTextWord*>* sentenceContents;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	bool isQuery;
	#endif
	//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	bool parseIsolatedSubreferenceSets;
	//#endif
	
	bool finishedPassingSentenceWords;
	
	vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes;

	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> forwardPropogationWordDataArray;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	vector<GIAtxtRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupArray;
	#else
	vector<vector<GIAtxtRelTranslatorRulesGroupActivationMemory*>> activationPathWordFirstActivationMemoryGroupArray;
	vector<vector<GIAtxtRelTranslatorRulesGroupParseTree*>> activationPathWordFirstParseTreeGroupArray;
	#endif
	#endif
	
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData* forwardPropogationActivationPointData;
};

#endif


class GIAtxtRelTranslatorRulesGroupClass
{
	public: void copyParseGroupInfo(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup);
	public: bool isTopLevelGroupType(const string groupTypeName, const int groupTypeReferenceSetType, const bool isQuery, const bool parseIsolatedSubreferenceSets);	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	public: bool determineIsQuery(vector<GIApreprocessorPlainTextWord*>* sentenceContents);
	#endif
};





#endif

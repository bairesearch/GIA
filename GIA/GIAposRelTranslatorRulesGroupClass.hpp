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
 * File Name: GIAposRelTranslatorRulesGroupClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l6c 21-June-2020
 * Requirements: requires plain text file
 * Description: Part-of-speech Relation Translator Rules
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_RULES_GROUP_CLASS
#define HEADER_GIA_POS_REL_TRANSLATOR_RULES_GROUP_CLASS

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorWordClass.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_SANI
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#endif


#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED (1)	//type 1

#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED (3)	//type 2
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED (4)	//type 2

#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE (5)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF (6)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED (9)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE (10)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET (11)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED (12)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET (13)	//type 3

#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED (7)	//type 4
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY (8)	//type 4



//TODO: rename COMPONENT to GROUP TYPE:
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_LOGICREFERENCESET (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_REFERENCESET (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SUBREFERENCESET (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE (4)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES (5)
static string GIAposRelTranslatorRulesGroupsComponentReferenceSetTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSet", "referenceSet", "subReferenceSet", "sentence"};


#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETS (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETPARTS (4)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETAPPENDS (5)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETSOPTIONAL (6)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS (7)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS (8)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS (9)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES (10)	//this is just a selection of prominent groupTypes (it doesn't include all of them)
static string GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSets", "referenceSets", "subReferenceSets", "subReferenceSetsPart", "subReferenceSetsAppend", "logicReferenceSetsOptional", "statements", "questions", "subjects"};




class GIAposRelTranslatorDebug
{
public:
	GIAposRelTranslatorDebug(void);
	~GIAposRelTranslatorDebug(void);
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	int activationMemoryGroupArrayIndex;
	int activationMemoryGroupArraySize;
	#endif
	bool firstComponentActive;
	bool secondComponentActive;
};



class GIAposRelTranslatorParserForwardPropogationSignalData
{
public:
	GIAposRelTranslatorParserForwardPropogationSignalData(void);
	~GIAposRelTranslatorParserForwardPropogationSignalData(void);
	
	GIAentityNode* semanticRelationReturnEntity;
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	bool semanticRelationReturnConnectionDynamic;
	#endif
	*/
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	GIAentityNode* semanticRelationReturnEntitySubject;
	GIAentityNode* semanticRelationReturnEntityDelimiter;
	string semanticRelationReturnFunctionNameRecord;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	vector<GIAentityNode*> semanticRelationReturnFunctionEntityArray;
	#endif
};

#ifdef GIA_POS_REL_TRANSLATOR_SANI
class GIAposRelTranslatorSANIForwardPropogationSignalData
{
public:
	GIAposRelTranslatorSANIForwardPropogationSignalData(void);
	~GIAposRelTranslatorSANIForwardPropogationSignalData(void);
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int firstIndexInSequence;
	#endif
	//bool createNewConnections;
	//int maxLayerToCreateNewConnections;
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;	//OLD: +1 (e.g. sentence length); ie word indices must occur before activatedNeuronWithMaxWordIndexCoverageLastWordIndex
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordNounVariantType;
	int wordVerbVariantType;
	#endif	
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
	bool foundPreviousActiveWord;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	GIAposRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;	//semantic relation function parameters
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	bool firstPOSval;
	#endif
};

class GIAposRelTranslatorSANIForwardPropogationWordData
{
public:
	GIAposRelTranslatorSANIForwardPropogationWordData(void);
	~GIAposRelTranslatorSANIForwardPropogationWordData(void);
		
	 //word specific variables:
	GIApreprocessorPlainTextWord* wordReference;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	int wordPOStype;	
	#endif
	
	int w;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool expectToSeeConnectionWithPreviousWordTrace;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
	vector<GIAposRelTranslatorSANIForwardPropogationWordData*> previousWordConnections;
	#endif
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool foundPreviousActiveWord;	//debug only
	#endif
};

#endif


class GIAposRelTranslatorRulesComponentNeuralNetwork;
class GIAposRelTranslatorRulesComponentParseTree;
class GIAposRelTranslatorRulesGroupActivationMemory;
class GIAposRelTranslatorRulesGroupParseTree;


class GIAposRelTranslatorRulesGroup
{
public:
	GIAposRelTranslatorRulesGroup(void);
	~GIAposRelTranslatorRulesGroup(void);

	//shared variables;	
	string groupName;
	string groupTypeName;
	int groupTypeReferenceSetType;
	vector<string> semanticRelationFunctionName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	string semanticRelationFunctionConditionNewName;
	#endif				
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
	int groupWeight;
	#endif
				
	//GIAposRelTranslatorRulesGroup variables;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	string previousWordPOStype;
	string existsPreceedingWordPOStype;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_INVERSE_NEURAL_NETWORK
	int numberOfConsecutiveTimesPreviousGroupType;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordVerbVariantTypeDerived;
	int wordNounVariantTypeDerived;	
	#endif
	#endif	
};



class GIAposRelTranslatorRulesGroupNeuralNetwork: public GIAposRelTranslatorRulesGroup
{
public:
	GIAposRelTranslatorRulesGroupNeuralNetwork(void);
	~GIAposRelTranslatorRulesGroupNeuralNetwork(void);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	public: void initiateANNneuron(const string groupName);
	#endif
	
	vector<GIAposRelTranslatorRulesComponentNeuralNetwork*> components;	

	//GIAposRelTranslatorRulesGroupNeuralNetwork variables;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	int networkDepth;
	int networkLeafSize;
	#endif	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	vector<GIAposRelTranslatorRulesGroupParseTree*> parseTreeGroupMemory;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	bool marked;
	#endif	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	bool counted;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
	bool activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp;	
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
	bool verified;
	#endif
	bool inputLayerNeuron;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
	bool firstHiddenLayerNeuron;
	#endif
	bool groupTypeIsString;
	//GIAposRelTranslatorSANIForwardPropogationWordData* wordDataTemp;
	int wordPOStype;
	int groupIndex;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WEIGHTS
	double groupStrength;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS
	bool topLevelSentenceNeuron;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
	bool nonResetActivationFoundDuringPreprocess;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
	bool newActivationFoundDuringPreprocess;
	#endif
	#endif
	vector<GIAposRelTranslatorRulesGroupNeuralNetwork*> ANNfrontGroupConnectionList;	//for IO only
	vector<GIAposRelTranslatorRulesGroupNeuralNetwork*> ANNbackGroupConnectionList;	//for IO only
	vector<GIAposRelTranslatorRulesComponentNeuralNetwork*> ANNfrontComponentConnectionList;
	GIAposRelTranslatorRulesGroupNeuralNetwork* next;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	ANNneuron* neuronReference;
	bool neuronDisplayPositionSet;
	int neuronDisplayPositionX;
	int neuronDisplayPositionY;
	bool neuronConnectivitySet;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_CENTRE_NEURONS
	int neuronDisplayPositionXcentred;	
	int neuronDisplayPositionYcentred;	
	#endif
	#endif
	string GIAtokenLayerName;
	string GIAtokenLayerClassName;
	string GIAtokenLayerClassTypeName;
	string GIAtokenLayerClassTypeInstanceName;
	bool neuronGenerated;
	bool neuronPropagated;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	bool neuronProcessed;
	#endif
	bool neuronPropagatedSave;
	bool neuronPreviousWordPOStypeTested;	//NOT USED
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT
	GIAposRelTranslatorSANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalData;
	GIAposRelTranslatorSANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//temporary	//only update semanticRelationReturnEntityForwardPropogationSignalData upon complete/group activation (prevents ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData from being invalidated by partial reactivations of a group; when accessed by an unrelated instance of GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwor) 	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_BIO_WEAK3
	bool solidified;
	#endif
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	vector<GIAposRelTranslatorRulesGroupActivationMemory*> activationMemoryGroupArray;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	vector<GIAposRelTranslatorRulesGroupParseTree*> currentParseTreeGroupArray;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_NEW
	GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp;	//used fill in currentParseTreeGroupTemp components before adding currentParseTreeGroup to parseTreeGroup network
	#endif
	//#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT
	bool neuronActive;	//interpretation: all components are active (unless they are optional / unused or cases)
	//#endif	
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
	GIAposRelTranslatorRulesGroupNeuralNetwork* optionalComponentsWrapperGroup;
	#endif
	#endif	

};


class GIAposRelTranslatorRulesGroupActivationMemory: public GIAposRelTranslatorRulesGroupNeuralNetwork
{
public:
	GIAposRelTranslatorRulesGroupActivationMemory(void);
	~GIAposRelTranslatorRulesGroupActivationMemory(void);

	
	//vector<GIAposRelTranslatorRulesComponentNeuralNetwork*> components;	

	//GIAposRelTranslatorRulesGroupActivationMemory variables;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
	int numberWordsInSet;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	GIAposRelTranslatorSANIForwardPropogationSignalData forwardPropogationSignalData;
	#endif
	GIAposRelTranslatorSANIForwardPropogationWordData forwardPropogationWordData;
	int layer;
	bool inputGroupString;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig;	//for memoryActivationGroups only
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	bool pointArrayGenerated;
	bool firstLevelActivationPointAdded;
	vector<GIAposRelTranslatorRulesGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupActivationPointArray;
	#endif
	bool wordGroupNeuron;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
	int lastWordIndexActivated;
	#endif
	#endif
	#endif
	#endif

};


class GIAposRelTranslatorRulesGroupParseTree: public GIAposRelTranslatorRulesGroup
{
public:
	GIAposRelTranslatorRulesGroupParseTree(void);
	~GIAposRelTranslatorRulesGroupParseTree(void);
		
	vector<GIAposRelTranslatorRulesComponentParseTree*> components;	

	//GIAposRelTranslatorRulesGroupParseTree variables;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	GIAposRelTranslatorRulesGroupNeuralNetwork* groupRef;	//backup of original group (non-parse-tree)
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	//bool variableStartComponentFound;
	bool missingOrVariableStartComponentFound;
	#endif
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
	bool missingStartComponentFound;
	#endif
	*/
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	vector<GIAposRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupActivationPointArray;	
	#endif	
	GIAposRelTranslatorSANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalData;
	GIAposRelTranslatorSANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//temporary	//only update semanticRelationReturnEntityForwardPropogationSignalData upon complete/group activation (prevents ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData from being invalidated by partial reactivations of a group; when accessed by an unrelated instance of GIAposRelTranslatorSANIParserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwor) 	
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	GIAposRelTranslatorRulesGroupNeuralNetwork* parseTreeGroupRefReverse;
	int parseTreeGroupRefReverseComponentIndex;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_BIO
	bool solidified;
	#endif
	#endif
	bool neuronTraced;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	bool neuronActive;	//interpretation: all components are active (unless they are optional / unused or cases)
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	int parseTreeMinWordIndex;
	int parseTreeMaxWordIndex;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
	double parseTreeMaxWeight;	//NOT USED
	double parseTreeMinWeight;
	double parseTreeTotalWeight;	//NOT USED
	#endif
	#endif
	#endif

};







class GIAposRelTranslatorRulesGroupType
{
public:
	GIAposRelTranslatorRulesGroupType(void);
	~GIAposRelTranslatorRulesGroupType(void);

	string groupTypeName;
	int referenceSetType;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	bool neuronDisplayPositionSet;
	int neuronDisplayPositionX;
	int neuronDisplayPositionY;
	int groupTypeXindex;
	int groupMaxY;
	#endif
		
	vector<GIAposRelTranslatorRulesGroupNeuralNetwork*> groups;
};



#ifdef GIA_POS_REL_TRANSLATOR_SANI
	
class GIAposRelTranslatorSANIForwardPropogationActivationPointData
{
public:
	GIAposRelTranslatorSANIForwardPropogationActivationPointData(void);
	~GIAposRelTranslatorSANIForwardPropogationActivationPointData(void);
	
	bool generateActivationPointArray;
	bool connectToPreviousActivationGroup;
	vector<GIAposRelTranslatorRulesGroupActivationMemory*>* activationPathWordFirstActivationMemoryGroupActivationPointArray;
	vector<GIAposRelTranslatorRulesGroupParseTree*>* activationPathWordFirstParseTreeGroupActivationPointArray;
	int layer;	
	int activationPathW;
};

class GIAposRelTranslatorSANIForwardPropogationSentenceData
{
public:
	GIAposRelTranslatorSANIForwardPropogationSentenceData(void);
	~GIAposRelTranslatorSANIForwardPropogationSentenceData(void);
	
	bool parseSentenceReverse;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	bool recordActivatedNeuronWithMaxWordIndexCoverage;
	GIAposRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage;
	bool activatedNeuronWithMaxWordIndexCoveragePartial;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS
	bool recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	bool recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
	int variableFirstComponentMaxDepth;
	#endif
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponent;	//ie candidateCoverageMissingOrVariableStartComponent
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
	vector<GIAposRelTranslatorRulesGroupNeuralNetwork*> listOfHighLevelNeuronsCompleteHistory;
	#endif
	#endif
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
	bool activatedNeuronWithMaxWordIndexCoverageMissingStartComponent;
	#endif
	*/
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
	bool findingCandidateComponent2;
	GIAposRelTranslatorRulesComponentNeuralNetwork* candidateComponent2ToFind;
	bool foundCandidateComponent2;
	GIAposRelTranslatorRulesGroupParseTree* candidateComponent2sourceParseTreeGroup;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	vector<vector<GIAposRelTranslatorRulesGroupNeuralNetwork*>*>* firstLayer;
	#else
	vector<GIAposRelTranslatorRulesGroupNeuralNetwork*>* firstLayer;
	#endif
	#endif
	
	//sentence specific variables:
	bool toplevelGroupActivationFound;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	bool parserEnabled;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE
	int performance;
	#endif
	vector<GIApreprocessorPlainTextWord*>* sentenceContents;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	bool isQuery;
	#endif
	//#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	bool parseIsolatedSubreferenceSets;
	//#endif
	
	bool finishedPassingSentenceWords;
	
	vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes;

	vector<GIAposRelTranslatorSANIForwardPropogationWordData*> forwardPropogationWordDataArray;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	vector<GIAposRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupArray;
	#else
	vector<vector<GIAposRelTranslatorRulesGroupActivationMemory*>> activationPathWordFirstActivationMemoryGroupArray;
	vector<vector<GIAposRelTranslatorRulesGroupParseTree*>> activationPathWordFirstParseTreeGroupArray;
	#endif
	#endif
	
	GIAposRelTranslatorSANIForwardPropogationActivationPointData* forwardPropogationActivationPointData;
};

#endif


class GIAposRelTranslatorRulesGroupClass
{
	public: void copyParseGroupInfo(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup);
	public: bool isTopLevelGroupType(const string groupTypeName, const int groupTypeReferenceSetType, const bool isQuery, const bool parseIsolatedSubreferenceSets);	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	public: bool determineIsQuery(vector<GIApreprocessorPlainTextWord*>* sentenceContents);
	#endif
};





#endif

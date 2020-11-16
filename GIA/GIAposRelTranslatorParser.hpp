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
 * File Name: GIAposRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3o3a 16-November-2020
 * Requirements: requires text parsed by BAI Sequentially Activated Neuronal Input (SANI)
 * Description: Part-of-speech Relation Translator Parser
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_PARSER
#define HEADER_GIA_POS_REL_TRANSLATOR_PARSER

#include "GIAglobalDefs.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
//#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE

#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAsentenceClass.hpp"
//#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
#include "GIAsemRelTranslatorOperations.hpp"
//#endif
#include "GIAtranslatorOperations.hpp"
#include "GIAtranslatorGrammar.hpp"
#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
#include "GIAbot.hpp"
#endif
#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
#include "GIAsynRelTranslatorRedistributeRelations.hpp"
#endif
#include "GIAtranslatorReferencing.hpp"
#include "GIAsemRelTranslatorParser.hpp"
#include "GIAsynRelTranslatorParser.hpp"
//#include "SANIrules.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "GIAposRelTranslatorHybrid.hpp"
#ifdef SANI_PARSE_SIMULTANEOUS
#include "SANIposRelTranslatorPermutations.hpp"
#else
#include "GIAposRelTranslatorParserOperations.hpp"
#endif
#include "SANIposRelTranslatorVariables.hpp"

#include "GIAsynRelTranslatorDefineSubstances.hpp"
#include "GIAsynRelTranslatorApplyAdvancedFeatures.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "SHAREDvars.hpp"

class GIAposRelTranslatorParserClass
{
	private: GIAentityNodeClassClass GIAentityNodeClassObject;
	private: GIAsentenceClassClass GIAsentenceClass;
	//#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	//#endif
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	private: GIAbotClass GIAbot;
	#endif
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	private: GIAsynRelTranslatorRedistributeRelationsClass GIAsynRelTranslatorRedistributeRelations;
	#endif
	private: GIAtranslatorReferencingClass GIAtranslatorReferencing;
	private: GIAsemRelTranslatorParserClass GIAsemRelTranslatorParser;
	private: GIAsynRelTranslatorParserClass GIAsynRelTranslatorParser;
	//private: SANIrulesClass SANIrules;
	private: SANInodesClass SANInodes;	
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#ifdef SANI_PARSE_SIMULTANEOUS
	private: SANIposRelTranslatorPermutationsClass SANIposRelTranslatorPermutations;	
	#else
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif
	#endif		
	private: GIAsynRelTranslatorDefineSubstancesClass GIAsynRelTranslatorDefineSubstances;
	private: GIAsynRelTranslatorApplyAdvancedFeaturesClass GIAsynRelTranslatorApplyAdvancedFeatures;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: SHAREDvarsClass SHAREDvars;
	
	public: void createSANItranslatorVariablesFromGIAtranslatorVariables(GIAtranslatorVariablesClass* translatorVariables, SANItranslatorVariablesClass* SANItranslatorVariables);

	public: bool convertSentenceTxtRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);
		private: bool locateAndAddAllNetworkIndexEntitiesBasedOnTxtRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
		private: void identifyComparisonVariable(GIAtranslatorVariablesClass* translatorVariables);
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
		public: bool generateSemanticRelationsFromTxtRelationsWrapperSentences(GIAtranslatorVariablesClass* translatorVariables);
		#endif
			private: bool generateSemanticRelationsFromTxtRelationsWrapper(GIAtranslatorVariablesClass* translatorVariables);
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
				private: bool reconcileSameReferenceSetConnectionsForAllRelationshipEntities(GIAtranslatorVariablesClass* translatorVariables);
				#endif
			private: void setPreprocessorSentenceTranslatorEntityReferences(LRPpreprocessorSentence* currentPreprocessorSentenceInList, vector<GIAentityNode*>* GIAentityNodeArray);

		private: void defineSubstancesBasedOnNetworkAndDeterminerInfo(GIAtranslatorVariablesClass* translatorVariables);

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI_POSTHOC_COLLAPSE_CONJUNCTION_ENTITIES
	private: void collapseConjunctionEntities(GIAtranslatorVariablesClass* translatorVariables);
		public: void deleteRelationshipEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity);
	#endif
	
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	private: bool relinkDynamicConnections(GIAtranslatorVariablesClass* translatorVariables);
		private: bool findIdealSemanticRelationshipSourceInSentenceSubnet(GIAtranslatorVariablesClass* translatorVariables, bool relationshipSourceIsSubject, GIAentityNode* relationshipEntity, GIAentityNode* relationshipSourceEntity, GIAentityNode* relationshipTargetEntity, GIAentityNode** relationshipSourceNewFound);
			private: bool calculateSimilarityOfSubnets(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* relationshipEntity, GIAentityNode* relationshipSourceEntity, GIAentityNode* relationshipTargetEntity, int* subsetSimilarity);
	#endif

	
};

//#endif
#endif

#endif

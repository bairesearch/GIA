 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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

	public: bool convertSentenceTxtRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);
		private: bool locateAndAddAllNetworkIndexEntitiesBasedOnTxtRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
		private: void identifyComparisonVariable(const GIAtranslatorVariablesClass* translatorVariables);
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
		public: bool generateSemanticRelationsFromTxtRelationsWrapperSentences(GIAtranslatorVariablesClass* translatorVariables);
		#endif
			private: bool generateSemanticRelationsFromTxtRelationsWrapper(GIAtranslatorVariablesClass* translatorVariables);
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
				private: bool reconcileSameReferenceSetConnectionsForAllRelationshipEntities(const GIAtranslatorVariablesClass* translatorVariables);
				#endif
			private: void setPreprocessorSentenceTranslatorEntityReferences(LRPpreprocessorSentence* currentPreprocessorSentenceInList, const vector<GIAentityNode*>* GIAentityNodeArray);

		private: void defineSubstancesBasedOnNetworkAndDeterminerInfo(const GIAtranslatorVariablesClass* translatorVariables);

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI_POSTHOC_COLLAPSE_CONJUNCTION_ENTITIES
	private: void collapseConjunctionEntities(GIAtranslatorVariablesClass* translatorVariables);
		public: void deleteRelationshipEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity);
	#endif
	
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	private: bool relinkDynamicConnections(GIAtranslatorVariablesClass* translatorVariables);
		private: bool findIdealSemanticRelationshipSourceInSentenceSubnet(const GIAtranslatorVariablesClass* translatorVariables, const bool relationshipSourceIsSubject, const GIAentityNode* relationshipEntity, const GIAentityNode* relationshipSourceEntity, const GIAentityNode* relationshipTargetEntity, constEffective GIAentityNode** relationshipSourceNewFound);
			private: bool calculateSimilarityOfSubnets(const GIAtranslatorVariablesClass* translatorVariables, const GIAentityNode* relationshipEntity, const GIAentityNode* relationshipSourceEntity, const GIAentityNode* relationshipTargetEntity, const int* subsetSimilarity);
	#endif

	
};

//#endif
#endif

#endif

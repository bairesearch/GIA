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
 * File Name: GIAtranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4d 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef USE_CE
	#include "CECodeextensionClass.h"
#endif
#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
#include "XMLparserClass.hpp"
#endif
#include "GIAtranslatorDefineGrammar.hpp"
#include "GIAtranslatorRedistributeRelations.hpp"
#include "GIAtranslatorRedistributeRelationsRelex.hpp"
#include "GIAtranslatorRedistributeRelationsStanford.hpp"
#include "GIAtranslatorDefineReferencing.hpp"
#include "GIAtranslatorDefineSubstances.hpp"
#include "GIAtranslatorLinkEntities.hpp"
#include "GIAtranslatorLinkEntitiesDynamic.hpp"
#include "GIAtranslatorApplyAdvancedFeatures.hpp"
#include "GIAdatabase.hpp"
#include "GIAnlp.hpp"	//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
#include "GIAnlpParser.hpp"
#include "GIAbot.hpp"
#include "GIAlrp.hpp"
#include "GIAsemanticParserOperations.hpp"
#include "GIAsemanticParserTranslator.hpp"
#include "GIAsemanticParserDatabase.hpp"
#include "GIAtranslatorRules.hpp"
#include "GIAtranslatorGeneric.hpp"
#include "SHAREDvars.hpp"	//file io


class GIAtranslatorClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAlrpClass GIAlrp;
	private: GIAnlpClass GIAnlp;
	private: GIAdatabaseClass GIAdatabase;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAtranslatorDefineReferencingClass GIAtranslatorDefineReferencing;
	#ifdef USE_GIA2
	private: GIAsemanticParserTranslatorClass GIAsemanticParserTranslator;
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	#endif
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	private: GIAtranslatorRedistributeRelationsClass GIAtranslatorRedistributeRelations;
	#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
	private: GIAtranslatorRedistributeRelationsRelexClass GIAtranslatorRedistributeRelationsRelex;
	private: GIAtranslatorRedistributeRelationsStanfordClass GIAtranslatorRedistributeRelationsStanford;
	private: GIAtranslatorDefineSubstancesClass GIAtranslatorDefineSubstances;
	private: GIAtranslatorLinkEntitiesClass GIAtranslatorLinkEntities;
	#endif
	private: GIAtranslatorLinkEntitiesDynamicClass GIAtranslatorLinkEntitiesDynamic;
	private: GIAtranslatorApplyAdvancedFeaturesClass GIAtranslatorApplyAdvancedFeatures;
	private: GIAbotClass GIAbot;
	private: GIAtranslatorRulesClass GIAtranslatorRules;
	private: void initialiseGIATranslatorForTexualContext();

/*
#ifdef USE_CE
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentencesCE(GIAparagraph* firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy);
#endif
*/
	public: bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, const string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName, const string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, const bool isQuery, int NLPfeatureParser, const int NLPdependencyRelationsParser, const bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, const bool parseGIA2file);
	/*
	#ifdef USE_CE
	bool createSemanticNetworkBasedUponDependencyParsedSentencesCE(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy);
	#endif
	*/
		private: bool createSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, const bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, const bool parseGIA2file);
		#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
			private: void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, const unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAparagraph* firstParagraphInList, const int NLPfeatureParser, int NLPdependencyRelationsType, const bool NLPassumePreCollapsedStanfordRelations, const bool parseGIA2file);
		#endif
			#ifdef USE_CE
				private: void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, const CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, const int NLPfeatureParser, int NLPdependencyRelationsType, const bool NLPassumePreCollapsedStanfordRelations);
					private: void convertCEsentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, const unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, const int NLPfeatureParser, int NLPdependencyRelationsType, const bool NLPassumePreCollapsedStanfordRelations);
			#endif
				private: void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, const unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, const int NLPfeatureParser, int NLPdependencyRelationsType, const bool NLPassumePreCollapsedStanfordRelations, const bool parseGIA2file);
					private: void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, const unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, const GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, const int NLPfeatureParser, int NLPdependencyRelationsType, const bool NLPassumePreCollapsedStanfordRelations, const bool parseGIA2file);
						private: void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, const GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, const int NLPfeatureParser, int NLPdependencyRelationsType, const bool NLPassumePreCollapsedStanfordRelations, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);
							private: void disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAentityNode* GIAfeatureTempEntityNodeArray[]);
							private: void disableEntitiesBasedOnFeatureTempEntityNodeArray(const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAentityNode* GIAfeatureTempEntityNodeArray[]);		//added 15 July 2012
						#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
							private: void createParentsOfSubclassEntities(const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAnetworkIndexNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList);
						#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
							private: void createAdditionalSubclassEntities(const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, const int sentenceIndex);
						#endif
								private: void linkSubclassEntitiesWithParentClassEntities(GIAentityNode* subclassNetworkIndexEntity, GIAentityNode* parentClassNetworkIndexEntity, const bool linkAsAlias, const bool onlyLinkIfSubclassOrParentConceptNotFound);
						#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
							private: void detectUserDeclaredSubclassEntities(const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
						#endif
						#endif

			#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
				private: void dreamModeLinkSpecificConceptsAndActions();
				#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
					private: void createAndLinkNonSpecificConceptsForAllEntities(vector<GIAentityNode*>* entityNodesActiveListComplete);
				#endif
			#endif

	//?substance to substance relationship - these they in actual fact represent different levels of detail in information to substance to action / action to action nodes - direct substance to substance relationships are missing the action/connectivity information

#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	private: void invertOrDuplicateConditionsIfRequired(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);
		private: void createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], string inverseConditionName, GIAfeature* featureArrayTemp[]);
#endif
};


#endif

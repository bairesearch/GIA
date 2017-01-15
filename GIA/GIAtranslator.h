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
 * File Name: GIAtranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2l8a 15-August-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorOperations.h"
#ifdef USE_CE
	#include "CECodeextensionClass.h"
#endif
#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
#include "XMLparserClass.h"
#endif

void initialiseGIATranslatorForTexualContext();

/*
#ifdef USE_CE
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentencesCE(GIAparagraph* firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy);
#endif
*/
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, bool parseGIA2file);
	/*
	#ifdef USE_CE
	bool createSemanticNetworkBasedUponDependencyParsedSentencesCE(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy);
	#endif
	*/
	bool createSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAparagraph* firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
		#endif
			#ifdef USE_CE
			void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
				void convertCEsentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
			#endif
			void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
				void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
					void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceConceptEntityNodesList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA, GIACoreference* firstGIACoreferenceInList);
						void disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAentityNode* GIAfeatureTempEntityNodeArray[]);
						void disableEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAentityNode* GIAfeatureTempEntityNodeArray[]);		//added 15 July 2012
			#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
			void dreamModeLinkSpecificConceptsAndActions();
				#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
				void createAndLinkNonSpecificConceptsForAllEntities(vector<GIAentityNode*>* entityNodesActiveListComplete);
				#endif
			#endif

	//?substance to substance relationship - these they in actual fact represent different levels of detail in information to substance to action / action to action nodes - direct substance to substance relationships are missing the action/connectivity information

#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
void invertOrDuplicateConditionsIfRequired(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);
	void createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], string inverseConditionName, GIAfeature* featureArrayTemp[]);
#endif

#endif

/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2c3b 15-January-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR




#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


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

#ifdef USE_CE
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int * maxNumberSentences, bool parseGIA2file, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, bool useCodeextensionsHeirachy);
#else
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int * maxNumberSentences, bool parseGIA2file);
#endif
	#ifdef USE_CE
	bool createSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, bool useCodeextensionsHeirachy);
	#else
	bool createSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
	#endif
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
		#endif
			#ifdef USE_CE
			void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
				void convertCEsentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
			#endif
			void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
				void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file);
					#ifdef GIA_USE_ADVANCED_REFERENCING
					void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA,  GIACoreference * firstGIACoreferenceInList);
					#else
					void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
					#endif
						void disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[]);
						void disableEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[]);		//added 15 July 2012
			#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
			void dreamModeLinkSpecificConceptsAndActions();
			#endif

	//?substance to substance relationship - these they in actual fact represent different levels of detail in information to substance to action / action to action nodes - direct substance to substance relationships are missing the action/connectivity information


#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
#define RULES_XML_TAG_translator ((string)"translator")
	#define RULES_XML_TAG_file ((string)"file")
		#define RULES_XML_ATTRIBUTE_name ((string)"name")
		#define RULES_XML_TAG_function ((string)"function")
			#define RULES_XML_TAG_example ((string)"example")
			#define RULES_XML_TAG_paramDepRel ((string)"paramDepRel")
			#define RULES_XML_TAG_paramEntity ((string)"paramEntity")
				#define RULES_XML_ATTRIBUTE_mode ((string)"mode")
				#define RULES_XML_TAG_option ((string)"option")
					#define RULES_XML_ATTRIBUTE_REL ((string)"REL")
					#define RULES_XML_ATTRIBUTE_REL_ENT ((string)"REL_ENT")
					#define RULES_XML_ATTRIBUTE_FUNC_ENT ((string)"FUNC_ENT")
					#define RULES_XML_ATTRIBUTE_swapIndex ((string)"swapIndex")
				#define RULES_XML_TAG_specialCase ((string)"specialCase")
					#define RULES_XML_ATTRIBUTE_type ((string)"type")
					#define RULES_XML_ATTRIBUTE_variable ((string)"variable")
					#define RULES_XML_ATTRIBUTE_value ((string)"value")
					#define RULES_XML_ATTRIBUTE_arrayIndex ((string)"arrayIndex")
					#define RULES_XML_ATTRIBUTE_negative ((string)"negative")
				#define RULES_XML_TAG_assert ((string)"assert")
	#define GIA_TRANSLATOR_XML_INTERPRETATION_ARRAY_DELIMITER '|'
bool applyGIATranslatorGenericXMLfunctions(string translatorFileName, Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA);
	bool applyGIATranslatorGenericXMLparam(XMLparserTag * currentParamTag, bool depRelOrEntity, bool executeOrReassign, Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA, string functionName);
		bool genericDepRelInterpretationApplyOptions(GIAgenericDepRelInterpretationParameters * paramDepRel, XMLparserTag * xmlTag, int REL, int REL_ENT, int FUNC_ENT, int swapIndex);
			bool genericDepRelInterpretationApplyOption(GIAgenericDepRelInterpretationParameters * paramDepRel, XMLParserAttribute * xmlAttribute, int REL, int REL_ENT, int FUNC_ENT, int swapIndex);
		bool genericEntityInterpretationApplyOptions(GIAgenericEntityInterpretationParameters * paramEntity, XMLparserTag * xmlTag);
			bool genericEntityInterpretationApplyOption(GIAgenericEntityInterpretationParameters * paramEntity, XMLParserAttribute * xmlAttribute);
				bool genericEntityInterpretationApplyOptionbool(bool * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch);
				bool genericEntityInterpretationApplyOptionint(int * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch);
				bool genericEntityInterpretationApplyOptionstring(string * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch);
				bool genericEntityInterpretationApplyOptionstringarray(string ** paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch);
				bool genericInterpretationGenerateSpecialCase(XMLparserTag * xmlTag, EntityCharacteristic * entityCharacteristics, string * type);
				bool genericDepRelInterpretationApplySpecialCase(EntityCharacteristic * entityCharacteristics, GIAgenericDepRelInterpretationParameters * paramDepRel, int REL, int REL_ENT, string type);
				bool genericEntityInterpretationApplySpecialCase(EntityCharacteristic * entityCharacteristics, GIAgenericEntityInterpretationParameters * paramEntity, string type);


#endif

#endif

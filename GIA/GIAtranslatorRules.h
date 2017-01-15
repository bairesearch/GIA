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
 * File Name: GIAtranslatorRules.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2j6e 10-June-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_RULES
#define HEADER_GIA_TRANSLATOR_RULES

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
//#include "GIAtranslatorOperations.h"
#include "GIAtranslatorGeneric.h"

#include "XMLparserClass.h"

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
bool applyGIATranslatorGenericXMLfunctions(string translatorFileName, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, GIAfeature* featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA);
	bool applyGIATranslatorGenericXMLparam(XMLparserTag* currentParamTag, bool depRelOrEntity, bool executeOrReassign, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, GIAfeature* featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA, string functionName);
		bool genericDepRelInterpretationApplyOptions(GIAgenericDepRelInterpretationParameters* paramDepRel, XMLparserTag* xmlTag, int REL, int REL_ENT, int FUNC_ENT, int swapIndex);
			bool genericDepRelInterpretationApplyOption(GIAgenericDepRelInterpretationParameters* paramDepRel, XMLParserAttribute* xmlAttribute, int REL, int REL_ENT, int FUNC_ENT, int swapIndex);
		bool genericEntityInterpretationApplyOptions(GIAgenericEntityInterpretationParameters* paramEntity, XMLparserTag* xmlTag);
			bool genericEntityInterpretationApplyOption(GIAgenericEntityInterpretationParameters* paramEntity, XMLParserAttribute* xmlAttribute);
				bool genericEntityInterpretationApplyOptionbool(bool* paramVal, XMLParserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch);
				bool genericEntityInterpretationApplyOptionint(int* paramVal, XMLParserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch, bool subtractOne);
				bool genericEntityInterpretationApplyOptionstring(string* paramVal, XMLParserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch);
				bool genericEntityInterpretationApplyOptionstringarray(string** paramVal, XMLParserAttribute* xmlAttribute, string iterationVariable, bool* foundMatch);
				bool genericInterpretationGenerateSpecialCase(XMLparserTag* xmlTag, GIAentityCharacteristic* entityCharacteristics, string* type);
				bool genericDepRelInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericDepRelInterpretationParameters* paramDepRel, int REL, int REL_ENT, string type);
				bool genericEntityInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericEntityInterpretationParameters* paramEntity, string type);


#endif

#endif

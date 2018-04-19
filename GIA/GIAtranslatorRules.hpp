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
 * File Name: GIAtranslatorRules.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e11a 21-January-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_RULES
#define HEADER_GIA_TRANSLATOR_RULES

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
//#include "GIAtranslatorOperations.hpp"
#include "GIAtranslatorGeneric.hpp"

#include "XMLparserClass.hpp"
#include "GIAtranslatorDefs.hpp"
#include "XMLrulesClass.hpp"

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
class GIAtranslatorRulesClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorGenericClass GIAtranslatorGeneric;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	public: bool applyGIATranslatorGenericXMLfunctions(const string translatorFileName, GIAtranslatorVariablesClass* translatorVariables);
		private: bool applyGIATranslatorGenericXMLparam(XMLparserTag* currentParamTag, const bool depRelOrEntity, bool executeOrReassign, GIAtranslatorVariablesClass* translatorVariables, const string functionName);
			private: bool genericDepRelInterpretationApplyOptions(GIAgenericDepRelInterpretationParameters* paramDepRel, const XMLparserTag* xmlTag, int REL, int REL_ENT, int FUNC_ENT, int swapIndex);
				private: bool genericDepRelInterpretationApplyOption(GIAgenericDepRelInterpretationParameters* paramDepRel, const XMLparserAttribute* xmlAttribute, int REL, int REL_ENT, int FUNC_ENT, int swapIndex);
			private: bool genericEntityInterpretationApplyOptions(GIAgenericEntityInterpretationParameters* paramEntity, const XMLparserTag* xmlTag);
				private: bool genericEntityInterpretationApplyOption(GIAgenericEntityInterpretationParameters* paramEntity, const XMLparserAttribute* xmlAttribute);
					private: bool genericEntityInterpretationApplyOptionbool(bool* paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch);
					private: bool genericEntityInterpretationApplyOptionint(int* paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch, const bool subtractOne);
					private: bool genericEntityInterpretationApplyOptionstring(string* paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch);
					private: bool genericEntityInterpretationApplyOptionstringarray(string** paramVal, const XMLparserAttribute* xmlAttribute, const string iterationVariable, bool* foundMatch);
					private: bool genericInterpretationGenerateSpecialCase(XMLparserTag* xmlTag, GIAentityCharacteristic* entityCharacteristics, string* type);
					private: bool genericDepRelInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericDepRelInterpretationParameters* paramDepRel, int REL, int REL_ENT, const string type);
					private: bool genericEntityInterpretationApplySpecialCase(GIAentityCharacteristic* entityCharacteristics, GIAgenericEntityInterpretationParameters* paramEntity, const string type);
};


#endif

#endif

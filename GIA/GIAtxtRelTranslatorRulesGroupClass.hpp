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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f7a 17-April-2018
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Rules
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_RULES_GROUP_CLASS
#define HEADER_GIA_TXT_REL_TRANSLATOR_RULES_GROUP_CLASS

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorWordClass.hpp"

//TODO: rename COMPONENT to GROUP TYPE:
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN (0)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_LOGICREFERENCESET (1)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_REFERENCESET (2)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SUBREFERENCESET (3)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES (4)
static string GIAtxtRelTranslatorRulesGroupsComponentReferenceSetTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSet", "referenceSet", "subReferenceSet"};


#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_UNKNOWN (0)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS (1)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS (2)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETS (3)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETPARTS (4)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETAPPENDS (5)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETSOPTIONAL (6)
#define GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES (7)	//this is just a selection of prominent groupTypes (it doesn't include all of them)
static string GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSets", "referenceSets", "subReferenceSets", "subReferenceSetsPart", "subReferenceSetsAppend", "logicReferenceSetsOptional"};



class GIAtxtRelTranslatorRulesComponent;

class GIAtxtRelTranslatorRulesGroup
{
public:
	GIAtxtRelTranslatorRulesGroup(void);
	~GIAtxtRelTranslatorRulesGroup(void);

	vector<string> semanticRelationFunctionName;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	string semanticRelationFunctionConditionNewName;
	#endif
	string groupName;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	string previousWordPOStype;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_EXISTS_PRECEEDING_WORD_POS_TYPE
	string existsPreceedingWordPOStype;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordVerbVariantTypeDerived;
	int wordNounVariantTypeDerived;	
	#endif
		
	string groupTypeNameBackup;
	
	int numberOfConsecutiveTimesPreviousGroupType;
								
	vector<GIAtxtRelTranslatorRulesComponent*> components;
};

class GIAtxtRelTranslatorRulesGroupType
{
public:
	GIAtxtRelTranslatorRulesGroupType(void);
	~GIAtxtRelTranslatorRulesGroupType(void);

	string groupTypeName;
	int referenceSetType;
		
	vector<GIAtxtRelTranslatorRulesGroup*> groups;
};

class GIAtxtRelTranslatorRulesGroupClass
{
	public: void copyParseGroupInfo(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup);
};




#endif

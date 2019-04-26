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
 * File Name: GIAbot.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3h3a 24-April-2019
 * Requirements: 
 * Description: Bot
 * /
 *******************************************************************************/


#include "GIAbot.hpp"

#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
void GIAbotClass::botSwitchFirstAndSecondPersonSyntactic(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passedFirstPersonGovernor = false;
			bool passedFirstPersonDependent = false;
			for(int i=0; i<FEATURE_FIRST_PERSON_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationGovernor == featureFirstPersonNameArray[i])
				{
					GIAentityNode* firstPersonEntity = (*translatorVariables->GIAfeatureTempEntityNodeArray)[currentRelationInList->relationGovernorIndex];
					currentRelationInList->relationGovernor = featureSecondPersonNameArray[i];
					firstPersonEntity->entityName = featureSecondPersonNameArray[i];
					passedFirstPersonGovernor = true;
				}
				if(currentRelationInList->relationDependent == featureFirstPersonNameArray[i])
				{
					GIAentityNode* firstPersonEntity = (*translatorVariables->GIAfeatureTempEntityNodeArray)[currentRelationInList->relationDependentIndex];
					currentRelationInList->relationDependent = featureSecondPersonNameArray[i];
					firstPersonEntity->entityName = featureSecondPersonNameArray[i];
					passedFirstPersonDependent = true;
				}
			}
			for(int i=0; i<FEATURE_SECOND_PERSON_NUMBER_OF_TYPES; i++)
			{
				if(!passedFirstPersonGovernor)
				{
					if(currentRelationInList->relationGovernor == featureSecondPersonNameArray[i])
					{
						GIAentityNode* secondPersonEntity = (*translatorVariables->GIAfeatureTempEntityNodeArray)[currentRelationInList->relationGovernorIndex];
						currentRelationInList->relationGovernor = featureFirstPersonNameArray[i];
						secondPersonEntity->entityName = featureFirstPersonNameArray[i];
					}
				}
				if(!passedFirstPersonDependent)
				{
					if(currentRelationInList->relationDependent == featureSecondPersonNameArray[i])
					{
						GIAentityNode* secondPersonEntity = (*translatorVariables->GIAfeatureTempEntityNodeArray)[currentRelationInList->relationDependentIndex];
						currentRelationInList->relationDependent = featureFirstPersonNameArray[i];
						secondPersonEntity->entityName = featureFirstPersonNameArray[i];
					}
				}
			}
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


void GIAbotClass::botSwitchFirstAndSecondPersonSemantic(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAsentence* currentSentenceInList = translatorVariables->currentSentenceInList;
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passedFirstPersonGovernor = false;
			bool passedFirstPersonDependent = false;
			for(int i=0; i<FEATURE_FIRST_PERSON_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationGovernor == featureFirstPersonNameArray[i])
				{
					currentRelationInList->relationGovernor = featureSecondPersonNameArray[i];
					passedFirstPersonGovernor = true;
				}
				if(currentRelationInList->relationDependent == featureFirstPersonNameArray[i])
				{
					currentRelationInList->relationDependent = featureSecondPersonNameArray[i];
					passedFirstPersonDependent = true;
				}
			}
			for(int i=0; i<FEATURE_SECOND_PERSON_NUMBER_OF_TYPES; i++)
			{
				if(!passedFirstPersonGovernor)
				{
					if(currentRelationInList->relationGovernor == featureSecondPersonNameArray[i])
					{
						currentRelationInList->relationGovernor = featureFirstPersonNameArray[i];
					}
				}
				if(!passedFirstPersonDependent)
				{
					if(currentRelationInList->relationDependent == featureSecondPersonNameArray[i])
					{
						currentRelationInList->relationDependent = featureFirstPersonNameArray[i];
					}
				}
			}
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
#ifndef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
void GIAbotClass::botSwitchFirstAndSecondPersonTxt(GIAtranslatorVariablesClass* translatorVariables)
{
	int numberOfWordsInSentence = GIApreprocessorSentenceClassObject.getSentenceContents(translatorVariables->currentPreprocessorSentenceInList)->size();
	vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(translatorVariables->currentPreprocessorSentenceInList);
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = (*sentenceContents)[w];
		for(int i=0; i<FEATURE_FIRST_PERSON_NUMBER_OF_TYPES; i++)
		{
			if(currentWord->translatorEntity->entityName == featureFirstPersonNameArray[i])
			{
				currentWord->translatorEntity->entityName = featureSecondPersonNameArray[i];
			}
		}
		for(int i=0; i<FEATURE_SECOND_PERSON_NUMBER_OF_TYPES; i++)
		{
			if(currentWord->translatorEntity->entityName == featureSecondPersonNameArray[i])
			{
				currentWord->translatorEntity->entityName = featureFirstPersonNameArray[i];
			}
		}
	}
}
#endif
#endif

#endif

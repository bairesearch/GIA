/*******************************************************************************
 *
 * File Name: GIAnlg.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n4c 23-July-2012
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
 *******************************************************************************/



#include "GIAnlg.h"

NLGSentence::NLGSentence(void)
{
	NLGInputViewText = "";
	
	next = NULL;
}

NLGSentence * generateLanguageFromEntityNode(GIAEntityNode * entityNode, NLGSentence * currentNLGsentence)
{
	NLGSentence * currentNLGsentenceUpdated = currentNLGsentence;
	if(!(entityNode->parsedForLanguageGeneration) && !(entityNode->disabled))
	{
		#ifdef GIA_NLG_DEBUG
		if(entityNode->isProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (is property)" << endl;
		}
		else if(entityNode->isAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;
		}
		else if(entityNode->isCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (is condition)" << endl;
		}
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << endl;
		}
		#endif



		//cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		//cout << "\tentityNode->isProperty = " << entityNode->isProperty << endl;
		//cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
		//cout << "\tentityNode->hasAssociatedInstanceIsAction = " << entityNode->hasAssociatedInstanceIsAction << endl;


		entityNode->parsedForLanguageGeneration = true;

 		bool supportAdditionalLinks = false;
		#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS
		supportAdditionalLinks = true;
		#endif
		if(entityNode->isAction)
		{
			generateThreeEntitySentenceFromEntityNode(entityNode, &(currentNLGsentenceUpdated->NLGInputViewText), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, 1, supportAdditionalLinks);
			generateNLGInputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));	
			NLGSentence * newNLGsentence = new NLGSentence();
			currentNLGsentenceUpdated->next = newNLGsentence;	
			currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;			
		}
		else if(entityNode->isCondition)
		{
			generateThreeEntitySentenceFromEntityNode(entityNode, &(currentNLGsentenceUpdated->NLGInputViewText), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, 1, supportAdditionalLinks);
			generateNLGInputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));		
			NLGSentence * newNLGsentence = new NLGSentence();
			currentNLGsentenceUpdated->next = newNLGsentence;	
			currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;			
		}	
				
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
			{
				if(nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[i])
				{
					generateTwoEntitySentenceFromEntityConnection(entityNode, *connectionIter, &(currentNLGsentenceUpdated->NLGInputViewText), i, 1);
					generateNLGInputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));
					NLGSentence * newNLGsentence = new NLGSentence();
					currentNLGsentenceUpdated->next = newNLGsentence;	
					currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;				
				}
								
				currentNLGsentenceUpdated = generateLanguageFromEntityNode((*connectionIter)->entity, currentNLGsentenceUpdated);
			}
		}

		//cout << "a8" << endl;

		#ifdef GIA_NLG_DEBUG
		if(entityNode->isProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is property)" << endl;
		}
		else if(entityNode->isAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;
		}
		else if(entityNode->isCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is condition)" << endl;
		}
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode->hasAssociatedTime)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << endl;
		}
		#endif

	}
	return currentNLGsentenceUpdated;
	//cout << "a0c" << endl;
}


//startEntityIndex should be 1 for first calcs...
void generateThreeEntitySentenceFromEntityNode(GIAEntityNode * entityNode0, string * generatedText, int connectionType1, int connectionType2, int startEntityIndex, bool supportAdditionalLinks)
{
	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode: ";
	#endif
	
	int entityIndex0 = startEntityIndex + 0;
	int entityIndex1 = startEntityIndex + 1;
	int entityIndex2 = startEntityIndex + 2;
	char entityIndex0stringCharstar[10];
	char entityIndex1stringCharstar[10];
	char entityIndex2stringCharstar[10];
	sprintf(entityIndex0stringCharstar, "%d", entityIndex0);
	sprintf(entityIndex1stringCharstar, "%d", entityIndex1);
	sprintf(entityIndex2stringCharstar, "%d", entityIndex2);
	string entityIndex0string = entityIndex0stringCharstar;
	string entityIndex1string = entityIndex1stringCharstar;
	string entityIndex2string = entityIndex2stringCharstar;
		
	GIAEntityNode * entityNode1 = (entityNode0->entityVectorConnectionsArray[connectionType1]).back()->entity;
	GIAEntityNode * entityNode2 = (entityNode0->entityVectorConnectionsArray[connectionType2]).back()->entity;

	generateNLGInputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);	
	generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);	
	generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);
	#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS
	if(supportAdditionalLinks)
	{
		//bool entityNodeHasAdditionalConnection[3][NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS];
		//int entityNodeHasAdditionalConnectionIndex[3][NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS];
		GIAEntityNode * entityNodeArray[3];	
		entityNodeArray[entityIndex0] = entityNode0;
		entityNodeArray[entityIndex1] = entityNode1;
		entityNodeArray[entityIndex2] = entityNode2;
		bool entityNode2HasCondition = false;
		int currentEntityIndexAdditional = entityIndex2 + 1;
		for(int i=0; i<NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS; i++)
		{
			for(int entityNodeIndex=0; entityNodeIndex<3; entityNodeIndex++)
			{	
				//entityNodeHasAdditionalConnection[entityNodeIndex][i] = false;
				int vectorConnectionIndex = nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i];
				if(entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() == 1)
				{//entity contains single property or condition
					GIAEntityConnection * entityConnectionAdditional = entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].back();
					GIAEntityNode * entityNodeAdditional = entityConnectionAdditional->entity;
					
					bool passAdditionalExtraRequirements = false;
					if(vectorConnectionIndex == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)
					{
						if(entityNodeAdditional->hasQuality)
						{
							passAdditionalExtraRequirements = true;
						}	
					}
					else
					{
						passAdditionalExtraRequirements = true;
					}
					
					if(passAdditionalExtraRequirements)
					{
						//entityNodeHasAdditionalConnection[entityNodeIndex][i] = true;
						//entityNodeHasAdditionalConnectionIndex[entityNodeIndex][i] = currentEntityIndexAdditional;					
						if(nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i])
						{
							generateThreeEntitySentenceFromEntityNode(entityNodeAdditional, generatedText, nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i], nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i]+1, currentEntityIndexAdditional, false);
						}
						else
						{
							generateTwoEntitySentenceFromEntityConnection(entityNodeArray[entityNodeIndex], entityConnectionAdditional, generatedText, nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i], currentEntityIndexAdditional);			
						}

						string nlgDependencyRelation1 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType1], entityIndex0string, entityIndex1string);										

						currentEntityIndexAdditional++;
					}
				}
			}		
		}
	}	
	#endif

	string nlgDependencyRelation1 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType1], entityIndex0string, entityIndex1string);										
	string nlgDependencyRelation2 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType2], entityIndex0string, entityIndex2string);										

	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode():" << endl;
	cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
	cout << "nlgDependencyRelation2 = " << nlgDependencyRelation2 << endl;
	#endif
	*generatedText = *generatedText + nlgDependencyRelation1;
	*generatedText = *generatedText + nlgDependencyRelation2;
	
	
	bool endOfSentence = true;
	#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS
	if(!supportAdditionalLinks)
	{
		endOfSentence = false;
	}
	#endif	
		
	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode: Exiting";
	#endif
}

void generateTwoEntitySentenceFromEntityConnection(GIAEntityNode * entityNode1, GIAEntityConnection * entityConnection, string * generatedText, int connectionType, int startEntityIndex)
{
	//if(!(entityConnection->parsedForLanguageGeneration))	//&& !(entityConnection->disabled)
	//{	
		int entityIndex1 = startEntityIndex + 1;
		int entityIndex2 = startEntityIndex + 2;
		int entityIndex0 = startEntityIndex + 0;
		char entityIndex1stringCharstar[10];
		char entityIndex2stringCharstar[10];
		char entityIndex0stringCharstar[10];
		sprintf(entityIndex1stringCharstar, "%d", entityIndex1);
		sprintf(entityIndex2stringCharstar, "%d", entityIndex2);
		sprintf(entityIndex0stringCharstar, "%d", entityIndex0);
		string entityIndex1string = entityIndex1stringCharstar;
		string entityIndex2string = entityIndex2stringCharstar;
		string entityIndex0string = entityIndex0stringCharstar;
				
		GIAEntityNode * entityNode2 = entityConnection->entity;

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection: ";
		if(entityNode2->isProperty)
		{
			cout << "entityNode2 = " << entityNode2->entityName << " (is property)" << endl;
		}
		else if(entityNode2->isAction)
		{
			cout << "entityNode2 = " << entityNode2->entityName << " (is action)" << endl;
		}
		else if(entityNode2->isCondition)
		{
			cout << "entityNode2 = " << entityNode2->entityName << " (is condition)" << endl;
		}
		else if(entityNode2->hasAssociatedInstance)
		{
			cout << "entityNode2 = " << entityNode2->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode2->hasAssociatedInstanceIsAction)
		{
			cout << "entityNode2 = " << entityNode2->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode2->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode2 = " << entityNode2->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode2->hasAssociatedTime)
		{
			cout << "entityNode2 = " << entityNode2->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode2 = " << entityNode2->entityName << endl;
		}
		#endif

		
		bool generateTwoDependencyRelations = false;
		string nlgDependencyRelationType1 = "";	
		string nlgDependencyRelationType2 = "";	
		string nlgDependencyRelationSharedArgument = "";
		
		if(entityNode1->isAction)
		{
			cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode1->isAction)" << endl;
		}	
		else if(entityNode1->isCondition)
		{
			cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode1->isCondition)" << endl;
		}
				
		if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)
		{			
			if(entityNode1->isProperty) 
			{
				#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
				bool hasQuality = false;
				if(entityNode1->hasQuality)
				{
					hasQuality = true;
					bool propertyOwnerIsAction = false;
					if(!(entityNode1->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES].empty()))
					{
						GIAEntityNode * propertyOwner = entityNode1->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES].back()->entity;
						if(propertyOwner->isAction)
						{
							propertyOwnerIsAction = true;
							nlgDependencyRelationType1 = RELATION_TYPE_ADJECTIVE_ADVMOD;
						}
					}
					if(!propertyOwnerIsAction)
					{
						nlgDependencyRelationType1 = RELATION_TYPE_ADJECTIVE_AMOD;		//or RELATION_TYPE_ADJECTIVE_PREDADJ?	
					}
				}
				if(!hasQuality)
				{
				#endif
					//eg possessive and/or tom has a bike	
					#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS				
					generateTwoDependencyRelations = true;
					nlgDependencyRelationType1 = RELATION_TYPE_SUBJECT;
					nlgDependencyRelationType2 = RELATION_TYPE_OBJECT;
					nlgDependencyRelationSharedArgument = RELATION_ENTITY_HAVE;
					#else
					nlgDependencyRelationType1 = RELATION_TYPE_POSSESSIVE;
					#endif
				#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
				}
				#endif
			}	
			else
			{//isConcept
				cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode1->isConcept) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)" << endl;
				nlgDependencyRelationType1 = RELATION_TYPE_SUBJECT;
			}
		}
		else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS)
		{
			#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS
			generateTwoDependencyRelations = true;
			nlgDependencyRelationType1 = RELATION_TYPE_SUBJECT;
			nlgDependencyRelationType2 = RELATION_TYPE_OBJECT;
			nlgDependencyRelationSharedArgument = RELATION_ENTITY_BE;		
			#else
			nlgDependencyRelationType1 = RELATION_TYPE_APPOSITIVE_OF_NOUN;
			#endif
		}
		
		if(generateTwoDependencyRelations)
		{
			GIAEntityNode * entityNode0 = new GIAEntityNode();
			entityNode0->entityName = nlgDependencyRelationSharedArgument;
			entityNode0->isAction = true;	//NB "have"/"be" are interpreted as actions/verbs by NLP/stanford/relex (but reduced by GIA)
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);			
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);		
			string nlgDependencyRelation1 = generateNLGInputViewLine(nlgDependencyRelationType1, entityIndex0string, entityIndex1string);										
			string nlgDependencyRelation2 = generateNLGInputViewLine(nlgDependencyRelationType2, entityIndex0string, entityIndex2string);											
		
			#ifdef GIA_NLG_DEBUG
			cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;			
			cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
			cout << "nlgDependencyRelation2 = " << nlgDependencyRelation2 << endl;
			#endif
			*generatedText = *generatedText + nlgDependencyRelation1;
			*generatedText = *generatedText + nlgDependencyRelation2;	
			
			delete entityNode0;	
		}
		else
		{
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);	
			string nlgDependencyRelation1 = generateNLGInputViewLine(nlgDependencyRelationType1, entityIndex1string, entityIndex2string);	
		
			#ifdef GIA_NLG_DEBUG
			cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;	
			cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
			#endif
			*generatedText = *generatedText + nlgDependencyRelation1;
			
		}

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection: ";
		if(entityNode2->isProperty)
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << " (is property)" << endl;
		}
		else if(entityNode2->isAction)
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << " (is action)" << endl;
		}
		else if(entityNode2->isCondition)
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << " (is condition)" << endl;
		}
		else if(entityNode2->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << " (has associated property)" << endl;
		}
		else if(entityNode2->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode2->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode2->hasAssociatedTime)
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "Exiting: entityNode2 = " << entityNode2->entityName << endl;
		}
		#endif		
		
	//}
}

void generateNLGInputViewFeatureTagsGenericPerSentence(string * generatedRelexTags)
{
	string NLGInputViewFeatureTagPosDeterminateDefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_DEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										
	string NLGInputViewFeatureTagPosDeterminateIndefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_INDEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										
	string NLGInputViewFeatureTagPosPunctuationFullstop = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_FULLSTOP, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FULLSTOP);	

	#ifdef GIA_NLG_DEBUG
	cout << "generateNLGInputViewFeatureTagsGenericPerSentence():" << endl;	
	cout << "NLGInputViewFeatureTagPosDeterminateDefinite = " << NLGInputViewFeatureTagPosDeterminateDefinite << endl;
	cout << "NLGInputViewFeatureTagPosDeterminateIndefinite = " << NLGInputViewFeatureTagPosDeterminateIndefinite << endl;
	cout << "NLGInputViewFeatureTagPosPunctuationFullstop = " << NLGInputViewFeatureTagPosPunctuationFullstop << endl;
	#endif
		
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagPosDeterminateDefinite;
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagPosDeterminateIndefinite;
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagPosPunctuationFullstop;
}

void generateNLGInputViewFeatureTagsFromEntityNode(GIAEntityNode * entityNode, int entityIndex, string * generatedRelexTags)
{
	char entityIndexStringCharstar[10];
	sprintf(entityIndexStringCharstar, "%d", entityIndex);
	string entityIndexString = entityIndexStringCharstar;
	
	//lemma
	#ifdef GIA_USE_NLG_BUG_IN_NLG2_REQUIRES_ORIG_WORD_NOT_LEMMA
	string NLGInputViewFeatureTagLemma = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_LEMMA, entityIndexString, entityNode->wordOrig);			//IMPORTANT; due to bug in nlg2, nlg2 currently requires the original word, not the lemma	
	#else
	string NLGInputViewFeatureTagLemma = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_LEMMA, entityIndexString, entityNode->entityName);						
	#endif
	
	//tense
	//string wordName = entityNode->entityName;
	string NLGInputViewFeatureTagTense = "";
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		GIATimeConditionNode * timeCondition = entityNode->timeConditionNode;

		if(timeCondition->tenseOnlyTimeCondition)
		{
			string tenseString = "";
			if(timeCondition->tense != GRAMMATICAL_TENSE_UNDEFINED)
			{
				//GRAMMATICAL_TENSE_PRESENT, GRAMMATICAL_TENSE_PAST, GRAMMATICAL_TENSE_FUTURE
				tenseString = grammaticalTenseNameArray[timeCondition->tense];
			}
			if(timeCondition->isProgressive)
			{
				tenseString = tenseString + GRAMMATICAL_TENSE_CONCATONATOR_RELEX + grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE];
			}
			/*
			if(timeCondition->isImperative)
			{
				tenseString = tenseString + GRAMMATICAL_TENSE_CONCATONATOR_RELEX + grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE];
			}
			*/				

			//eg tense(decide, past)
			string NLGInputViewFeatureTagTense = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_TENSE, entityIndexString, tenseString);							
		}
		else
		{
			//wordName = wordName + "(" + timeCondition->conditionName + ")";
		}	
	}

	//noun_number
	string nounNumberString = "";
	string NLGInputViewFeatureTagNounNumber = "";
	if((entityNode->grammaticalNumber != GRAMMATICAL_NUMBER_UNDEFINED) && (entityNode->grammaticalNumber != GRAMMATICAL_NUMBER_UNCOUNTABLE))
	{
		nounNumberString = grammaticalNumberNameArray[entityNode->grammaticalNumber];
		NLGInputViewFeatureTagNounNumber = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_NOUNNUMBER, entityIndexString, nounNumberString);
	}
											
	//pos + inflection
	string inflectionString = "";	
	string posString = "";
	bool isDefinite = false;	
	if(entityNode->isAction)
	{
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_VERB];
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_VERB];
	}	
	else if(entityNode->isCondition)
	{
		//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_PREP];	//no inflection for prepositions	
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_PREP];	
	}
	else if(entityNode->isProperty) 
	{
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
		bool hasQuality = false;
		if(entityNode->hasQuality)
		{
			hasQuality = true;
			bool propertyOwnerIsAction = false;
			if(!(entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES].empty()))
			{
				GIAEntityNode * propertyOwner = entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES].back()->entity;
				if(propertyOwner->isAction)
				{
					propertyOwnerIsAction = true;
					//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADV];	//no inflection for adjectives?					
					posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADV];
				}
			}
			if(!propertyOwnerIsAction)
			{
				//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADJ];	//no inflection for adverbs?			
				posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADJ];
			}
		}
		if(!hasQuality)
		{
		#endif
			inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];		
			posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
			isDefinite = true;
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
		}
		#endif
	}	
	else
	{//isConcept
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];	
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
	}
	string NLGInputViewFeatureTagInflection = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_INFLECTION, entityIndexString, inflectionString);	
	string NLGInputViewFeatureTagPos = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, entityIndexString, posString);
	
	//flag definite
	string NLGInputViewFeatureTagFlagDefinite = "";
	if(isDefinite)
	{
		NLGInputViewFeatureTagFlagDefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_DEFINITE, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_DEFINITE);
	}
	
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	//gender
	bool isPerson = false;
	bool hasGender = false;
	string genderString = "";
	string NLGInputViewFeatureTagFlagPerson = "";
	string NLGInputViewFeatureTagGender = "";
	if(entityNode->grammaticalGenderTemp != GRAMMATICAL_GENDER_UNDEFINED)
	{
		//relex compatible only
		if(entityNode->grammaticalGenderTemp == GRAMMATICAL_GENDER_PERSON)
		{
			isPerson = true;
		}
		else
		{
			genderString = grammaticalGenderNameArray[entityNode->grammaticalGenderTemp];
			hasGender = true;
		}
	}
	else
	{
		//stanford/relex compatible
		//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford()	[see GIATranslatorDefineGrammar.cpp]
		if(entityNode->grammaticalRelexPersonOrStanfordProperNounTemp)
		{
			isPerson = true;
		}
		
	}
	if(hasGender)
	{
		NLGInputViewFeatureTagGender = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_GENDER, entityIndexString, genderString);										
	}
		
	//flag person
	if(isPerson)
	{
		NLGInputViewFeatureTagFlagPerson = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_PERSON, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_PERSON);	
	}
	#endif
	
	
	
	#ifdef GIA_NLG_DEBUG
	cout << "generateNLGInputViewFeatureTagsFromEntityNode():" << endl;
	cout << "NLGInputViewFeatureTagLemma = " << NLGInputViewFeatureTagLemma << endl;
	cout << "NLGInputViewFeatureTagFlagDefinite = " << NLGInputViewFeatureTagFlagDefinite << endl;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	cout << "NLGInputViewFeatureTagGender = " << NLGInputViewFeatureTagGender << endl;		
	#endif
	cout << "NLGInputViewFeatureTagTense = " << NLGInputViewFeatureTagTense << endl;
	cout << "NLGInputViewFeatureTagNounNumber = " << NLGInputViewFeatureTagNounNumber << endl;
	cout << "NLGInputViewFeatureTagInflection = " << NLGInputViewFeatureTagInflection << endl;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	cout << "NLGInputViewFeatureTagFlagPerson = " << NLGInputViewFeatureTagFlagPerson << endl;																			
	#endif
	cout << "NLGInputViewFeatureTagPos = " << NLGInputViewFeatureTagPos << endl;				
	#endif
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagLemma;
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagFlagDefinite;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagGender;		
	#endif
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagTense;
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagNounNumber;		
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagInflection;	
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagFlagPerson;		
	#endif
	*generatedRelexTags = *generatedRelexTags + NLGInputViewFeatureTagPos;
	
}	
		



string generateNLGInputViewLine(string type, string governor, string dependent)
{
	#ifdef GIA_NLG_DEBUG_MANUALLY_HARDCODE_INTO_NLG2
	string NLGInputViewLine = "\"" + type + "(" + governor + ", " + dependent + ")" + "\\n\" +" + "\n";	
	#else
	string NLGInputViewLine = type + "(" + governor + ", " + dependent + ")" + "\n";
	#endif
	return NLGInputViewLine;
}

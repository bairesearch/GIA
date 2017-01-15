/*******************************************************************************
 *
 * File Name: GIAnlg.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n4f 25-July-2012
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

		#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN
		if(!(entityNode->definiteSourceAddedInLanguageGeneration))
		{
		#endif
 			bool supportAdditionalLinks = true;
			if(entityNode->isAction)
			{
				generateThreeEntitySentenceFromEntityNode(entityNode, &(currentNLGsentenceUpdated->NLGInputViewText), GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, 1, supportAdditionalLinks);
				#ifdef GIA_USE_NLG2
				NLG2generateNLGInputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));	
				#endif
				NLGSentence * newNLGsentence = new NLGSentence();
				currentNLGsentenceUpdated->next = newNLGsentence;	
				currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;			
			}
			else if(entityNode->isCondition)
			{
				//cout << "a6" << endl;
				generateThreeEntitySentenceFromEntityNode(entityNode, &(currentNLGsentenceUpdated->NLGInputViewText), GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, 1, supportAdditionalLinks);
				#ifdef GIA_USE_NLG2
				NLG2generateNLGInputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));		
				#endif
				NLGSentence * newNLGsentence = new NLGSentence();
				currentNLGsentenceUpdated->next = newNLGsentence;	
				currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;			
			}	
		#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN
		}
		#endif	

		//cout << "a7" << endl;
				
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
			{
				if(nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[i])
				{
					#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN
					if(!((*connectionIter)->entity->definiteSourceAddedInLanguageGeneration))
					{
					#endif
						generateTwoEntitySentenceFromEntityConnection(entityNode, *connectionIter, &(currentNLGsentenceUpdated->NLGInputViewText), i, 1, false);

						#ifdef GIA_USE_NLG2
						NLG2generateNLGInputViewFeatureTagsGenericPerSentence(&(currentNLGsentenceUpdated->NLGInputViewText));
						#endif
						NLGSentence * newNLGsentence = new NLGSentence();
						currentNLGsentenceUpdated->next = newNLGsentence;	
						currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;	
					
					#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN
					}
					#endif								
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
	cout << "generateThreeEntitySentenceFromEntityNode: " << entityNode0->entityName << endl;
	if(supportAdditionalLinks)
	{
		cout << "supportAdditionalLinks" << endl;
	}
	#endif
	
	GIAEntityNode * entityNode1 = NULL;
	GIAEntityNode * entityNode2 = NULL;
	bool entityNodeAvailableArray[3];
	entityNodeAvailableArray[0] = true;
	entityNodeAvailableArray[1] = false;
	entityNodeAvailableArray[2] = false;
	string determinateArray[3];
	if(!(entityNode0->entityVectorConnectionsArray[connectionType1]).empty())
	{
		entityNode1 = (entityNode0->entityVectorConnectionsArray[connectionType1]).back()->entity;
		entityNodeAvailableArray[1] = true;
	}
	//cout << "af2" << endl;
	if(!(entityNode0->entityVectorConnectionsArray[connectionType2]).empty())
	{
		entityNode2 = (entityNode0->entityVectorConnectionsArray[connectionType2]).back()->entity;
		entityNodeAvailableArray[2] = true;
	}
	
	if(entityNodeAvailableArray[0])
	{	
		determinateArray[0] = calcDeterminate(entityNode0);	
	}
	if(entityNodeAvailableArray[1])
	{
		determinateArray[1] = calcDeterminate(entityNode1);
	}
	if(entityNodeAvailableArray[2])
	{	
		determinateArray[2] = calcDeterminate(entityNode2);	
	}
		
	//cout << "af3" << endl;
	
#ifdef GIA_USE_NLG2
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
	
	NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);
	if(entityNodeAvailableArray[1])
	{	
		NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);	
	}
	if(entityNodeAvailableArray[2])
	{
		NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);
	}
#else
	string entityTextExpandedArray[3];
	for(int i=0; i<3; i++)
	{
		entityTextExpandedArray[i] = "";
	}
	entityTextExpandedArray[0] = getWordOrig(entityNode0);
	if(entityNodeAvailableArray[1])
	{
		entityTextExpandedArray[1] = getWordOrig(entityNode1);
	}
	if(entityNodeAvailableArray[2])
	{
		entityTextExpandedArray[2] = getWordOrig(entityNode2);	
	}
#endif
	//cout << "af4" << endl;
	
	#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS
	if(supportAdditionalLinks)
	{
		//bool entityNodeHasAdditionalConnection[3][NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS];
		//int entityNodeHasAdditionalConnectionIndex[3][NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS];
		GIAEntityNode * entityNodeArray[3];	
		entityNodeArray[0] = entityNode0;
		entityNodeArray[1] = entityNode1;
		entityNodeArray[2] = entityNode2;
		bool entityNode2HasCondition = false;
		#ifdef GIA_USE_NLG2
		int currentEntityIndexAdditional = entityIndex2 + 1;
		#else
		int currentEntityIndexAdditional = GIA_NLG_INDEX_IRRELEVANT;		
		#endif
		
		//cout << "af5" << endl;
		for(int i=0; i<NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS; i++)
		{
			//cout << "af6" << endl;
			for(int entityNodeIndex=0; entityNodeIndex<3; entityNodeIndex++)
			{	
				if(entityNodeAvailableArray[entityNodeIndex])
				{
					//cout << "af7" << endl;
					//entityNodeHasAdditionalConnection[entityNodeIndex][i] = false;
					int vectorConnectionIndex = nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i];
					if((connectionType1 != nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[0]) && (connectionType2 != nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[1]))
					{//do not add double conditions/prepositions...
						//cout << "af8" << endl;
						/*
						if((entityNodeIndex == 2) && (vectorConnectionIndex == GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT))
						{
							cout << "totem:" << entityNodeArray[entityNodeIndex]->entityName << endl;
							cout << "entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() = " << entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() << endl;
						}
						*/
						
						if(entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() == 1)
						{//entity contains single property or condition
							//cout << "af9" << endl;
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

							//cout << "af10" << endl;

							if(passAdditionalExtraRequirements)
							{
								//cout << "af11" << endl;
								//entityNodeHasAdditionalConnection[entityNodeIndex][i] = true;
								//entityNodeHasAdditionalConnectionIndex[entityNodeIndex][i] = currentEntityIndexAdditional;					
								if(nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i])
								{
									//cout << "entityNodeIndex = " << entityNodeIndex << endl;
									//cout << "vectorConnectionIndex = " << vectorConnectionIndex << endl;
									//cout << "entityNodeAdditional->entityName = " << entityNodeAdditional->entityName << endl;
									//cout << "af12" << endl;
									#ifdef GIA_USE_NLG2
									generateThreeEntitySentenceFromEntityNode(entityNodeAdditional, generatedText, nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i], nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i]+1, currentEntityIndexAdditional, false);
									#else
									generateThreeEntitySentenceFromEntityNode(entityNodeAdditional, &(entityTextExpandedArray[entityNodeIndex]), nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[0], nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[1], currentEntityIndexAdditional, false);
									#endif
								}
								else
								{
									//cout << "af13" << endl;
									#ifdef GIA_USE_NLG2
									generateTwoEntitySentenceFromEntityConnection(entityNodeArray[entityNodeIndex], entityConnectionAdditional, generatedText, nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i], currentEntityIndexAdditional, true);
									#else
									generateTwoEntitySentenceFromEntityConnection(entityNodeArray[entityNodeIndex], entityConnectionAdditional, &(entityTextExpandedArray[entityNodeIndex]), nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i], currentEntityIndexAdditional, true);
									#endif			
								}

								currentEntityIndexAdditional++;
							}
						}
					}
				}
			}		
		}
	}	
	#endif

	//cout << "af18" << endl;
	
#ifdef GIA_USE_NLG2
	string nlgDependencyRelation1 = NLG2generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType1], entityIndex0string, entityIndex1string);										
	string nlgDependencyRelation2 = NLG2generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType2], entityIndex0string, entityIndex2string);										
	
	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode():" << endl;
	cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
	cout << "nlgDependencyRelation2 = " << nlgDependencyRelation2 << endl;
	#endif
	*generatedText = *generatedText + nlgDependencyRelation1;
	*generatedText = *generatedText + nlgDependencyRelation2;
#else
	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode():" << endl;
	cout << "entityTextExpandedArray[0] = " << entityTextExpandedArray[0] << endl;
	cout << "entityTextExpandedArray[1] = " << entityTextExpandedArray[1] << endl;
	cout << "entityTextExpandedArray[2] = " << entityTextExpandedArray[2] << endl;
	#endif
	
	entityTextExpandedArray[0] = determinateArray[0] + entityTextExpandedArray[0];
	if(supportAdditionalLinks)
	{
		entityTextExpandedArray[1] = determinateArray[1] + entityTextExpandedArray[1];
	}
	entityTextExpandedArray[2] = determinateArray[2] + entityTextExpandedArray[2];
	
	if(supportAdditionalLinks)
	{//is a condition link; add copular- eg the dog is near the park.
		if((connectionType1 == nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[0]) && (connectionType2 == nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[1]))
		{
			entityTextExpandedArray[0] = string(NLG_DEFINITION_TEXT) + NLG_TEXT_SPACE + entityTextExpandedArray[0]; 
		}
	}
	if(entityNodeAvailableArray[1])
	{	
		if(supportAdditionalLinks)
		{	
			*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[1];
		}
	}	
	*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[0];
	if(entityNodeAvailableArray[2])
	{	
		*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[2];
	}
	
#endif
	
	//if(supportAdditionalLinks)
	//{
		
		//cout << "generateThreeEntitySentenceFromEntityNode:" << endl;
		if(entityNodeAvailableArray[0])
		{
			//cout << "entityNode0->definiteSourceAddedInLanguageGeneration: " << entityNode0->entityName << endl;		
			entityNode0->definiteSourceAddedInLanguageGeneration = true;
		}		
		if(entityNodeAvailableArray[1])
		{
			//cout << "entityNode1->definiteSourceAddedInLanguageGeneration: " << entityNode1->entityName << endl;
			entityNode1->definiteSourceAddedInLanguageGeneration = true;
		}
		if(entityNodeAvailableArray[2])
		{
			//cout << "entityNode2->definiteSourceAddedInLanguageGeneration: " << entityNode2->entityName << endl;		
			entityNode2->definiteSourceAddedInLanguageGeneration = true;
		}		
	//}
	
	#ifdef GIA_NLG_DEBUG
	cout << "Exiting: generateThreeEntitySentenceFromEntityNode" << endl;
	#endif
}

void generateTwoEntitySentenceFromEntityConnection(GIAEntityNode * entityNode1, GIAEntityConnection * entityConnection, string * generatedText, int connectionType, int startEntityIndex, bool additionalLink)
{
	//if(!(entityConnection->parsedForLanguageGeneration))	//&& !(entityConnection->disabled)
	//{	
				
	GIAEntityNode * entityNode2 = entityConnection->entity;

	#ifdef GIA_NLG_DEBUG
	cout << "generateTwoEntitySentenceFromEntityConnection: " << entityNode1->entityName << endl;
	#endif

#ifdef GIA_USE_NLG2	
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
	
	bool generateTwoDependencyRelations = false;
	string nlgDependencyRelationType1 = "";	
	string nlgDependencyRelationType2 = "";	
	string nlgDependencyRelationSharedArgument = "";	
#else
	string entityTextExpandedArray[3];
	for(int i=0; i<3; i++)
	{
		entityTextExpandedArray[i] = "";
	}
	bool generateLinkingWord = false;
	string linkingWord = "";
	bool prepend = false;	
	entityTextExpandedArray[0] = "";	
	entityTextExpandedArray[1] = getWordOrig(entityNode1);
	entityTextExpandedArray[2] = getWordOrig(entityNode2);
#endif



	if(entityNode2->isAction)
	{
		cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode1->isAction)" << endl;
	}	
	else if(entityNode2->isCondition)
	{
		cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode1->isCondition)" << endl;
	}

	if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)
	{			
		if(entityNode2->isProperty) 
		{
			//cout << "helsinki" << endl;
			
			#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
			bool hasQuality = false;
			if(entityNode2->hasQuality)
			{
				hasQuality = true;
				if(entityNode1->isAction)
				{
					#ifdef GIA_USE_NLG2	
					nlgDependencyRelationType1 = RELATION_TYPE_ADJECTIVE_ADVMOD
					#else
					#endif
				}
				else
				{
					#ifdef GIA_USE_NLG2	
					nlgDependencyRelationType1 = RELATION_TYPE_ADJECTIVE_AMOD;		//or RELATION_TYPE_ADJECTIVE_PREDADJ?											
					#else
					prepend = true;						
					#endif
				}
			}
			if(!hasQuality)
			{
			#endif
				//eg possessive and/or tom has a bike	
				#ifdef GIA_USE_NLG2
				#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS				
				generateTwoDependencyRelations = true;
				nlgDependencyRelationType1 = RELATION_TYPE_SUBJECT;
				nlgDependencyRelationType2 = RELATION_TYPE_OBJECT;
				nlgDependencyRelationSharedArgument = RELATION_ENTITY_HAVE;
				#else
				nlgDependencyRelationType1 = RELATION_TYPE_POSSESSIVE;
				#endif
				#else
				generateLinkingWord = true;
				linkingWord = NLG_POSSESSIVE_TEXT;		
				#endif
			#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
			}
			#endif
		}	
		else
		{//isConcept
			cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode1->isConcept) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)" << endl;
			exit(0);
		}
	}
	else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS)
	{
		#ifdef GIA_USE_NLG2
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS
		generateTwoDependencyRelations = true;
		nlgDependencyRelationType1 = RELATION_TYPE_SUBJECT;
		nlgDependencyRelationType2 = RELATION_TYPE_OBJECT;
		nlgDependencyRelationSharedArgument = RELATION_ENTITY_BE;		
		#else
		nlgDependencyRelationType1 = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		#endif
		#else
		generateLinkingWord = true;
		linkingWord = NLG_DEFINITION_TEXT;
		#endif
	}

#ifdef GIA_USE_NLG2
	if(generateTwoDependencyRelations)
	{
		GIAEntityNode * entityNode0 = new GIAEntityNode();
		entityNode0->entityName = nlgDependencyRelationSharedArgument;
		entityNode0->isAction = true;	//NB "have"/"be" are interpreted as actions/verbs by NLP/stanford/relex (but reduced by GIA)
		NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);			
		NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);
		NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);		
		string nlgDependencyRelation1 = NLG2generateNLGInputViewLine(nlgDependencyRelationType1, entityIndex0string, entityIndex1string);										
		string nlgDependencyRelation2 = NLG2generateNLGInputViewLine(nlgDependencyRelationType2, entityIndex0string, entityIndex2string);											

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
		NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);
		NLG2generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);	
		string nlgDependencyRelation1 = NLG2generateNLGInputViewLine(nlgDependencyRelationType1, entityIndex1string, entityIndex2string);	

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;	
		cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
		#endif
		*generatedText = *generatedText + nlgDependencyRelation1;
	}
#else

	#ifdef GIA_NLG_DEBUG
	cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;
	cout << "entityTextExpandedArray[0] = " << entityTextExpandedArray[0] << endl;
	cout << "entityTextExpandedArray[1] = " << entityTextExpandedArray[1] << endl;
	cout << "entityTextExpandedArray[2] = " << entityTextExpandedArray[2] << endl;	
	#endif
		
	if(prepend)
	{	
		//assert !generateLinkingWord
		if(additionalLink)
		{
			*generatedText = entityTextExpandedArray[2] + NLG_TEXT_SPACE + *generatedText;	
		}
		else
		{
			addDeterminate(entityNode1, &(entityTextExpandedArray[1]));
			//*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[2] + NLG_TEXT_SPACE + entityTextExpandedArray[1];
			*generatedText = *generatedText + entityTextExpandedArray[1] + NLG_TEXT_SPACE + string(NLG_DEFINITION_TEXT) + NLG_TEXT_SPACE + entityTextExpandedArray[2];
		}
	}
	else
	{


		if(!additionalLink)
		{
			addDeterminate(entityNode1, &(entityTextExpandedArray[1]));
			*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[1];		
		}
		
		if(generateLinkingWord)
		{
			GIAEntityNode * entityNode0 = new GIAEntityNode();
			entityNode0->entityName = linkingWord;
			entityNode0->isAction = true;	//NB "have"/"be" are interpreted as actions/verbs by NLP/stanford/relex (but reduced by GIA)
			entityTextExpandedArray[0] = linkingWord;
			
			addDeterminate(entityNode0, &(entityTextExpandedArray[0]));	
			*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[0];
			
			delete entityNode0;
		}
			
		addDeterminate(entityNode2, &(entityTextExpandedArray[2]));				
		*generatedText = *generatedText + NLG_TEXT_SPACE + entityTextExpandedArray[2];
	}
	
	#ifdef GIA_NLG_DEBUG
	cout << "generateTwoEntitySentenceFromEntityConnection():" << endl;
	cout << "generatedText = " << *generatedText << endl;
	#endif	
#endif

	//if(!additionalLink)
	//{
		//cout << "generateTwoEntitySentenceFromEntityNode:" << endl;
		//cout << "entityNode1->definiteSourceAddedInLanguageGeneration: " << entityNode1->entityName << endl;
		//cout << "entityNode2->definiteSourceAddedInLanguageGeneration: " << entityNode2->entityName << endl;
		entityNode1->definiteSourceAddedInLanguageGeneration = true;
		entityNode2->definiteSourceAddedInLanguageGeneration = true;
	//}
	
	#ifdef GIA_NLG_DEBUG
	cout << "Exiting: generateTwoEntitySentenceFromEntityConnection" << endl;
	#endif		

	//}
}

#ifdef GIA_USE_NLG2
void NLG2generateNLGInputViewFeatureTagsGenericPerSentence(string * generatedNLGInputViewTags)
{
	string NLGInputViewFeatureTagPosDeterminateDefinite = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_DEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										
	string NLGInputViewFeatureTagPosDeterminateIndefinite = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_INDEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										
	string NLGInputViewFeatureTagPosPunctuationFullstop = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_FULLSTOP, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FULLSTOP);	

	#ifdef GIA_NLG_DEBUG
	cout << "NLG2generateNLGInputViewFeatureTagsGenericPerSentence():" << endl;	
	cout << "NLGInputViewFeatureTagPosDeterminateDefinite = " << NLGInputViewFeatureTagPosDeterminateDefinite << endl;
	cout << "NLGInputViewFeatureTagPosDeterminateIndefinite = " << NLGInputViewFeatureTagPosDeterminateIndefinite << endl;
	cout << "NLGInputViewFeatureTagPosPunctuationFullstop = " << NLGInputViewFeatureTagPosPunctuationFullstop << endl;
	#endif
		
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagPosDeterminateDefinite;
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagPosDeterminateIndefinite;
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagPosPunctuationFullstop;
}

void NLG2generateNLGInputViewFeatureTagsFromEntityNode(GIAEntityNode * entityNode, int entityIndex, string * generatedNLGInputViewTags)
{
	char entityIndexStringCharstar[10];
	sprintf(entityIndexStringCharstar, "%d", entityIndex);
	string entityIndexString = entityIndexStringCharstar;
	
	//lemma
	string NLGInputViewFeatureTagLemma = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_LEMMA, entityIndexString, getWordOrig(entityNode));
	
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
			string NLGInputViewFeatureTagTense = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_TENSE, entityIndexString, tenseString);							
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
		NLGInputViewFeatureTagNounNumber = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_NOUNNUMBER, entityIndexString, nounNumberString);
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
			if(!(entityNode->definiteSourceAddedInLanguageGeneration))
			{
				isDefinite = true;
			}
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
		}
		#endif
	}	
	else
	{//isConcept
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];	
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
	}
	string NLGInputViewFeatureTagInflection = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_INFLECTION, entityIndexString, inflectionString);	
	string NLGInputViewFeatureTagPos = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, entityIndexString, posString);
	
	//flag definite
	string NLGInputViewFeatureTagFlagDefinite = "";
	if(isDefinite)
	{
		NLGInputViewFeatureTagFlagDefinite = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_DEFINITE, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_DEFINITE);
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
		NLGInputViewFeatureTagGender = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_GENDER, entityIndexString, genderString);										
	}
		
	//flag person
	if(isPerson)
	{
		NLGInputViewFeatureTagFlagPerson = NLG2generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_PERSON, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_PERSON);	
	}
	#endif
	
	
	
	#ifdef GIA_NLG_DEBUG
	cout << "NLG2generateNLGInputViewFeatureTagsFromEntityNode():" << endl;
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
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagLemma;
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagFlagDefinite;
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagGender;		
	#endif
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagTense;
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagNounNumber;		
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagInflection;	
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagFlagPerson;		
	#endif
	*generatedNLGInputViewTags = *generatedNLGInputViewTags + NLGInputViewFeatureTagPos;
	
}	
		



string NLG2generateNLGInputViewLine(string type, string governor, string dependent)
{
	#ifdef GIA_NLG_DEBUG_MANUALLY_HARDCODE_INTO_NLG2
	string NLGInputViewLine = "\"" + type + "(" + governor + ", " + dependent + ")" + "\\n\" +" + "\n";	
	#else
	string NLGInputViewLine = type + "(" + governor + ", " + dependent + ")" + "\n";
	#endif
	return NLGInputViewLine;
}

#else


void addDeterminate(GIAEntityNode * entityNode, string * entityTextExpanded)
{
	string determinate = calcDeterminate(entityNode);
	*entityTextExpanded = determinate + *entityTextExpanded;
}

string calcDeterminate(GIAEntityNode * entityNode)
{
	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	//gender
	bool isPerson = false;
	bool hasGender = false;
	string genderString = "";
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
	#endif

	//determinate	
	bool addDeterminate = false;
	string determinate = "";	
	if(entityNode->isAction)
	{

	}	
	else if(entityNode->isCondition)
	{

	}
	else if(entityNode->isProperty) 
	{
		if(!(entityNode->hasQuality))
		{
			if(entityNode->definiteSourceAddedInLanguageGeneration)
			{	
				if(entityNode->entityName == "cow")
				{
					cout << "cow; definiteSourceAddedInLanguageGeneration" << endl;
				}		
				determinate = NLG_DEFINITE_TEXT;	//the
				addDeterminate = true;
			}
			else
			{	
				determinate = NLG_INDEFINITE_TEXT;	//a
				addDeterminate = true;
			}
		}
	}	
	else
	{//isConcept

	}
	
	string determinateFinal = "";
	if(addDeterminate)
	{
		#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
		if(!isPerson)
		{
		#endif		
			determinateFinal = determinate + NLG_TEXT_SPACE;
		#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
		}
		#endif
	}	
	return determinateFinal;
}


#endif

string getWordOrig(GIAEntityNode * entityNode)
{
	string wordOrig = "";
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	if(entityNode->wordOrig != "")
	{
		wordOrig = entityNode->wordOrig;		//IMPORTANT; due to bug in nlg2, nlg2 currently requires the original word, not the lemma
	}
	else
	{
		wordOrig = entityNode->entityName;
	}
	#else
	wordOrig = generateMorphology(entityNode);
	#endif
	return wordOrig;
}


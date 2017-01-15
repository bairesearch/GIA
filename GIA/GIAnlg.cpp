/*******************************************************************************
 *
 * File Name: GIAnlg.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n4b 21-July-2012
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
			generateThreeEntitySentenceFromEntityNode(entityNode, generatedText, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, 1, supportAdditionalLinks);
		}
		else if(entityNode->isCondition)
		{
			generateThreeEntitySentenceFromEntityNode(entityNode, generatedText, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, 1, supportAdditionalLinks);
		}	
				
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
			{
				if(nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[i])
				{
					generateTwoEntitySentenceFromEntityConnection(entityNode, *connectionIter, generatedText, i, 1);
					NLGSentence * newNLGsentence = new NLGSentence();
					currentNLGsentenceUpdated->next = newNLGsentence;	
					currentNLGsentenceUpdated = currentNLGsentenceUpdated->next;				
				}
								
				currentNLGsentenceUpdated = generateLanguageFromEntityNode((*connectionIter)->entity, currentNLGsentence);
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
				entityNodeHasAdditionalConnection[entityNodeIndex][i] = false;
				int vectorConnectionIndex = nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i];
				if(entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].size() == 1)
				{//entity contains single property or condition
					GIAEntityConnection * entityConnectionAdditional = entityNodeArray[entityNodeIndex]->entityVectorConnectionsArray[vectorConnectionIndex].back();
					GIAEntityNode * entityNodeAdditional = entityConnectionAdditional->entity;
					
					//entityNodeHasAdditionalConnection[entityNodeIndex][i] = true;
					//entityNodeHasAdditionalConnectionIndex[entityNodeIndex][i] = currentEntityIndexAdditional;					
					if(nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i])
					{
						generateThreeEntitySentenceFromEntityNode(entityNodeAdditional, string * generatedText, nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i], nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[i]+1, currentEntityIndexAdditional, false)
					}
					else
					{
						generateTwoEntitySentenceFromEntityConnection(entityNodeArray[entityNodeIndex], entityConnectionAdditional, generatedText, nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[i], currentEntityIndexAdditional)					
					}
					
					string nlgDependencyRelation1 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType1], entityIndex0string, entityIndex1string);										
					
					currentEntityIndexAdditional++;
				}
			}		
		}
	}	
	#endif

	string nlgDependencyRelation1 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType1], entityIndex0string, entityIndex1string);										
	string nlgDependencyRelation2 = generateNLGInputViewLine(nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[connectionType2], entityIndex0string, entityIndex2string);										

	//#ifdef GIA_NLG_DEBUG
	cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
	cout << "nlgDependencyRelation2 = " << nlgDependencyRelation2 << endl;
	//#endif
	*generatedText = *generatedText + nlgDependencyRelation1;
	*generatedText = *generatedText + nlgDependencyRelation2;
	
	
	bool endOfSentence = true;
	#ifdef NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS
	if(!supportAdditionalLinks)
	{
		endOfSentence = false;
	}
	#endif	
	
	if(endOfSentence)
	{
		*generatedText = *generatedText + "\n\n";	//how to declare end of sentence?
	}
		
	#ifdef GIA_NLG_DEBUG
	cout << "generateThreeEntitySentenceFromEntityNode: Exiting";
	#endif
}

void generateTwoEntitySentenceFromEntityConnection(GIAEntityNode * entityNode0, GIAEntityConnection * entityConnection, string * generatedText, int connectionType, int startEntityIndex)
{
	//if(!(entityConnection->parsedForLanguageGeneration))	//&& !(entityConnection->disabled)
	//{	
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
				
		GIAEntityNode * entityNode1 = entityConnection->entity;

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection: ";
		if(entityNode1->isProperty)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (is property)" << endl;
		}
		else if(entityNode1->isAction)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (is action)" << endl;
		}
		else if(entityNode1->isCondition)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedInstance)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsAction)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedTime)
		{
			cout << "entityNode1 = " << entityNode1->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode1 = " << entityNode1->entityName << endl;
		}
		#endif

		
		bool generateTwoDependencyRelations = false;
		string nlgDependencyRelationType0 = "";	
		string nlgDependencyRelationType1 = "";	
		string nlgDependencyRelationSharedArgument = "";
		
		if(entityNode0->isAction)
		{
			cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode0->isAction)" << endl;
		}	
		else if(entityNode0->isCondition)
		{
			cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode0->isCondition)" << endl;
		}
				
		if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)
		{			
			if(entityNode0->isProperty) 
			{
				#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
				bool hasQuality = false;
				if(entityNode0->hasQuality)
				{
					hasQuality = true;
					bool propertyOwnerIsAction = false;
					if(!(entityNode0->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->empty()))
					{
						GIAEntityNode * propertyOwner = entityNode0->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->back()->entity;
						if(propertyOwner->isAction)
						{
							propertyOwnerIsAction = true;
							nlgDependencyRelationType0 = RELATION_TYPE_ADJECTIVE_ADVMOD;
						}
					}
					if(!propertyOwnerIsAction)
					{
						nlgDependencyRelationType0 = RELATION_TYPE_ADJECTIVE_AMOD;		//or RELATION_TYPE_ADJECTIVE_PREDADJ?	
					}
				}
				if(!hasQuality)
				{
				#endif
					//eg possessive and/or tom has a bike	
					#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS				
					generateTwoDependencyRelations = true;
					nlgDependencyRelationType0 = RELATION_TYPE_SUBJECT;
					nlgDependencyRelationType1 = RELATION_TYPE_OBJECT;
					nlgDependencyRelationSharedArgument = RELATION_ENTITY_HAVE;
					#else
					nlgDependencyRelationType0 = RELATION_TYPE_POSSESSIVE;
					#endif
				#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
				}
				#endif
			}	
			else
			{//isConcept
				cout << "error: generateTwoEntitySentenceFromEntityConnection && (entityNode0->isConcept) && (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES)" << endl;
				nlgDependencyRelationType0 = RELATION_TYPE_SUBJECT;
			}
		}
		else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS)
		{
			#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS
			generateTwoDependencyRelations = true;
			nlgDependencyRelationType0 = RELATION_TYPE_SUBJECT;
			nlgDependencyRelationType1 = RELATION_TYPE_OBJECT;
			nlgDependencyRelationSharedArgument = RELATION_ENTITY_BE;		
			#else
			nlgDependencyRelationType0 = RELATION_TYPE_APPOSITIVE_OF_NOUN;
			#endif
		}
		
		if(generateTwoDependencyRelations)
		{
			GIAEntityNode * entityNode2 = new GIAEntityNode();
			entityNode2->name = nlgDependencyRelationSharedArgument;
			entityNode2->isAction = true;	//NB "have"/"be" are interpreted as actions/verbs by NLP/stanford/relex (but reduced by GIA)
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);		
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode2, entityIndex2, generatedText);
			string nlgDependencyRelation0 = generateNLGInputViewLine(nlgDependencyRelationType0, entityIndex2string, entityIndex0string);										
			string nlgDependencyRelation1 = generateNLGInputViewLine(nlgDependencyRelationType1, entityIndex2string, entityIndex1string);											
		
			//#ifdef GIA_NLG_DEBUG
			cout << "nlgDependencyRelation0 = " << nlgDependencyRelation0 << endl;
			cout << "nlgDependencyRelation1 = " << nlgDependencyRelation1 << endl;
			//#endif
			*generatedText = *generatedText + nlgDependencyRelation0;
			*generatedText = *generatedText + nlgDependencyRelation1;	
			
			delete entityNode2;	
		}
		else
		{
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode0, entityIndex0, generatedText);
			generateNLGInputViewFeatureTagsFromEntityNode(entityNode1, entityIndex1, generatedText);	
			string nlgDependencyRelation0 = generateNLGInputViewLine(nlgDependencyRelationType0, entityIndex0string, entityIndex1string);	
		
			//#ifdef GIA_NLG_DEBUG
			cout << "nlgDependencyRelation0 = " << nlgDependencyRelation0 << endl;
			//#endif
			*generatedText = *generatedText + nlgDependencyRelation0;
			
		}

		#ifdef GIA_NLG_DEBUG
		cout << "generateTwoEntitySentenceFromEntityConnection: ";
		if(entityNode1->isProperty)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (is property)" << endl;
		}
		else if(entityNode1->isAction)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (is action)" << endl;
		}
		else if(entityNode1->isCondition)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated property)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode1->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated property is condition)" << endl;
		}
		else if(entityNode1->hasAssociatedTime)
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "Exiting: entityNode1 = " << entityNode1->entityName << endl;
		}
		#endif		
		
	//}
}

void generateRelexFeatureTagsGenericPerSentence(string * generatedRelexTags)
{
	string relexFeatureTagPosDeterminateDefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_DEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										
	string relexFeatureTagPosDeterminateIndefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_INDEFINITE, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE);										

	//#ifdef GIA_NLG_DEBUG
	cout << "relexFeatureTagPosDeterminateDefinite = " << relexFeatureTagPosDeterminateDefinite << endl;
	cout << "relexFeatureTagPosDeterminateIndefinite = " << relexFeatureTagPosDeterminateIndefinite << endl;
	//#endif
		
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagPosDeterminateDefinite;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagPosDeterminateIndefinite;
}

void generateNLGInputViewFeatureTagsFromEntityNode(GIAEntityNode * entityNode, int entityIndex, string * generatedRelexTags)
{
	char entityIndexStringCharstar[10];
	sprintf(entityIndexStringCharstar, "%d", entityIndex);
	string entityIndexString = entityIndexStringCharstar;
	
	//lemma
	string relexFeatureTagLemma = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_LEMMA, entityIndexString, entityNode->name);									
		
	//tense
	//string wordName = entityNode->entityName;
	string relexFeatureTagTense = "";
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
			string relexFeatureTagTense = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_TENSE, entityIndexString, tenseString);							
		}
		else
		{
			//wordName = wordName + "(" + timeCondition->conditionName + ")";
		}	
	}

	//noun_number
	string nounNumberString = "";
	if((entityNode->grammaticalNumber != grammaticalNumberNameArray[MEASURE_TYPE_UNKNOWN]) && (entityNode->grammaticalNumber != grammaticalNumberNameArray[MEASURE_TYPE_UNDEFINED]))
	{
		nounNumberString = grammaticalNumberNameArray[entityNode->grammaticalNumber];
	}
	string relexFeatureTagNounNumber = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_NOUNNUMBER, entityIndexString, nounNumberString);										


	//pos + inflection
	string posString = "";
	string inflectionString = "";
	bool isDefinite = false;	
	if(entityNode->isAction)
	{
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_VERB];
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_VERB];
	}	
	else if(entityNode->isCondition)
	{
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_PREP];	
		//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_PREP];	//no inflection for prepositions
	}
	else if(entityNode->isProperty) 
	{
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
		bool hasQuality = false;
		if(entityNode->hasQuality)
		{
			hasQuality = true;
			bool propertyOwnerIsAction = false;
			if(!(entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->empty()))
			{
				GIAEntityNode * propertyOwner = entityNode->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]->back()->entity;
				if(propertyOwner->isAction)
				{
					propertyOwnerIsAction = true;
					posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADV];
					//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADV];	//no inflection for adjectives?
				}
			}
			if(!propertyOwnerIsAction)
			{
				posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_ADJ];
				//inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_ADJ];	//no inflection for adverbs?
			}
		}
		if(!hasQuality)
		{
		#endif
			posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
			inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];
			isDefinite = true;
		#ifdef NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES	
		}
		#endif
	}	
	else
	{//isConcept
		posString = grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NOUN];
		inflectionString = grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NOUN];
	}
	string relexFeatureTagPos = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_POS, entityIndexString, posString);
	string relexFeatureTagInflection = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_INFLECTION, entityIndexString, inflectionString);
	
	//flag definite
	string relexFeatureTagFlagDefinite = "";
	if(isDefinite)
	{
		relexFeatureTagFlagDefinite = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_DEFINITE, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_DEFINITE);
	}
	
	/*
	//gender
	bool isPerson = false;
	string genderString = "";	
	if(entityNode->grammaticalGenderTemp != grammaticalGenderNameArray[GRAMMATICAL_GENDER_UNDEFINED])
	{
		if(entityNode->grammaticalGenderTemp == GRAMMATICAL_GENDER_PERSON)
		{
			isPerson = true;
		}
		else
		{
			genderString = grammaticalGenderNameArray[entityNode->grammaticalGenderTemp];
		}
	}
	string relexFeatureTagGender = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_GENDER, entityIndexString, genderString);										
	
	//flag person
	if(isPerson)
	{
		relexFeatureTagFlagPerson = generateNLGInputViewLine(NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_PERSON, entityIndexString, NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_PERSON);	
	}
	*/
	
	
	//#ifdef GIA_NLG_DEBUG
	cout << "relexFeatureTagLemma = " << relexFeatureTagLemma << endl;
	cout << "relexFeatureTagTense = " << relexFeatureTagTense << endl;
	cout << "relexFeatureTagNounNumber = " << relexFeatureTagNounNumber << endl;
	cout << "relexFeatureTagPos = " << relexFeatureTagPos << endl;	
	cout << "relexFeatureTagInflection = " << relexFeatureTagInflection << endl;			
	cout << "relexFeatureTagFlagDefinite = " << relexFeatureTagFlagDefinite << endl;	
	//cout << "relexFeatureTagGender = " << relexFeatureTagGender << endl;		
	//cout << "relexFeatureTagFlagPerson = " << relexFeatureTagFlagPerson << endl;																			
	//#endif
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagLemma;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagTense;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagNounNumber;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagPos;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagInflection;
	*generatedRelexTags = *generatedRelexTags + relexFeatureTagFlagDefinite;
	//*generatedRelexTags = *generatedRelexTags + relexFeatureTagGender;
	//*generatedRelexTags = *generatedRelexTags + relexFeatureTagFlagPerson;	
	
}	
		



string generateNLGInputViewLine(string type, string governor, string dependent)
{
	string NLGInputViewLine = type + "(" + governor + ", " + dependent + ")" + "\n";
	return NLGInputViewLine;
}

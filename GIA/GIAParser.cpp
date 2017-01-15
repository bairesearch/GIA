/*******************************************************************************
 *
 * File Name: GIAParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i10a 12-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Parses tabular subsections (Eg <relations>) of CFF File
 *
 *******************************************************************************/



#include "GIAParser.h"
#include "GIATranslatorOperations.h"	//required for convertStanfordRelationToRelex

#define MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA 150 //max characters of some word in input data. includes '\0' at end of a string

#define CHAR_TAB '\t'
#define CHAR_COMMA ','
#define CHAR_NEW_LINE '\n'
#define CHAR_OPEN_BRACKET '('
#define CHAR_CLOSE_BRACKET ')'
#define CHAR_OPEN_SQUARE_BRACKET '['
#define CHAR_CLOSE_SQUARE_BRACKET ']'
#define CHAR_DASH '-'

string convertStanfordRelationToRelex(string * stanfordRelation)
{
	//prepend '_'
	string relationTypeRelexStandard = "";
	relationTypeRelexStandard = relationTypeRelexStandard + RELEX_DEPENDENCY_RELATION_PREPENDITION + *stanfordRelation;
	
	//now deal with anamolies between dependency relation definitions;
	for(int i=0; i<GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES; i++)
	{
		//cout << "relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD][i] = " << relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD][i] << endl;
		//cout << "relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_RELEX][i] = " << relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_RELEX][i] << endl;
		
		if(*stanfordRelation == relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD][i])
		{
			relationTypeRelexStandard = relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_RELEX][i];
		}
	}
	
	bool stanfordPrepositionFound = false;
	string tempString = convertStanfordPrepositionToRelex(stanfordRelation, GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);
	if(stanfordPrepositionFound)
	{
		relationTypeRelexStandard = *stanfordRelation;	//do not modify stanford preposition relations "prep_...." to "_prep_..."
	}

	return relationTypeRelexStandard;
}


void GIATHparseRelexRelationsText(string * relationsText, Relation * firstRelationInList, int * maxNumberOfWordsInSentence, bool NLPrelexCompatibilityMode)
{
	*maxNumberOfWordsInSentence = 0;
	
	int numberOfCharactersInRelationsText = relationsText->length();
	
	char currentItemString[MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA] = "";
	currentItemString[0] = '\0';	

	/* Data file layout example

		_subj(slip[4], hand[3])
		_poss(hand[3], John[1])
	*/

	Relation * currentRelation = firstRelationInList;
	
	int relationIndex = 0;
	int characterIndex = 0;
	
	characterIndex++;	//skip first new line in .cff file
	
	int currentRelationPart = 0; 
	//cout << "h1" << endl;

	while (characterIndex < numberOfCharactersInRelationsText)
	{
		char c = (*relationsText)[characterIndex];
		//cout << "c = " << c << endl;
		
		switch(c)
		{
			case CHAR_NEW_LINE:
			{
				/*
				cout << "relation added;" << endl;
				cout << "currentRelation->relationType = " << currentRelation->relationType << endl;
				cout << "currentRelation->relationGovernor = " << currentRelation->relationGovernor << endl;
				cout << "currentRelation->relationDependent = " << currentRelation->relationDependent << endl;
				cout << "currentRelation->relationGovernorIndex = " << currentRelation->relationGovernorIndex << endl;
				cout << "currentRelation->relationDependentIndex = " << currentRelation->relationDependentIndex << endl;
				*/
					
				Relation * newRelation = new Relation();
				currentRelation->next = newRelation;
				currentRelation = currentRelation->next;
				
				currentRelationPart = 0;
				currentItemString[0] = '\0';

				relationIndex++;
				
				break;
			}
			case CHAR_OPEN_BRACKET:
			{
				string relationType = currentItemString;
				if(NLPrelexCompatibilityMode)
				{
					relationType = convertStanfordRelationToRelex(&relationType);
				}
				currentRelation->relationType = relationType;
				currentItemString[0] = '\0';
				currentRelationPart++;
				
				break;
			}			
			case CHAR_CLOSE_BRACKET:
			{
				break;
			}
			case CHAR_OPEN_SQUARE_BRACKET:
			{
				if(currentRelationPart == 1)
				{	
					currentRelation->relationGovernor = currentItemString;
				}
				else if(currentRelationPart == 2)
				{
					currentRelation->relationDependent = currentItemString;
				}
				currentItemString[0] = '\0';
							
				break;
			}
			case CHAR_CLOSE_SQUARE_BRACKET:
			{
				if(currentRelationPart == 1)
				{	
					currentRelation->relationGovernorIndex = int(atof(currentItemString));
				}
				else if(currentRelationPart == 2)
				{
					currentRelation->relationDependentIndex = int(atof(currentItemString));
				}
				
				if(currentRelation->relationDependentIndex > *maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationDependentIndex;
				}
						
				currentItemString[0] = '\0';
				currentRelationPart++; 
				
				break;
			}									
			case CHAR_COMMA:
			{								
				characterIndex++;	//skip space in .cff file directly after comma
				
				break;
			}
			default:
			{
				char characterString[2];
				characterString[0] = c;
				characterString[1] = '\0';
				strcat(currentItemString, characterString);
				break;
			}
		}
		
		characterIndex++;
	}

}



void GIATHparseStanfordParserRelationsText(string * relationsText, Sentence * currentSentenceInList, int * maxNumberOfWordsInSentence, bool featuresNotPreviouslyFilled)
{
	Relation * firstRelationInList = currentSentenceInList->firstRelationInList;
	Feature * firstFeatureInList = currentSentenceInList->firstFeatureInList;

	string relationType;
	string relationGovernor;
	string relationDependent;
	int relationGovernorIndex;
	int relationDependentIndex;

	*maxNumberOfWordsInSentence = 0;
	
	int numberOfCharactersInRelationsText = relationsText->length();
	
	char currentItemString[MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA] = "";
	currentItemString[0] = '\0';	

	/* Data file layout example

		det(fish-2, The-1)
		nsubj(swam-7, fish-2)
	*/

	Relation * currentRelation = firstRelationInList;
	
	int relationIndex = 0;
	int characterIndex = 0;
		
	int currentRelationPart = 0; 
	//cout << "h1" << endl;

	while(characterIndex < numberOfCharactersInRelationsText)
	{
		char c = (*relationsText)[characterIndex];
		//cout << "c = " << c << endl;
		
		if(c == CHAR_NEW_LINE)
		{
			currentRelation->relationType = relationType;
			currentRelation->relationGovernorIndex = relationGovernorIndex;
			currentRelation->relationDependentIndex = relationDependentIndex;		
			
			if(!featuresNotPreviouslyFilled)
			{
				/*
				//don't use these, use lemmas instead (as per Stanford Core NLP/Relex dependency relation definitions)
				currentRelation->relationGovernor = relationGovernor;
				currentRelation->relationDependent = relationDependent;						
				*/				
				Feature * currentFeatureInList = firstFeatureInList;
				for(int f=0; currentFeatureInList->entityIndex != currentRelation->relationDependentIndex; f++)
				{
					currentFeatureInList = currentFeatureInList->next;
				}
				currentRelation->relationDependent = currentFeatureInList->lemma;
				currentFeatureInList = firstFeatureInList;
				for(int f=0; currentFeatureInList->entityIndex != currentRelation->relationGovernorIndex; f++)
				{
					currentFeatureInList = currentFeatureInList->next;
				} 				
				currentRelation->relationGovernor = currentFeatureInList->lemma;						
			}
			else
			{
				currentRelation->relationGovernor = relationGovernor;
				currentRelation->relationDependent = relationDependent;			
			}
			
			/*
			#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
			cout << "relation added;" << endl;
			cout << "currentRelation->relationType = " << currentRelation->relationType << endl;
			cout << "currentRelation->relationGovernor = " << currentRelation->relationGovernor << endl;
			cout << "currentRelation->relationDependent = " << currentRelation->relationDependent << endl;
			cout << "currentRelation->relationGovernorIndex = " << currentRelation->relationGovernorIndex << endl;
			cout << "currentRelation->relationDependentIndex = " << currentRelation->relationDependentIndex << endl;
			#endif
			*/
			
			#ifdef GIA_NLP_PARSER_STANFORD_PARSER_DISABLE_ROOT_RELATION
			if(currentRelation->relationType != RELATION_TYPE_ROOT)
			{
				Relation * newRelation = new Relation();
				currentRelation->next = newRelation;
				currentRelation = currentRelation->next;			
			}
			#else
			Relation * newRelation = new Relation();
			currentRelation->next = newRelation;
			currentRelation = currentRelation->next;
			#endif
			
			currentRelationPart = 0;
			currentItemString[0] = '\0';

			relationIndex++;
		}
		else if(c == CHAR_OPEN_BRACKET)
		{
			relationType = currentItemString;
			relationType = convertStanfordRelationToRelex(&relationType);

			currentItemString[0] = '\0';
			currentRelationPart++;
		}	
		else if(c == CHAR_DASH)
		{
			if(currentRelationPart == 1)
			{	
				relationGovernor = currentItemString;
			}
			else if(currentRelationPart == 2)
			{
				relationDependent = currentItemString;
			}
			currentItemString[0] = '\0';
		}
		else if((c == CHAR_COMMA) || (c == CHAR_CLOSE_BRACKET))
		{
			if(currentRelationPart == 1)
			{	
				relationGovernorIndex = int(atof(currentItemString));
			}
			else if(currentRelationPart == 2)
			{
				relationDependentIndex = int(atof(currentItemString));
			}

			if(currentRelation->relationDependentIndex > *maxNumberOfWordsInSentence)
			{
				*maxNumberOfWordsInSentence = currentRelation->relationDependentIndex;
			}

			currentItemString[0] = '\0';
			currentRelationPart++; 
			
			if(c == CHAR_COMMA)
			{
				characterIndex++;	//skip space after ,
			}
		}
		else
		{
			char characterString[2];
			characterString[0] = c;
			characterString[1] = '\0';
			strcat(currentItemString, characterString);
		}
		
		characterIndex++;
	}
}




void GIATHparseFeaturesText(string * featuresText, Feature * firstFeatureInList, bool * isQuestion)
{	
	int numberOfCharactersInRelationsText = featuresText->length();
	
	char currentItemString[MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA] = "";
	currentItemString[0] = '\0';	

	/* Data file layout example

		1	joe	joe	noun	uncountable|.n
		2	went	go	verb	past|.v-d
		3	sailing	sail	verb	progressive|.v
		4	on	on	prep	
		5	dateID1	12 December 2010.	noun	date
		6	.	.	punctuation

	*/

	Feature * currentFeature = firstFeatureInList;
	
	int featureIndex = 1;
	int characterIndex = 0;
	
	characterIndex++;	//skip first new line in .cff file
	
	int currentFeaturePart = 0; 
	//cout << "h1" << endl;

	while (characterIndex < numberOfCharactersInRelationsText)
	{
		char c = (*featuresText)[characterIndex];
		//cout << c;
		
		switch(c)
		{
			case CHAR_NEW_LINE:
			{
				switch(currentFeaturePart)
				{
					case 4:
					{
						currentFeature->grammar = currentItemString;
						break;	
					}					
				}
				
			#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
				//identify is sentence is a question
				//cout << "currentFeature->type = " << currentFeature->type << endl;
				if(currentFeature->type == FEATURE_TYPE_PUNCTUATION)
				{
					if(currentFeature->word == FEATURE_WORD_QUESTIONMARK)
					{
						//cout << "isQuestion == true" << endl;
						*isQuestion = true;
					}				
				}
			#endif

				/*
				cout << "feature added;" << endl;
				cout << "currentFeature->entityIndex = " << currentFeature->entityIndex << endl;
				cout << "currentFeature->word = " << currentFeature->word << endl;
				cout << "currentFeature->lemma = " << currentFeature->lemma << endl;
				cout << "currentFeature->type = " << currentFeature->type << endl;
				cout << "currentFeature->grammar = " << currentFeature->grammar << endl;
				*/
											
				Feature * newFeature = new Feature();
				newFeature->previous = currentFeature;
				currentFeature->next = newFeature;
				currentFeature = currentFeature->next;
				
				currentFeaturePart = 0;
				currentItemString[0] = '\0';

				featureIndex++;
				
				break;
			}
			case CHAR_TAB:
			{	
				switch(currentFeaturePart)
				{
					case 0:
					{
						currentFeature->entityIndex = int(atof(currentItemString));
						if(currentFeature->entityIndex != featureIndex)
						{
							cout << "features parse error: (currentFeature->entityIndex != featureIndex)" << endl;
						}
						break;	
					}
					case 1:
					{
						currentFeature->word = currentItemString;
						break;	
					}
					case 2:
					{
						currentFeature->lemma = currentItemString;
						break;	
					}
					case 3:
					{
						currentFeature->type = currentItemString;
						break;	
					}
					case 4:
					{
						currentFeature->grammar = currentItemString;
						break;	
					}					
				}
				currentItemString[0] = '\0';
				currentFeaturePart++;
				
				break;
			}			
			default:
			{
				char characterString[2];
				characterString[0] = c;
				characterString[1] = '\0';
				strcat(currentItemString, characterString);
				break;
			}
		}
		
		characterIndex++;
	}

}




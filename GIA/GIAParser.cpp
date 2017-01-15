/*******************************************************************************
 *
 * File Name: GIAParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Parses tabular subsections (Eg <relations>) of CFF File
 *
 *******************************************************************************/



#include "GIAParser.h"


#define MAX_CHARACTERS_GIATH 150 //max characters of some word in input data. includes '\0' at end of a string

#define CHAR_TAB '\t'
#define CHAR_COMMA ','
#define CHAR_NEW_LINE '\n'
#define CHAR_OPEN_BRACKET '('
#define CHAR_CLOSE_BRACKET ')'
#define CHAR_OPEN_SQUARE_BRACKET '['
#define CHAR_CLOSE_SQUARE_BRACKET ']'




void GIATHparseRelationsText(string * relationsText, Relation * firstRelationInList, int * maxNumberOfWordsInSentence)
{
	*maxNumberOfWordsInSentence = 0;
	
	int numberOfCharactersInRelationsText = relationsText->length();
	
	char currentItemString[MAX_CHARACTERS_GIATH] = "";
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
				cout << "currentRelation->relationFunction = " << currentRelation->relationFunction << endl;
				cout << "currentRelation->relationArgument = " << currentRelation->relationArgument << endl;
				cout << "currentRelation->relationFunctionIndex = " << currentRelation->relationFunctionIndex << endl;
				cout << "currentRelation->relationArgumentIndex = " << currentRelation->relationArgumentIndex << endl;
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
				currentRelation->relationType = currentItemString;
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
					currentRelation->relationFunction = currentItemString;
				}
				else if(currentRelationPart == 2)
				{
					currentRelation->relationArgument = currentItemString;
				}
				currentItemString[0] = '\0';
							
				break;
			}
			case CHAR_CLOSE_SQUARE_BRACKET:
			{
				if(currentRelationPart == 1)
				{	
					currentRelation->relationFunctionIndex = int(atof(currentItemString));
				}
				else if(currentRelationPart == 2)
				{
					currentRelation->relationArgumentIndex = int(atof(currentItemString));
				}
				
				if(currentRelation->relationArgumentIndex > *maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationArgumentIndex;
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



void GIATHparseFeaturesText(string * featuresText, Feature * firstFeatureInList)
{	
	int numberOfCharactersInRelationsText = featuresText->length();
	
	char currentItemString[MAX_CHARACTERS_GIATH] = "";
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




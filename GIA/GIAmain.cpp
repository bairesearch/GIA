/*******************************************************************************
 *
 * File Name: GIAmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b8b 22-Sept-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Yet to Do: all Nodes should be indexed in an indexed database to allow for fast referencing
 *
 *******************************************************************************/
 


#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


#include "GIAmain.h"
#include "GIAParser.h"
#include "GIATranslator.h"
#include "GIAEntityNodeClass.h"
#include "GIAActionNodeClass.h"
#include "GIAdraw.h"
#include "GIAXMLconversion.h"
#include "XMLParserClass.h"
#include "XMLrulesClass.h"


#define CFF_XML_TAG_nlparse ((string)"nlparse")
#define CFF_XML_TAG_sentence ((string)"sentence")
#define CFF_XML_TAG_parse ((string)"parse")
#define CFF_XML_TAG_relations ((string)"relations")
#define CFF_XML_TAG_features ((string)"features")

int main(int argc,char **argv)
{
	bool result = true;

	Sentence * firstSentenceInList = new Sentence();
	Relation * newRelation = new Relation();
	Feature * newFeature = new Feature();
	firstSentenceInList->firstRelationInList = newRelation;
	firstSentenceInList->firstFeatureInList = newFeature;
	Sentence * currentSentence = firstSentenceInList;
	
	string relexOutputXMLFileName = "test-corpus-parsed.xml";
	
	XMLParserTag * firstTagInXMLFile = new XMLParserTag();
	readXMLFile(relexOutputXMLFileName, firstTagInXMLFile);

	cout << "hasd" << endl;
	
	XMLParserTag * currentTag = firstTagInXMLFile;
	currentTag = parseTagDownALevel(currentTag, CFF_XML_TAG_nlparse, &result);
	if(result)
	{
		//now for every sentence;
		while(currentTag->nextTag != NULL)
		{
			if(currentTag->name == CFF_XML_TAG_sentence)
			{
				XMLParserTag * firstTagInSentence;
				firstTagInSentence = parseTagDownALevel(currentTag, CFF_XML_TAG_sentence, &result);	
				XMLParserTag * firstTagInFirstParse;
				firstTagInFirstParse = parseTagDownALevel(firstTagInSentence, CFF_XML_TAG_parse, &result);
				
				if(result)
				{
					XMLParserTag * currentTagInParse = firstTagInFirstParse;
					while(currentTagInParse->nextTag != NULL)
					{
						
						if(currentTagInParse->name == CFF_XML_TAG_relations)
						{
							//cout << "currentTagInParse->value = " << currentTagInParse->value << endl;
							int maxNumberOfWordsInSentence = 0;
							GIATHparseRelationsText(&(currentTagInParse->value), currentSentence->firstRelationInList, &maxNumberOfWordsInSentence);
							currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
						}
						else if(currentTagInParse->name == CFF_XML_TAG_features)
						{
							//cout << "currentTagInParse->value = " << currentTagInParse->value << endl;
							GIATHparseFeaturesText(&(currentTagInParse->value), currentSentence->firstFeatureInList);
						}
																		
						currentTagInParse = currentTagInParse->nextTag;
					}
					
				}
				Sentence * newSentence = new Sentence();
				Relation * newRelation = new Relation();
				Feature * newFeaturesultre = new Feature();
				newSentence->previous = currentSentence;				
				newSentence->firstRelationInList = newRelation;
				newSentence->firstFeatureInList = newFeature;					
				currentSentence->next = newSentence;
				currentSentence = currentSentence->next;
								
			}
			currentTag = currentTag->nextTag;
		}

	}
	
	vector<GIAEntityNode*> indexOfEntityNodes;
	vector<string> indexOfEntityNames;
	vector<GIATimeConditionNode*> indexOfTimeNodes;
	vector<long> indexOfTimeNumbers;

	
	convertSentenceRelationsIntoGIAnetworkNodes(&indexOfEntityNodes, &indexOfEntityNames, &indexOfTimeNodes, &indexOfTimeNumbers, firstSentenceInList);
	//exit(0);
	
	int rasterImageWidth = 640;
	int rasterImageHeight = 480; 
	string outputFileName = "visualisation";
	bool display = true;
	
	
	printGIAnetworkNodes(&indexOfEntityNodes, rasterImageWidth, rasterImageHeight, outputFileName, display);

	
	string semanticNetXMLFileName = GIA_SEMANTIC_NET_XML_FILE_NAME;
	if(!writeSemanticNetXMLFile(semanticNetXMLFileName, getTranslatorEntityNodesCompleteList(), getTranslatorActionNodesCompleteList(), getTranslatorConditionNodesCompleteList()))
	{
		result = false;
	}
	
	/*
	if(!testReadSemanticNetXMLFile2(getTranslatorEntityNodesCompleteList(), getTranslatorActionNodesCompleteList(), getTranslatorConditionNodesCompleteList()))
	{
		result = false;
	}
	*/
	
}

/*******************************************************************************
 *
 * File Name: GIAParser.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1a 2-Apr-2012
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Parses tabular subsections (Eg <relations>) of CFF File
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PARSER
#define HEADER_GIA_PARSER

#include "GIASentenceClass.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
using namespace std;

//#define DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS


void GIATHparseRelationsText(string * relationsText, Relation * firstRelationInList, int * maxNumberOfWordsInSentence);
void GIATHparseFeaturesText(string * featuresText, Feature * firstFeatureInList, bool * isQuestion);


#endif

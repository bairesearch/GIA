/*******************************************************************************
 *
 * File Name: GIAwordnet.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1a 20-Apr-2012
 * Requirements: requires wordnet libraries to be installed
 * Description: searches wordnet database and parses wordnet output
 *
 *******************************************************************************/


#ifndef HEADER_GIA_WORDNET
#define HEADER_GIA_WORDNET

#include "GIAglobalDefs.h"
#include "wn.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
using namespace std;

#define MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA 150 //max characters of some word in input data. includes '\0' at end of a string
#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT (10000)
#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_LINE (1000)
#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_NUMBER_OF_SENSES (10)	//supports e^10 senses
#define WORDNET_FINDTHEINFO_OUTPUT_MAX_NUMBER_SYNONYMS (100)

void initialiseWordNet();
bool recordUntilCharacterOrEscapeCharacter(int charIndex, char * output, int * newCharIndex, char * lineString, char characterToRecordUntil, char escapeCharacter);

SynsetPtr findMostPopularSynset(string word, bool * wordIsFound, int wordType);
SynsetPtr findSynsets(string word, bool * wordIsFound, int wordType);
SynsetPtr findMostPopularSynset(SynsetPtr firstSenseInList, int wordType);
	void findSynonymsOLD(string word, bool * wordIsFound, string listOfSynonyms[], int wordType);

#endif

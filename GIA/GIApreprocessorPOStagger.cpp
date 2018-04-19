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
 * File Name: GIApreprocessorPOStagger.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e10a 15-January-2018
 * Requirements: requires plain text file
 * Description: preprocessor POS tagger
 *
 *******************************************************************************/


#include "GIApreprocessorPOStagger.hpp"
#include "GIAtranslatorDefs.hpp"


#ifdef GIA_PREPROCESSOR_POS_TAGGER

/*plan: 
execute createPreprocessSentencesSimple on wiki text

DONE read GIA lrp word lists to write to index list (GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES)

[NOTUSED] DONE Convert wiki words to lower case and add to index (GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_WIKI_DUMP_TEXT)
	FUTURE: Cross correlate index with that derived from existing GIA lrp word lists

DONE then execute GIA lrp multiword detection on wikitext, 
FUTURE: then save to file, or keep word lists in memory
Planned method:
	Identify the pos/possible pos tags of each word 5 to the left and right of the centre word
		7(word types)^3(possible/ambiguous/alternate word type cases) + (5*2)(number of context words)
	Could even train these experiences (input output cases) using a neural network 
	With 100billiom inputs and 7 outputs
OLD planned method: then for every triplet, find each word in index (ie get ID); A B C
	then add a new instance in the GIA word vec POS tagger database, indexed by a combination of A and C
	Only do this if A and C are pos unambiguous?
*/


#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MAP
multimap<string, pair<unsigned char, int>> POStaggerMap;		//each key is 10 bytes long: word context POS (ambiguity info) permutation, and the value is 1 byte long: POS (ambiguity info) for central word
	//as it currently stands POStaggerMap will be roughly the same size as the original wiki dump text (ie 12GB; too large)
bool GIApreprocessorPOStaggerClass::findInstancePOStaggerMap(const string POSambiguityInfoPermutation, unsigned char centreWordPOSambiguityInfo, int* numberOfInstances, const bool incrementIfFound)
{
	bool result = false;
	
	pair<multimap<string, pair<unsigned char, int>>::iterator, multimap<string, pair<unsigned char, int>>::iterator> rangeFound = POStaggerMap.equal_range(POSambiguityInfoPermutation);
	for(multimap<string, pair<unsigned char, int>>::iterator it = rangeFound.first; it != rangeFound.second; it++)
	{
		unsigned char centreWordPOSambiguityInfoCurrent = (it->second).first;
		if(centreWordPOSambiguityInfoCurrent == centreWordPOSambiguityInfo)
		{
			result = true;
			*numberOfInstances = (it->second).second;
			if(incrementIfFound)
			{
				(it->second).second = (it->second).second + 1;
			}
		}
	}
	
	return result;
}	
void GIApreprocessorPOStaggerClass::insertInstanceInPOStaggerMap(const string POSambiguityInfoPermutation, const unsigned char centreWordPOSambiguityInfo, const int numberOfInstances)
{
	pair<unsigned char, int> value = make_pair(centreWordPOSambiguityInfo, numberOfInstances);
	POStaggerMap.insert(pair<string, pair<unsigned char, int>>(POSambiguityInfoPermutation, value));
}	
multimap<string, pair<unsigned char, int>>* GIApreprocessorPOStaggerClass::getPOStaggerMap()
{
	return &POStaggerMap;
}
#endif


#ifdef GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_WIKI_DUMP_TEXT
bool wikiDumpWordIndexListLoaded;
unordered_map<string, GIApreprocessorMultiwordReductionWord*> wikiDumpWordIndexListGlobal;

bool GIApreprocessorPOStaggerClass::createWordIndexListFromWikiDumpText(bool useLRP)
{
	bool result = true;
	
	for(int wikiDumpFileBatchIndex=0; wikiDumpFileBatchIndex<GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NUMBER_OF_FILES; wikiDumpFileBatchIndex++)
	{
		string inputFileName = generateWikiDumpTextInputFileName(wikiDumpFileBatchIndex);
		GIApreprocessorSentence* firstGIApreprocessorSentenceInWikiDumpText = new GIApreprocessorSentence();
		if(!generatePreprocessorSentenceObjectsFromText(inputFileName, firstGIApreprocessorSentenceInWikiDumpText, useLRP))
		{
			result = false;
		}

		//add words to wikiDumpWordIndexListGlobal
		GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInWikiDumpText;
		while(currentGIApreprocessorSentenceInList->next != NULL)
		{	
			for(int w=0; w<currentGIApreprocessorSentenceInList->sentenceContentsLRP.size(); w++)
			{		
				GIApreprocessorWord* currentWord = (currentGIApreprocessorSentenceInList->sentenceContentsLRP)[w];
				string wordLowerCase = SHAREDvars.convertStringToLowerCase(&(currentWord->tagName));	//CHECKTHIS: verify that currentWord->tagName is case sensitive
				wikiDumpWordIndexListGlobal->insert(pair<string, GIApreprocessorMultiwordReductionWord*>(wordLowerCase, currentWord));
			}
			currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
		}		

		delete firstGIApreprocessorSentenceInWikiDumpText;
	}
	
	wikiDumpWordIndexListLoaded = true;
	
	return result;	
}
#endif

#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE

#ifndef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY
//UNTESTED
bool GIApreprocessorPOStaggerClass::generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, bool useLRP)
{
	bool result = true;

	string currentFolder = SHAREDvarsClass().getCurrentDirectory();
			
	for(int wikiDumpFileBatchIndex=0; wikiDumpFileBatchIndex<GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NUMBER_OF_FILES; wikiDumpFileBatchIndex++)
	{
		if(!generatePOStaggerDatabaseFromWikiDumpText(wikiDumpFolderName, wikiDumpFileBatchIndex, useLRP))
		{
			result = false;
		}
	}

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	GIApreprocessorPOStaggerDatabase.writeDatabaseNeuralNetwork();
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_EXECUTE_FEED
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_EXECUTE_FEED_SINGLE_EPOCH_ONLY
	GIApreprocessorPOStaggerDatabase.externalANNtrainEpoch();
	#else
	GIApreprocessorPOStaggerDatabase.externalANNtrain();
	#endif
	#endif
	#endif
	#endif
	
	SHAREDvarsClass().setCurrentDirectory(currentFolder);

	return result;
}
#endif

bool GIApreprocessorPOStaggerClass::generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, const int wikiDumpFileBatchIndex, bool useLRP)
{
	bool result = true;
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	if(!GIApreprocessorMultiwordReduction.createWordIndexListFromLRPfiles())
	{
		result = false;
	}
	#else
	cerr << "GIApreprocessorPOStaggerClass::generatePOStaggerDatabaseFromWikiDumpText{} error: !GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES; word index list currently needs to be regenerated everytime POS tagger database generation occurs" << endl;
	#endif
	
	string inputFileName = generateWikiDumpTextInputFileName(wikiDumpFileBatchIndex);
	cout << "wikiDumpFileName = " << inputFileName << endl;
	SHAREDvarsClass().setCurrentDirectory(wikiDumpFolderName);
	GIApreprocessorSentence* firstGIApreprocessorSentenceInWikiDumpText = new GIApreprocessorSentence();
	if(!generatePreprocessorSentenceObjectsFromText(inputFileName, firstGIApreprocessorSentenceInWikiDumpText, useLRP))
	{
		result = false;
	}
	
	//#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_ANN_DEBUG_WRITE_NN_FREQUENTLY
	long numberOfNeuralNetworkFeeds = 0;
	//#endif
	
	//generate POS tagger database entries
	cout << "finished generatePreprocessorSentenceObjectsFromText" << endl;
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
	string XtrainBatchFileName = GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchFileName(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_X_TRAIN_BATCH_FILE_NAME_PARTA, wikiDumpFileBatchIndex);
	string YtrainBatchFileName = GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchFileName(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_Y_TRAIN_BATCH_FILE_NAME_PARTA, wikiDumpFileBatchIndex);
	ofstream XtrainBatchFileObject(XtrainBatchFileName.c_str());
	ofstream YtrainBatchFileObject(YtrainBatchFileName.c_str());
	#else
	ANNexperience* firstExperienceInList = new ANNexperience();
	ANNexperience* currentExperienceInList = firstExperienceInList;
	#endif
	#endif
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInWikiDumpText;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		for(int wCentre=0; wCentre<currentGIApreprocessorSentenceInList->sentenceContentsLRP.size(); wCentre++)
		{
			//centre word calculations
			GIApreprocessorWord* centreWord = (currentGIApreprocessorSentenceInList->sentenceContentsLRP)[wCentre];
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_POS_DEBUG
			cout << "centreWord = " << centreWord->tagName << endl;
			#endif
			
			#ifndef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION_EVEN_IF_AMBIGUOUS
			unsigned int centreWordPOSambiguityInfoSpecialCharacterTemp = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;
			if(!determinePOSambiguityInfoForSpecialCharacters(centreWord, &centreWordPOSambiguityInfoSpecialCharacterTemp))
			{//ignore centre words that are special characters
			#endif
				//cout << "centreWord = " << centreWord->tagName << endl;
				
				bool identifiedEveryWordInDatabasePOSpermutation = true;
				bool identifiedEveryWordInDatabasePOSpermutationIsUnambiguous = true;
				
				string centreWordLowerCase = SHAREDvars.convertStringToLowerCase(&(centreWord->tagName));	//CHECKTHIS: verify that currentWord->tagName is case sensitive
				GIApreprocessorMultiwordReductionWord* centreWordFound = NULL;
				//bool identifiedCentreWordInDatabasePOSpermutation = true;
				unsigned int centreWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
				if(!findWordInWordListAllTypesWithPOSambiguityInfo(centreWordLowerCase, &centreWordFound, &centreWordPOSambiguityInfo))
				{
					//identifiedCentreWordInDatabasePOSpermutation = false;
					identifiedEveryWordInDatabasePOSpermutation = false;
				}

				vector<unsigned int> POSambiguityInfoPermutation;
				if(!generatePOSambiguityInfoPermutation(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), wCentre, &identifiedEveryWordInDatabasePOSpermutation, &identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, &POSambiguityInfoPermutation))
				{
					result = false;
				}

				#ifdef GIA_PREPROCESSOR_POS_TAGGER_ONLY_ADD_DATABASE_ENTRY_IF_POS_AMBIGUITY_INFO_FOUND_FOR_EVERY_CONTEXT_WORD
				if(identifiedEveryWordInDatabasePOSpermutation)
				{
				#endif
					unsigned int centreWordUnambiguousPOSvalue = INT_DEFAULT_VALUE;
					bool centreWordAmbiguous = determinePOSambiguityInfoIsAmbiguous(centreWordPOSambiguityInfo, &centreWordUnambiguousPOSvalue, true);
					#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
					if(identifiedEveryWordInDatabasePOSpermutationIsUnambiguous && !centreWordAmbiguous)
					{
					#endif
						#ifndef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION_EVEN_IF_AMBIGUOUS
						if(!centreWordAmbiguous)
						{
						#endif
							#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
							vector<vector<unsigned int>*> POSambiguityInfoUnambiguousPermutationArray;
							vector<unsigned int>* POSambiguityInfoUnambiguousPermutationNew = new vector<unsigned int>(POSambiguityInfoPermutation.size());
							POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
							generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, &POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
							for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray.size(); i++)
							{
								vector<unsigned int>* POSambiguityInfoPermutationTemp = POSambiguityInfoUnambiguousPermutationArray[i];
							#else
								vector<unsigned int>* POSambiguityInfoPermutationTemp = &POSambiguityInfoPermutation;
							#endif
								#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
								#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
								if(!addPOStaggerDatabaseEntry(centreWordPOSambiguityInfo, POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSvalue, &XtrainBatchFileObject, &YtrainBatchFileObject))
								#else
								if(!addPOStaggerDatabaseEntry(centreWordPOSambiguityInfo, POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSvalue, currentExperienceInList))
								#endif
								#else
								if(!addPOStaggerDatabaseEntry(centreWordPOSambiguityInfo, POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSvalue, &numberOfNeuralNetworkFeeds))
								#endif
								{
									result = false;
								}
							#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
							}
							#endif
						#ifndef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION_EVEN_IF_AMBIGUOUS
						}
						#endif  

						#ifdef GIA_PREPROCESSOR_POS_TAGGER_POS_DEBUG
						cout << "insertInstanceInPOStaggerMap: POSambiguityInfoPermutation = " << POSambiguityInfoPermutation << ", centreWordPOSambiguityInfo = " << centreWordPOSambiguityInfo << endl;
						#endif
					#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
					}
					#endif
				#ifdef GIA_PREPROCESSOR_POS_TAGGER_ONLY_ADD_DATABASE_ENTRY_IF_POS_AMBIGUITY_INFO_FOUND_FOR_EVERY_CONTEXT_WORD
				}
				#endif
			#ifndef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION_EVEN_IF_AMBIGUOUS
			}
			#endif
		}

		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MEMORY_FREE_DELETE_PREPROCESSOR_SENTENCES_ONCE_PROCESSED
		GIApreprocessorSentence* previousGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList;
		#endif		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MEMORY_FREE_DELETE_PREPROCESSOR_SENTENCES_ONCE_PROCESSED
		previousGIApreprocessorSentenceInList->next = NULL;	//prevents future sentences from being deleted
		delete previousGIApreprocessorSentenceInList;
		#endif
	}		

	#ifndef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MEMORY_FREE_DELETE_PREPROCESSOR_SENTENCES_ONCE_PROCESSED
	delete firstGIApreprocessorSentenceInWikiDumpText;
	#endif
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_ANN_DEBUG_WRITE_NN_FREQUENTLY
	GIApreprocessorPOStaggerDatabase.writeDatabaseNeuralNetwork();
	string neuralNetworkXmlFileName = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_DEFAULT_XML_FILE_NAME;
	string neuralNetworkXmlFileNameBackup = neuralNetworkXmlFileName + STRING_FULLSTOP + SHAREDvars.convertIntToString(wikiDumpFileBatchIndex);
	SHAREDvars.copyFiles(neuralNetworkXmlFileName, neuralNetworkXmlFileNameBackup);
	cout << "B numberOfNeuralNetworkFeeds = " << numberOfNeuralNetworkFeeds << endl;
	//exit(EXIT_ERROR);
	#endif
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
	XtrainBatchFileObject.close();
	YtrainBatchFileObject.close();
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_EXECUTE_FEED_SINGLE_BATCH_ONLY
	GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchTrainDataExecuteFeed();
	#endif
	#else
	GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchTrainData(firstExperienceInList, wikiDumpFileBatchIndex);
	delete firstExperienceInList;
	#endif
	#endif
	#endif
	
	return result;
}

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
bool GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry(const unsigned int centreWordPOSambiguityInfo, vector<unsigned int>* POSambiguityInfoPermutation, const unsigned int centreWordUnambiguousPOSvalue, ofstream* XtrainBatchFileObject, ofstream* YtrainBatchFileObject)
#else
bool GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry(const unsigned int centreWordPOSambiguityInfo, vector<unsigned int>* POSambiguityInfoPermutation, const unsigned int centreWordUnambiguousPOSvalue, ANNexperience** currentExperienceInList)
#endif
#else
bool GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry(const unsigned int centreWordPOSambiguityInfo, vector<unsigned int>* POSambiguityInfoPermutation, const unsigned int centreWordUnambiguousPOSvalue, long* numberOfNeuralNetworkFeeds)
#endif
{
	bool result = true;
	//cout << "addPOStaggerDatabaseEntry" << endl;
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	ANNexperience* currentExperienceInList = new ANNexperience();
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET
	unsigned int centreWordUnambiguousPOSvalueTemp = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET_TRAIN_EXPERIENCE_OUTPUT_VALUE_NOT_USED;
	#else
	unsigned int centreWordUnambiguousPOSvalueTemp = centreWordUnambiguousPOSvalue; 
	#endif
	generateANNexperienceFromPOSambiguityInfoPermutation(POSambiguityInfoPermutation, centreWordUnambiguousPOSvalueTemp, currentExperienceInList);
	GIApreprocessorPOStaggerDatabase.feedNeuralNetworkWithExperience(currentExperienceInList);
	delete currentExperienceInList;
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_ANN_DEBUG_WRITE_NN_FREQUENTLY
	(*numberOfNeuralNetworkFeeds) = (*numberOfNeuralNetworkFeeds) + 1;
	if((*numberOfNeuralNetworkFeeds)%100000 == 0)
	{
		GIApreprocessorPOStaggerDatabase.writeDatabaseNeuralNetwork();
		cout << "A numberOfNeuralNetworkFeeds = " << numberOfNeuralNetworkFeeds << endl;
	}
	#endif
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
	ANNexperience* currentExperienceInList = new ANNexperience();
	#endif
	generateANNexperienceFromPOSambiguityInfoPermutation(POSambiguityInfoPermutation, centreWordUnambiguousPOSvalue, currentExperienceInList);
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
	string experienceInputString = GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchDataExperienceInput(currentExperienceInList);
	string experienceOutputString = GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchDataExperienceOutput(currentExperienceInList);
	*XtrainBatchFileObject << experienceInputString << endl;
	*YtrainBatchFileObject << experienceOutputString << endl;
	delete currentExperienceInList;
	#else
	(*currentExperienceInList)->next = new ANNexperience();
	(*currentExperienceInList) = (*currentExperienceInList)->next;
	#endif
	#endif
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM
	for(int i=0; i<POSambiguityInfoPermutation->size(); i++)
	{
		unsigned int val = (*POSambiguityInfoPermutation)[i]; 
		//cout << "val = " << val << endl;
	}
	string POSambiguityInfoPermutationString = convertPOSambiguityInfoPermutationToString(POSambiguityInfoPermutation);
	unsigned char centreWordPOSambiguityInfoChar = convertPOSambiguityInfoToChar(centreWordPOSambiguityInfo);
	//cout << "POSambiguityInfoPermutationString = " << POSambiguityInfoPermutationString << endl;
	//cout << "centreWordPOSambiguityInfoString = " << centreWordPOSambiguityInfoString << endl;
	if(!GIApreprocessorPOStaggerDatabase.DBwritePOSpermutationEstimate(POSambiguityInfoPermutationString, centreWordPOSambiguityInfoChar))
	{
		result = false;
	}
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MAP
	unsigned int centreWordPOSambiguityInfoFound;
	int numberOfInstances = 0;
	bool incrementIfFound = true;
	string POSambiguityInfoPermutationString = convertPOSambiguityInfoPermutationToString(&POSambiguityInfoPermutation);
	unsigned char centreWordPOSambiguityInfoChar = convertPOSambiguityInfoToChar(centreWordPOSambiguityInfo);
	if(!findInstancePOStaggerMap(POSambiguityInfoPermutationString, centreWordPOSambiguityInfoChar, &numberOfInstances, incrementIfFound))
	{
		numberOfInstances = 1;
		insertInstanceInPOStaggerMap(POSambiguityInfoPermutationString, centreWordPOSambiguityInfoChar, numberOfInstances);
	}	
	#endif
	
	return result;	
}

string GIApreprocessorPOStaggerClass::generateWikiDumpTextInputFileName(int wikiDumpFileBatchIndex)
{
	string formatString = GIApreprocessorPOStaggerDatabase.generateIntFormatString(GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NAME_PART_B_INDEX_NUMBER_OF_CHARACTERS);
	string inputFileName = string(GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NAME_PART_A) + SHAREDvars.convertIntToString(wikiDumpFileBatchIndex, formatString) + GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NAME_PART_C;
	return inputFileName;
}


bool GIApreprocessorPOStaggerClass::generatePreprocessorSentenceObjectsFromText(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInText, bool useLRP)
{
	bool result = true;
	
	//generate preprocessor sentence objects from text
	string fileContents = SHAREDvars.getFileContents(inputFileName);
	bool interpretNewLinesAsNewSentences = true;	//CHECKTHIS
	//cout << "fileContents = " << fileContents << endl;
	if(!createPreprocessSentencesBasic(fileContents, firstGIApreprocessorSentenceInText, interpretNewLinesAsNewSentences))
	{
		result = false;
	}	

	//code replicated from createPreprocessSentencesForGIA
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInText;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		currentGIApreprocessorSentenceInList->sentenceContentsLRP = currentGIApreprocessorSentenceInList->sentenceContentsOriginal;
		
		#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
		for(int w=0; w<currentGIApreprocessorSentenceInList->sentenceContentsLRP.size(); w++)
		{		
			GIApreprocessorMultiwordReductionPlainTextWord* sentenceContentsLRPcurrentWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((currentGIApreprocessorSentenceInList->sentenceContentsLRP)[w]);
			GIApreprocessorMultiwordReductionPlainTextWord* sentenceContentsOriginalcurrentWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((currentGIApreprocessorSentenceInList->sentenceContentsLRP)[w]);
			sentenceContentsLRPcurrentWord->preprocessorUpperLevelWordReference = sentenceContentsOriginalcurrentWord;
			sentenceContentsLRPcurrentWord->preprocessorUpperLevelWordReferenceSize = 1;
		}
		#endif
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	if(useLRP)
	{
		//perform multiword reduction
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
		bool isQuery = false;

		string outputLRPTextPlainTXTFileName = GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WIKIDUMP_MULTIWORD_FILE_NAME;
		string outputLRPTextForNLPonlyPlainTXTFileNameBase = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_OUTPUT_FOR_NLP_ONLY_FILE_EXTENSION;

		string outputLRPTextPlainTXTFileNameWikiDumpMultiword = string(outputLRPTextPlainTXTFileName) + GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WIKIDUMP_MULTIWORD_FILE_EXTENSION;
		string outputLRPTextForNLPonlyPlainTXTFileNameWikiDumpMultiword = string(outputLRPTextForNLPonlyPlainTXTFileNameBase) + GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WIKIDUMP_MULTIWORD_FILE_EXTENSION;
		GIApreprocessorMultiwordReduction.initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);
		GIApreprocessorMultiwordReduction.setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	//required for local variable access
		if(!GIApreprocessorMultiwordReduction.parseTextFileAndReduceLanguage(firstGIApreprocessorSentenceInText, outputLRPTextPlainTXTFileNameWikiDumpMultiword, outputLRPTextForNLPonlyPlainTXTFileNameWikiDumpMultiword))
		{
			result = false;
		}
		GIApreprocessorMultiwordReduction.deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	//required so as not to intefere with GIA preprocessor
		#endif
	}
	
	return result;
}	

bool GIApreprocessorPOStaggerClass::createPreprocessSentencesBasic(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences)
{	
	bool result = true;
	
	int charCount = 0;
	char currentToken;
	string currentWord = "";
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	GIApreprocessorMultiwordReductionPlainTextWord* firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
	GIApreprocessorMultiwordReductionPlainTextWord* currentWordInSentence = firstWordInSentence;
	string sentenceContentsOriginalText = "";
	int entityIndex = GIA_NLP_START_ENTITY_INDEX;	//only assigned after collapse?
	int sentenceIndex = GIA_NLP_START_SENTENCE_INDEX;
	
	while(charCount < fileContents.length())
	{		
		currentToken = fileContents[charCount];
		sentenceContentsOriginalText = sentenceContentsOriginalText + currentToken;
		
		bool punctuationMarkFound = false;
		if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
		{
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
			if(!GIApreprocessorMultiwordReductionClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
			{
			#endif
				punctuationMarkFound = true;
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
			}
			#endif
		}
		
		bool whiteSpaceFound = false;
		if(SHAREDvars.charInCharArray(currentToken, nlpWhitespaceCharacterArray, GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS))	//NB this includes CHAR_NEWLINE
		{
			whiteSpaceFound = true;
		}
		
		bool newlineFound = false;
		if(currentToken == CHAR_NEWLINE)
		{
			newlineFound = true;
		}
		
		bool specialCharFound = false;
		if(!SHAREDvars.charInCharArray(currentToken, preprocessorLowerOrUpperCaseLettersArray, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_OR_UPPER_CASE_LETTERS_NUMBER_OF_TYPES))
		{
			specialCharFound = true;
		}
		
		/*
		//not required because it will be interpreted as specialCharFound
		bool quotationCharFound = false;
		if(!SHAREDvars.charInCharArray(currentToken, nlpQuotationMarkCharacterArray, GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS))
		{
			quotationCharFound = true;
		}
		*/
		
		bool endOfSentencePunctuationMarkFound = false;

		if(whiteSpaceFound || newlineFound || punctuationMarkFound || specialCharFound)
		{
			bool lastWordBlank = true;
			
			int lastCharacterIndexOfLastWordBeingFilled = charCount-1; 
			//cout << "currentWord = " << currentWord << ", currentToken = " << currentToken << endl;
			
			if(currentWord != "")
			{//do not add empty tag after closing quotation marks	//e.g. GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS or (newlineFound && interpretNewLinesAsNewSentences && previousCharacter==whitespace)
				lastWordBlank = false;
				GIApreprocessorMultiwordReductionClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, lastCharacterIndexOfLastWordBeingFilled);
			}
			
			if(punctuationMarkFound)
			{
				string punctuationMark = ""; 
				punctuationMark = punctuationMark + currentToken;
				GIApreprocessorMultiwordReductionClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &punctuationMark, &entityIndex, charCount);

				if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
				{
					#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
					if(!GIApreprocessorMultiwordReductionClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
					{
					#endif
						endOfSentencePunctuationMarkFound = true;
					#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
					}
					#endif
				}
				
				bool lastCharacterInFile = false;
				#ifndef GIA_EXPECT_NEWLINE_AT_END_OF_INPUT_TEXT_FILE
				if(charCount == fileContents.length()-1)
				{
					lastCharacterInFile = true;
				}
				#endif
				if(endOfSentencePunctuationMarkFound)
				{
					GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
					currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
					currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
					currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
					currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
					firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
					currentWordInSentence = firstWordInSentence;
					sentenceContentsOriginalText = "";
					entityIndex = 0;
					sentenceIndex++;
				}
			}
			else if(newlineFound && interpretNewLinesAsNewSentences)
			{
				#ifdef GIA_PREPROCESSOR_DISALLOW_EMPTY_SENTENCE_OBJECTS
				if(firstWordInSentence->nextTag != NULL)
				{
				#endif
					GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
					currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
					currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
					currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
					currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
					firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
					currentWordInSentence = firstWordInSentence;
					sentenceContentsOriginalText = "";
					entityIndex = 0;
					sentenceIndex++;
				#ifdef GIA_PREPROCESSOR_DISALLOW_EMPTY_SENTENCE_OBJECTS
				}
				#endif
			}
			
		}
		else
		{
			currentWord = currentWord + currentToken;
		}
		
		charCount++;
	}
		
	return result;
}
#endif

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
void GIApreprocessorPOStaggerClass::generatePOSambiguityInfoUnambiguousPermutationArray(vector<vector<unsigned int>*>* POSambiguityInfoUnambiguousPermutationArray, vector<unsigned int>* POSambiguityInfoPermutation, vector<unsigned int>* POSambiguityInfoUnambiguousPermutationLocal, int wordIndex)
{
	if(wordIndex < GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION)
	{
		unsigned int POSambiguityInfo = (*POSambiguityInfoPermutation)[wordIndex];
		int maxValForPOSambiguousEntry = 1U << GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE;
		//cout << "maxValForPOSambiguousEntry = " << maxValForPOSambiguousEntry << endl;
		//cout << "POSambiguityInfo = " << POSambiguityInfo << endl;

		if((POSambiguityInfo > 0) && (POSambiguityInfo < maxValForPOSambiguousEntry))
		{
			//remove POSambiguityInfoUnambiguousPermutationLocal from POSambiguityInfoUnambiguousPermutationArray as it will be replicated
			vector<vector<unsigned int>*>::iterator it = find(POSambiguityInfoUnambiguousPermutationArray->begin(), POSambiguityInfoUnambiguousPermutationArray->end(), POSambiguityInfoUnambiguousPermutationLocal);
  			if(it != POSambiguityInfoUnambiguousPermutationArray->end())
  			{
				POSambiguityInfoUnambiguousPermutationArray->erase(it);
				//cout << "remove POSambiguityInfoUnambiguousPermutationLocal from POSambiguityInfoUnambiguousPermutationArray as it will be replicated" << endl;
			}
			else
			{
				cerr << "GIApreprocessorPOStaggerClass::generatePOSambiguityInfoUnambiguousPermutationArray error{}: POSambiguityInfoUnambiguousPermutationLocal not found in POSambiguityInfoUnambiguousPermutationArray" << endl;
				exit(EXIT_ERROR);
			}
			
			for(int x=0; x<GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE; x++)
			{
				if(SHAREDvars.getBitValue(POSambiguityInfo, x))
				{
					vector<unsigned int>* POSambiguityInfoUnambiguousPermutationNew = new vector<unsigned int>(*POSambiguityInfoUnambiguousPermutationLocal);
					POSambiguityInfoUnambiguousPermutationArray->push_back(POSambiguityInfoUnambiguousPermutationNew);
					(*POSambiguityInfoUnambiguousPermutationNew)[wordIndex] = SHAREDvars.setBitValue(0, x, true);	//create a new unambigious hypothetical POS value
					generatePOSambiguityInfoUnambiguousPermutationArray(POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, wordIndex+1);
				}
			}
			
			delete POSambiguityInfoUnambiguousPermutationLocal;
		}
		else
		{
			//special POS type (e.g. punctuation)
			(*POSambiguityInfoUnambiguousPermutationLocal)[wordIndex] = POSambiguityInfo;
			generatePOSambiguityInfoUnambiguousPermutationArray(POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationLocal, wordIndex+1);
		}
	}
}						
#endif

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
bool GIApreprocessorPOStaggerClass::generateANNexperienceFromPOSambiguityInfoPermutation(vector<unsigned int>* POSambiguityInfoPermutation, const int centreWordUnambiguousPOSvalue, ANNexperience* currentExperience)
{
	bool result = true;
	
	vector<bool> inputNeuronExperienceValues(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS, false);
	for(int i=0; i<GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION; i++)
	{
		int POSambiguityInfoNeuralNetworkInputNeuron = INT_DEFAULT_VALUE;
		vector<bool> inputNeuronExperienceValuesContextWord(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD, false);	
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT						
		if(determinePOSambiguityInfoNeuralNetworkInputNeuronForSpecialCharacters((*POSambiguityInfoPermutation)[i], &POSambiguityInfoNeuralNetworkInputNeuron))
		{
			inputNeuronExperienceValuesContextWord[POSambiguityInfoNeuralNetworkInputNeuron] = true;
		}
		else
		{
			int numberOfPOStypesToCheck = GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE;
		#else
			int numberOfPOStypesToCheck = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD;
		#endif
			for(int POStype = 0; POStype<numberOfPOStypesToCheck; POStype++)
			{
				bool bitValue = SHAREDvars.getBitValue((*POSambiguityInfoPermutation)[i], POStype);
				inputNeuronExperienceValuesContextWord[POStype] = bitValue;
			}
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT	
		}
		#endif

		for(int j=0; j<GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD; j++)
		{
			inputNeuronExperienceValues[i*GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD+j] = inputNeuronExperienceValuesContextWord[j];	
		}
	}
	
	/*
	for(int i=0; i<POSambiguityInfoPermutation.size(); i++)
	{
		cout << int((*POSambiguityInfoPermutation)[i]);
	}
	cout << endl;
	cout << "inputNeuronExperienceValues = " << convertBoolVectorToString(&inputNeuronExperienceValues) << endl;
	*/
	
	int outputNeuronExperienceValueCentreWord = centreWordUnambiguousPOSvalue;
	//only feed neural network with experience if centre word POS is not ambiguous
	//now create an experience and train the network with the experience
	currentExperience->classTargetValue = outputNeuronExperienceValueCentreWord;	//CHECKTHIS
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
	for(int i=0; i<GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS; i++)
	{
		currentExperienceInput->inputValue = inputNeuronExperienceValues[i];
		currentExperienceInput->next = new ANNexperienceInput();
		currentExperienceInput = currentExperienceInput->next;
	}
	
	return result;
}

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT	
bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfoNeuralNetworkInputNeuronForSpecialCharacters(const unsigned int POSambiguityInfo, int* POSambiguityInfoNeuralNetworkInputNeuron)
{
	bool specialCharFound = false;

	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_END_OF_SENTENCE)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_PUNCTUATION_MARK_CHARACTER_END_OF_SENTENCE;
	}
	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_OTHER)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_PUNCTUATION_MARK_CHARACTER_OTHER;
	}
	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_QUOTATION_CHARACTER)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_QUOTATION_CHARACTER;
	}
	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_OUT_OF_SENTENCE_BOUNDS)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_OUT_OF_SENTENCE_BOUNDS;
	}
		
	return specialCharFound;
}
#endif
#endif

int GIApreprocessorPOStaggerClass::convertGIAPOStaggerValueToGrammaticalWordType(const int POSvalue)
{
	int grammaticalWordType = GIAPOStaggerValueCrossReferenceGrammaticalWordTypeArray[POSvalue];
	return grammaticalWordType;
}
	
bool GIApreprocessorPOStaggerClass::generatePOSambiguityInfoPermutation(vector<GIApreprocessorWord*>* sentenceContentsLRP, int wCentre, bool* identifiedEveryWordInDatabasePOSpermutation, bool* identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, vector<unsigned int>* POSambiguityInfoPermutation)
{
	bool result = true;
	
	GIApreprocessorWord* centreWord = (*sentenceContentsLRP)[wCentre];

	//context word calculations	
	int wMin = wCentre - (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2);
	int wMax = wCentre + (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2);
	int wSize = wMax - wMin;	//GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION
	POSambiguityInfoPermutation->resize(GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION, GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN);	//GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION = max size of wSize
	int POSambiguityInfoPermutationIndex = 0;	

	for(int w=wMin; w<=wMax; w++)
	{	
		/*
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION_OLD
		if(w == wCentre)	
		{
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
			(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_CENTRE_WORD;
			#else
			(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INPUT_CENTRE_WORD_NOTUSED, true);
			#endif
			POSambiguityInfoPermutationIndex++;
			#endif
		}
		#else
		*/
		#ifndef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION
		if(w == wCentre)	
		{
			//ignore centre word
		}
		else 
		#endif
		/*
		#endif
		*/
		if((w < 0) || (w >= sentenceContentsLRP->size()))
		{
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
			(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_OUT_OF_SENTENCE_BOUNDS;	
			#else
			(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_OUT_OF_SENTENCE_BOUNDS, true);	
			#endif
			POSambiguityInfoPermutationIndex++;					
		}
		else
		{
			GIApreprocessorWord* contextWord = (*sentenceContentsLRP)[w];
			string contextWordLowerCase = SHAREDvars.convertStringToLowerCase(&(contextWord->tagName));	//CHECKTHIS: verify that currentWord->tagName is case sensitive

			GIApreprocessorMultiwordReductionWord* contextWordFound = NULL;
			unsigned int contextWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
			if(!findWordInWordListAllTypesWithPOSambiguityInfo(contextWordLowerCase, &contextWordFound, &contextWordPOSambiguityInfo))
			{
				if(!determinePOSambiguityInfoForSpecialCharacters(contextWord, &contextWordPOSambiguityInfo))
				{
					*identifiedEveryWordInDatabasePOSpermutation = false;
					#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
					*identifiedEveryWordInDatabasePOSpermutationIsUnambiguous = false;
					#endif
				}
			}
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
			else
			{
				unsigned int wordUnambiguousPOSvalueNOTUSED = INT_DEFAULT_VALUE;
				bool wordAmbiguous = determinePOSambiguityInfoIsAmbiguous(contextWordPOSambiguityInfo, &wordUnambiguousPOSvalueNOTUSED, false);
				if(wordAmbiguous)
				{
					*identifiedEveryWordInDatabasePOSpermutationIsUnambiguous = false;
				}
			}
			#endif

			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
			if(contextWordPOSambiguityInfo > GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_MAX_SIZE)	//error checking
			{
				cerr << "GIApreprocessorPOStaggerClass::generatePOStaggerDatabaseFromWikiDumpText{} error: (contextWordPOSambiguityInfo > GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_MAX_SIZE{255})" << endl;
				exit(EXIT_ERROR);
			}
			#endif
			
			(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = contextWordPOSambiguityInfo;
			//cout << "POSambiguityInfoPermutationIndex = " << POSambiguityInfoPermutationIndex << endl;
			POSambiguityInfoPermutationIndex++;
		}
	}
		
	return result;
}	

bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfoIsAmbiguous(const unsigned int POSambiguityInfo, unsigned int* unambiguousPOSvalue, const bool treatWordAsAmbiguousIfNullPOSvalue)
{
	bool ambiguous = false;
	
	int numberOfPOStypesRecorded = 0;
	for(int POStype = 0; POStype<GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE; POStype++)
	{
		bool bitValue = SHAREDvars.getBitValue(POSambiguityInfo, POStype);
		if(bitValue)
		{
			*unambiguousPOSvalue = POStype;
			numberOfPOStypesRecorded++;
		}
	}
	if(numberOfPOStypesRecorded > 1)
	{
		ambiguous = true;
	}
	if(POSambiguityInfo == 0)
	{
		if(treatWordAsAmbiguousIfNullPOSvalue)
		{
			ambiguous = true;
		}
		else
		{
			*unambiguousPOSvalue = 0;
		}
	}

	#ifndef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
	if(!ambiguous)
	{
		//this is required when determinePOSambiguityInfoIsAmbiguous is being used to calculate unambiguousPOSvalue for special characters also
		for(int POStype = 0; POStype<GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_MAX_BITS_INTERMEDIATE; POStype++)
		{
			bool bitValue = SHAREDvars.getBitValue(POSambiguityInfo, POStype);
			if(bitValue)
			{
				*unambiguousPOSvalue = POStype;
			}
		}
	}
	#endif
				
	return ambiguous;
}	

bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfoForSpecialCharacters(const GIApreprocessorWord* word, unsigned int* POSambiguityInfo)
{
	bool specialCharFound = false;
	if((word->tagName).length() == 1)
	{
		char wordChar = (word->tagName)[0];
		if(SHAREDvars.charInCharArray(wordChar, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
		{
			specialCharFound = true;
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
			*POSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_END_OF_SENTENCE;
			#else
			*POSambiguityInfo = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_PUNCTUATION_MARK_CHARACTER_END_OF_SENTENCE, true);
			#endif
		}
		if(SHAREDvars.charInCharArray(wordChar, nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
		{
			specialCharFound = true;
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
			*POSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_OTHER;
			#else
			*POSambiguityInfo = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_PUNCTUATION_MARK_CHARACTER_OTHER, true);
			#endif
		}
		if(SHAREDvars.charInCharArray(wordChar, nlpQuotationMarkCharacterArray, GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS))
		{
			specialCharFound = true;
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
			*POSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_QUOTATION_CHARACTER;
			#else
			*POSambiguityInfo = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_QUOTATION_CHARACTER, true);
			#endif
		}
	}
	return specialCharFound;
}



bool GIApreprocessorPOStaggerClass::findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, unsigned int* POSambiguityInfoFound)
{	
	bool result = false;
		
	unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>* wordListAllTypesWithPOSambiguityInfo = GIApreprocessorMultiwordReduction.getWordListAllTypesWithPOSambiguityInfo();

	unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>::iterator it;
	it = wordListAllTypesWithPOSambiguityInfo->find(word);
	if(it != wordListAllTypesWithPOSambiguityInfo->end())
	{
		result = true;
		*wordFound = (it->second).first;
		*POSambiguityInfoFound = (unsigned int)((it->second).second);
	}
	
	return result;
}	

string GIApreprocessorPOStaggerClass::convertBoolVectorToString(vector<bool>* inputNeuronExperienceValuesContextWord)
{
	string boolVectorString = "";
	for(int i=0; i<inputNeuronExperienceValuesContextWord->size(); i++)
	{
		boolVectorString = boolVectorString + SHAREDvars.convertIntToString(int(inputNeuronExperienceValuesContextWord->at(i)));
	}
	return boolVectorString;
}

string GIApreprocessorPOStaggerClass::convertPOSambiguityInfoPermutationToString(vector<unsigned int>* POSambiguityInfoPermutation)
{
	string POSambiguityInfoPermutationString = "";
	for(int i=0; i<POSambiguityInfoPermutation->size(); i++)
	{
		POSambiguityInfoPermutationString = POSambiguityInfoPermutationString + (char)convertPOSambiguityInfoToChar((*POSambiguityInfoPermutation)[i]);
	}
	return POSambiguityInfoPermutationString;
}

unsigned char GIApreprocessorPOStaggerClass::convertPOSambiguityInfoToChar(unsigned int POSambiguityInfo)
{
	unsigned char POSambiguityInfoChar = (unsigned char)(0);
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT
	POSambiguityInfoChar = (unsigned char)(POSambiguityInfo);	//(char)(POSambiguityInfoPermutation->at(i));	//OLD: SHAREDvars.convertIntToString(unsignedIntVector->at(i));	
	#else
	unsigned int unambiguousPOSvalue = INT_DEFAULT_VALUE;
	if(!determinePOSambiguityInfoIsAmbiguous(POSambiguityInfo, &unambiguousPOSvalue, false))
	{
		//cout << "unambiguousPOSvalue = " << int(unambiguousPOSvalue) << endl;
		//cout << "unambiguousPOSvalue = " << int((unsigned char)(char(int(unambiguousPOSvalue)))) << endl;
		POSambiguityInfoChar = (unsigned char)(unambiguousPOSvalue);
	}
	else
	{
		cerr << "GIApreprocessorPOStaggerClass::convertPOSambiguityInfoPermutationToString error: !GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT && determinePOSambiguityInfoIsAmbiguous; verify that GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS is true" << endl;
	}
	#endif
	return POSambiguityInfoChar;
}


#endif



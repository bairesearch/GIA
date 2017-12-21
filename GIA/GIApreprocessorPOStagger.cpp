/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIApreprocessorPOStagger.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e7c 16-December-2017
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
bool GIApreprocessorPOStaggerClass::generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, bool useLRP)
{
	bool result = true;

	string currentFolder = SHAREDvarsClass().getCurrentDirectory();
			
	for(int wikiDumpFileBatchIndex=0; wikiDumpFileBatchIndex<GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NUMBER_OF_FILES; wikiDumpFileBatchIndex++)
	{
		it(!generatePOStaggerDatabaseFromWikiDumpText(wikiDumpFileBatchIndex))
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
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_ANN_DEBUG_WRITE_NN_FREQUENTLY
	long numberOfNeuralNetworkFeeds = 0;
	#endif
	
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
			
			unsigned char centreWordPOSambiguityInfoSpecialCharacterTemp = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;
			if(!determinePOSambiguityInfoForSpecialCharacters(centreWord, &centreWordPOSambiguityInfoSpecialCharacterTemp))
			{//ignore centre words that are special characters
			
				//cout << "centreWord = " << centreWord->tagName << endl;
				
				bool identifiedEveryWordInDatabasePOSpermutation = true;
				
				string centreWordLowerCase = SHAREDvars.convertStringToLowerCase(&(centreWord->tagName));	//CHECKTHIS: verify that currentWord->tagName is case sensitive
				GIApreprocessorMultiwordReductionWord* centreWordFound = NULL;
				//bool identifiedCentreWordInDatabasePOSpermutation = true;
				unsigned char centreWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
				if(!findWordInWordListAllTypesWithPOSambiguityInfo(centreWordLowerCase, &centreWordFound, &centreWordPOSambiguityInfo))
				{
					//identifiedCentreWordInDatabasePOSpermutation = false;
					identifiedEveryWordInDatabasePOSpermutation = false;
				}

				string POSambiguityInfoPermutation = "";
				if(!generatePOSambiguityInfoPermutation(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), wCentre, &identifiedEveryWordInDatabasePOSpermutation, &POSambiguityInfoPermutation))
				{
					result = false;
				}

				#ifdef GIA_PREPROCESSOR_POS_TAGGER_ONLY_ADD_DATABASE_ENTRY_IF_POS_AMBIGUITY_INFO_FOUND_FOR_EVERY_CONTEXT_WORD
				if(identifiedEveryWordInDatabasePOSpermutation)
				{
				#endif
					int centreWordUnambiguousPOSvalue = INT_DEFAULT_VALUE;
					if(!determinePOSambiguityInfoIsAmbiguous(centreWordPOSambiguityInfo, &centreWordUnambiguousPOSvalue))	//CHECKTHIS
					{
						#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
						#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
						ANNexperience* currentExperienceInList = new ANNexperience();
						generateANNexperienceFromPOSambiguityInfoPermutation(POSambiguityInfoPermutation, centreWordUnambiguousPOSvalue, currentExperience);
						GIApreprocessorPOStaggerDatabase.feedNeuralNetworkWithExperienceBackpropagation(currentExperienceInList);
						delete currentExperienceInList;
						#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_ANN_DEBUG_WRITE_NN_FREQUENTLY
						numberOfNeuralNetworkFeeds++;
						if(numberOfNeuralNetworkFeeds%100000 == 0)
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
						XtrainBatchFileObject << experienceInputString << endl;
						YtrainBatchFileObject << experienceOutputString << endl;
						delete currentExperienceInList;
						#else
						currentExperienceInList->next = new ANNexperience();
						currentExperienceInList = currentExperienceInList->next;
						#endif
						#endif
						#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM
						//only add the reference if the centre word is unambiguous - CHECKTHIS
						if(!GIApreprocessorPOStaggerDatabase.DBwritePOSpermutationEstimate(POSambiguityInfoPermutation, centreWordPOSambiguityInfo))
						{
							result = false;
						}
						#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MAP
						unsigned char centreWordPOSambiguityInfoFound;
						int numberOfInstances = 0;
						bool incrementIfFound = true;
						if(!findInstancePOStaggerMap(POSambiguityInfoPermutation, centreWordPOSambiguityInfo, &numberOfInstances, incrementIfFound))
						{
							numberOfInstances = 1;
							insertInstanceInPOStaggerMap(POSambiguityInfoPermutation, centreWordPOSambiguityInfo, numberOfInstances);
						}	
						#endif	
					}
					
					#ifdef GIA_PREPROCESSOR_POS_TAGGER_POS_DEBUG
					cout << "insertInstanceInPOStaggerMap: POSambiguityInfoPermutation = " << POSambiguityInfoPermutation << ", centreWordPOSambiguityInfo = " << centreWordPOSambiguityInfo << endl;
					#endif
				#ifdef GIA_PREPROCESSOR_POS_TAGGER_ONLY_ADD_DATABASE_ENTRY_IF_POS_AMBIGUITY_INFO_FOUND_FOR_EVERY_CONTEXT_WORD
				}
				#endif
			}
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

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
bool GIApreprocessorPOStaggerClass::generateANNexperienceFromPOSambiguityInfoPermutation(const string POSambiguityInfoPermutation, const int centreWordUnambiguousPOSvalue, ANNexperience* currentExperience)
{
	bool result = true;
	
	vector<bool> inputNeuronExperienceValues(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS, false);
	for(int i=0; i<GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION; i++)
	{
		int POSambiguityInfoNeuralNetworkInputNeuron = INT_DEFAULT_VALUE;
		vector<bool> inputNeuronExperienceValuesContextWord(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD, false);							
		if(determinePOSambiguityInfoNeuralNetworkInputNeuronForSpecialCharacters(POSambiguityInfoPermutation[i], &POSambiguityInfoNeuralNetworkInputNeuron))
		{
			inputNeuronExperienceValuesContextWord[POSambiguityInfoNeuralNetworkInputNeuron] = true;
		}
		else
		{
			for(int POStype = 0; POStype<GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE; POStype++)
			{
				bool bitValue = SHAREDvars.getBitValue(POSambiguityInfoPermutation[i], POStype);
				inputNeuronExperienceValuesContextWord[POStype] = bitValue;
			}
		}

		for(int j=0; j<GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD; j++)
		{
			inputNeuronExperienceValues[i*GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD+j] = inputNeuronExperienceValuesContextWord[j];	
		}
	}
	
	/*
	cout << "POSambiguityInfoPermutation = " << endl;
	for(int i=0; i<POSambiguityInfoPermutation.length(); i++)
	{
		cout << int(POSambiguityInfoPermutation[i]);
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

bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfoNeuralNetworkInputNeuronForSpecialCharacters(const unsigned char POSambiguityInfo, int* POSambiguityInfoNeuralNetworkInputNeuron)
{
	bool specialCharFound = false;

	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_END_OF_SENTENCE)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INPUT_PUNCTUATION_MARK_CHARACTER_END_OF_SENTENCE;
	}
	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_OTHER)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INPUT_PUNCTUATION_MARK_CHARACTER_OTHER;
	}
	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_QUOTATION_CHARACTER)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INPUT_QUOTATION_CHARACTER;
	}
	if(POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_OUT_OF_SENTENCE_BOUNDS)
	{
		specialCharFound = true;
		*POSambiguityInfoNeuralNetworkInputNeuron = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INPUT_OUT_OF_SENTENCE_BOUNDS;
	}
		
	return specialCharFound;
}
#endif

int GIApreprocessorPOStaggerClass::convertGIAPOStaggerValueToGrammaticalWordType(const int POSvalue)
{
	int grammaticalWordType = GIAPOStaggerValueCrossReferenceGrammaticalWordTypeArray[POSvalue];
	return grammaticalWordType;
}
	
bool GIApreprocessorPOStaggerClass::generatePOSambiguityInfoPermutation(vector<GIApreprocessorWord*>* sentenceContentsLRP, int wCentre, bool* identifiedEveryWordInDatabasePOSpermutation, string* POSambiguityInfoPermutationRecord)
{
	bool result = true;
	
	GIApreprocessorWord* centreWord = (*sentenceContentsLRP)[wCentre];

	//context word calculations	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION
	int wMin = SHAREDvars.maxInt(wCentre - (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2), 0);
	int wMax = SHAREDvars.minInt(wCentre + (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2), currentGIApreprocessorSentenceInList->sentenceContentsLRP.size());
	int wSize = wMax - wMin + 1;
	string POSambiguityInfoPermutation(GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION, (unsigned char)(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN));	//GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION = max size of wSize
	int POSambiguityInfoPermutationIndex = (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2) - (wCentre - wMin);	//CHECKTHIS
	#else
	int wMin = wCentre - (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2);
	int wMax = wCentre + (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2);
	int wSize = wMax - wMin;	//GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION
	string POSambiguityInfoPermutation(GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION, (unsigned char)(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN));	//GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION = max size of wSize
	int POSambiguityInfoPermutationIndex = 0;	
	#endif

	for(int w=wMin; w<wMax; w++)
	{	
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION
		if(w == wCentre)	
		{
			POSambiguityInfoPermutation[POSambiguityInfoPermutationIndex] = (unsigned char)(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_CENTRE_WORD);
			POSambiguityInfoPermutationIndex++;
		}
		#else
		if((w < 0) || (w >= sentenceContentsLRP->size()))
		{
			POSambiguityInfoPermutation[POSambiguityInfoPermutationIndex] = (unsigned char)(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_OUT_OF_SENTENCE_BOUNDS);	
			POSambiguityInfoPermutationIndex++;					
		}
		#endif
		else
		{
			GIApreprocessorWord* contextWord = (*sentenceContentsLRP)[w];
			string contextWordLowerCase = SHAREDvars.convertStringToLowerCase(&(contextWord->tagName));	//CHECKTHIS: verify that currentWord->tagName is case sensitive

			GIApreprocessorMultiwordReductionWord* contextWordFound = NULL;
			unsigned char contextWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
			if(!findWordInWordListAllTypesWithPOSambiguityInfo(contextWordLowerCase, &contextWordFound, &contextWordPOSambiguityInfo))
			{
				if(!determinePOSambiguityInfoForSpecialCharacters(contextWord, &contextWordPOSambiguityInfo))
				{
					*identifiedEveryWordInDatabasePOSpermutation = false;
				}
			}

			if(contextWordPOSambiguityInfo > 255)	//error checking
			{
				cerr << "GIApreprocessorPOStaggerClass::generatePOStaggerDatabaseFromWikiDumpText{} error: (contextWordPOSambiguityInfo > 255)" << endl;
				exit(EXIT_ERROR);
			}
			POSambiguityInfoPermutation[POSambiguityInfoPermutationIndex] = (unsigned char)(contextWordPOSambiguityInfo);
			//cout << "POSambiguityInfoPermutationIndex = " << POSambiguityInfoPermutationIndex << endl;
			POSambiguityInfoPermutationIndex++;
		}
	}
	
	*POSambiguityInfoPermutationRecord = POSambiguityInfoPermutation;
	
	return result;
}	

bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfoIsAmbiguous(const unsigned char POSambiguityInfo, int* unambiguousPOSvalue)
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
	if((numberOfPOStypesRecorded == 0) || (numberOfPOStypesRecorded > 1))
	{
		ambiguous = true;
	}
	
	return ambiguous;
}	

bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfoForSpecialCharacters(const GIApreprocessorWord* word, unsigned char* POSambiguityInfo)
{
	bool specialCharFound = false;
	if((word->tagName).length() == 1)
	{
		char wordChar = (word->tagName)[0];
		if(SHAREDvars.charInCharArray(wordChar, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
		{
			specialCharFound = true;
			*POSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_END_OF_SENTENCE;
		}
		if(!SHAREDvars.charInCharArray(wordChar, nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
		{
			specialCharFound = true;
			*POSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_PUNCTUATION_MARK_CHARACTER_OTHER;
		}
		if(!SHAREDvars.charInCharArray(wordChar, nlpQuotationMarkCharacterArray, GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS))
		{
			specialCharFound = true;
			*POSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_QUOTATION_CHARACTER;
		}
	}
	return specialCharFound;
}



bool GIApreprocessorPOStaggerClass::findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, unsigned char* POSambiguityInfoFound)
{	
	bool result = false;
		
	unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>* wordListAllTypesWithPOSambiguityInfo = GIApreprocessorMultiwordReduction.getWordListAllTypesWithPOSambiguityInfo();

	unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>::iterator it;
	it = wordListAllTypesWithPOSambiguityInfo->find(word);
	if(it != wordListAllTypesWithPOSambiguityInfo->end())
	{
		result = true;
		*wordFound = (it->second).first;
		*POSambiguityInfoFound = (it->second).second;
	}
	
	return result;
}	

#endif

string GIApreprocessorPOStaggerClass::convertBoolVectorToString(vector<bool>* inputNeuronExperienceValuesContextWord)
{
	string boolVectorString = "";
	for(int i=0; i<inputNeuronExperienceValuesContextWord->size(); i++)
	{
		boolVectorString = boolVectorString + SHAREDvars.convertIntToString(int(inputNeuronExperienceValuesContextWord->at(i)));
	}
	return boolVectorString;
}




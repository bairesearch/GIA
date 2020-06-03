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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l2a 02-June-2020
 * Requirements: requires plain text file
 * Description: Preprocessor POS tagger
 * /
 *******************************************************************************/


#include "GIApreprocessorPOStagger.hpp"
#include "GIAsynRelTranslatorDefs.hpp"


#ifdef GIA_PREPROCESSOR_POS_TAGGER

/*plan: 
execute createPreprocessSentencesSimple on wiki text

DONE read GIA lrp word lists to write to index list (GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES)

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




#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
bool GIApreprocessorPOStaggerClass::executePrelimFeatureProcessingOnSentences(const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
			
		for(int wCentre=0; wCentre<sentenceContents->size(); wCentre++)
		{
			GIAfeature* currentFeatureInList = new GIAfeature();	//prelim feature

			//centre word calculations
			GIApreprocessorPlainTextWord* centreWord = (*sentenceContents)[wCentre];
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_POS_DEBUG
			cout << "centreWord = " << centreWord->tagName << endl;
			#endif

			bool identifiedEveryWordInDatabasePOSpermutation = true;
			bool identifiedEveryWordInDatabasePOSpermutationIsUnambiguous = true;

			
			uint64_t centreWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
			bool centreWordPOSisAmbiguous = false;
			unsigned char centreWordUnambiguousPOSindex = INT_DEFAULT_VALUE;
			if(!determinePOSambiguityInfo(centreWord, &centreWordPOSambiguityInfo, &centreWordPOSisAmbiguous, &centreWordUnambiguousPOSindex, &identifiedEveryWordInDatabasePOSpermutation))
			{
				result = false;
			}
								
			if(centreWordPOSisAmbiguous)
			{		
				/*
				cout << "\ncentreWordPOSisAmbiguous" << endl;
				cout << "centreWord = " << centreWord->tagName << endl;
				cout << "DBconvertByteToHex(centreWordPOSambiguityInfo) = " << GIApreprocessorPOStaggerDatabase.DBconvertByteToBinaryString(centreWordPOSambiguityInfo) << endl;
				*/
				
				vector<uint64_t> POSambiguityInfoPermutation;
				if(!generatePOSambiguityInfoPermutation(sentenceContents, wCentre, &identifiedEveryWordInDatabasePOSpermutation, &identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, &POSambiguityInfoPermutation))
				{
					result = false;
				}
				
				/*
				//debug:
				for(int i=0; i<POSambiguityInfoPermutation.size(); i++)
				{
					cout << "i = " << i << endl;
					cout << "\tPOSambiguityInfoPermutation.at(i) = " << POSambiguityInfoPermutation.at(i) << endl;
					unsigned char POSpermutationIndexByte = GIApreprocessorPOStaggerDatabase.convertPOSambiguityInfoToIndex(POSambiguityInfoPermutation.at(i));
					cout << "\tint(POSpermutationIndexByte) = " << int(POSpermutationIndexByte) << endl;
				}
				*/

				#ifdef GIA_PREPROCESSOR_POS_TAGGER_ONLY_ADD_DATABASE_ENTRY_IF_POS_AMBIGUITY_INFO_FOUND_FOR_EVERY_CONTEXT_WORD
				if(identifiedEveryWordInDatabasePOSpermutation)
				{
				#endif

					bool foundMatchingCentreWordPOSambiguityInfo = false;
					unsigned char centreWordPOSindexPrediction = INT_DEFAULT_VALUE;
					bool centreWordPOSisAmbiguous = false;
					#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
					double experienceBackPropagationPassError = REALLY_LARGE_DOUBLE;
					#else 
					int maximumNumberOfInstances = 0;
					#endif
					
					//cout << "GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY:" << endl;
	
					#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
					vector<vector<uint64_t>*> POSambiguityInfoUnambiguousPermutationArray;
					vector<uint64_t>* POSambiguityInfoUnambiguousPermutationNew = new vector<uint64_t>(POSambiguityInfoPermutation.size());
					POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
					generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, &POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
					for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray.size(); i++)
					{
						vector<uint64_t>* POSambiguityInfoPermutationTemp = POSambiguityInfoUnambiguousPermutationArray[i];
					#else
						vector<uint64_t>* POSambiguityInfoPermutationTemp = &POSambiguityInfoPermutation;
					#endif

						bool foundMatchingCentreWordPOSambiguityInfoTemp = false;
						unsigned char centreWordPOSindexPredictionTemp = INT_DEFAULT_VALUE;
						bool centreWordPOSisAmbiguousTemp = false;
						//#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
						double experienceBackPropagationPassErrorTemp = 0.0; 
						//#else
						int maximumNumberOfInstancesTemp = 0;
						//#endif
						
						//cout << "permutation = " << GIApreprocessorPOStaggerDatabase.convertPOSambiguityInfoPermutationToPOSambiguityInfoPermutationIndexString(POSambiguityInfoPermutationTemp) << endl;

						predictPOStaggerDatabaseEntry(centreWordPOSambiguityInfo, POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSindex, centreWord, &foundMatchingCentreWordPOSambiguityInfoTemp, &centreWordPOSindexPredictionTemp, &centreWordPOSisAmbiguousTemp, &experienceBackPropagationPassErrorTemp, &maximumNumberOfInstancesTemp);
						if(foundMatchingCentreWordPOSambiguityInfoTemp)
						{
							#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
							if(experienceBackPropagationPassErrorTemp < experienceBackPropagationPassError)
							#else
							if(maximumNumberOfInstancesTemp > maximumNumberOfInstances)
							#endif
							{
								foundMatchingCentreWordPOSambiguityInfo = foundMatchingCentreWordPOSambiguityInfoTemp;
								centreWordPOSindexPrediction = centreWordPOSindexPredictionTemp;
								centreWordPOSisAmbiguous = centreWordPOSisAmbiguousTemp;
								#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
								experienceBackPropagationPassError = experienceBackPropagationPassErrorTemp;
								#else
								maximumNumberOfInstances = maximumNumberOfInstancesTemp;
								#endif
							}
							//cout << "predictPOStaggerDatabaseEntry" << endl;
						}
						else
						{
							//cout << "!predictPOStaggerDatabaseEntry" << endl;
						}

					#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
					}
					#endif


					#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
					currentFeatureInList->grammaticalWordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;
					#else
					if(foundMatchingCentreWordPOSambiguityInfo)
					{
						if(!centreWordPOSisAmbiguous)
						{
							#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
							unsigned char centreWordPOSvalueFirstAmbiguousPrediction = 0;
							//cout << "centreWordPOSindexPrediction = " << int(centreWordPOSindexPrediction) << endl;
							if(GIApreprocessorPOStaggerDatabase.verifyPOStaggerDatabasePredictionAgainstPOSambiguityInfo(centreWordPOSindexPrediction, centreWordPOSambiguityInfo, &centreWordPOSvalueFirstAmbiguousPrediction))
							{
							#endif
								currentFeatureInList->GIAposType = centreWordPOSindexPrediction;
							#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
							}
							else
							{
								currentFeatureInList->GIAposType = centreWordPOSvalueFirstAmbiguousPrediction;

								cerr << "verifyPOStaggerDatabasePredictionAgainstPOSambiguityInfo failed" << endl;
								cerr << "centreWord = " << centreWord->tagName << endl;
								cerr << "wCentre = " << wCentre << endl; 
								cerr << "centreWordPOSindexPrediction =" << centreWordPOSindexPrediction << endl;
								cerr << "centreWord->centreWordPOSambiguityInfo = " << GIApreprocessorPOStaggerDatabase.DBconvertByteToBinaryString(centreWordPOSambiguityInfo) << endl;
							}
							#endif

						}
						else
						{							
							currentFeatureInList->GIAposType = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
							cerr << "GIA_PREPROCESSOR_POS_TAGGER GIAposRelTranslatorHybridClass::executePrelimFeatureProcessingOnSentences{} error: word POS cannot be determined (centreWordPOSisAmbiguous): " << centreWord->tagName << endl;
							exit(EXIT_ERROR);
						}
					}
					else
					{
						currentFeatureInList->GIAposType = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
						cout << "GIA_PREPROCESSOR_POS_TAGGER GIAposRelTranslatorHybridClass::executePrelimFeatureProcessingOnSentences{} warning: word POS cannot be determined (!foundMatchingCentreWordPOSambiguityInfo): " << centreWord->tagName << endl;
					}
					#endif


				#ifdef GIA_PREPROCESSOR_POS_TAGGER_ONLY_ADD_DATABASE_ENTRY_IF_POS_AMBIGUITY_INFO_FOUND_FOR_EVERY_CONTEXT_WORD
				}
				#endif
			}
			else
			{
				currentFeatureInList->GIAposType = centreWordUnambiguousPOSindex;
				//cout << "currentFeatureInList->grammaticalWordType = " << currentFeatureInList->grammaticalWordType << endl;
			}


			(*sentenceContents)[wCentre]->featureReferencePrelim = currentFeatureInList;
		}
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}	
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_PERFORM_BATCH_PREDICTIONS
	//generate experience list
	ANNexperience* firstExperienceInList = NULL;
	ANNexperience* currentExperienceInList = NULL;
	bool foundFirstExperience = false;
	currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
	
		for(int wCentre=0; wCentre<sentenceContents->size(); wCentre++)
		{
			GIApreprocessorPlainTextWord* centreWord = (*sentenceContents)[wCentre];
			
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
			for(int i=0; i<centreWord->POStaggerExperiencePermutations.size(); i++)
			{
				ANNexperience* centreWordPOStaggerExperienceTemp = centreWord->POStaggerExperiencePermutations[i];
			#else
			if(centreWord->POStaggerExperience != NULL)
			{
				ANNexperience* centreWordPOStaggerExperienceTemp = centreWord->POStaggerExperience;
			#endif
				if(!foundFirstExperience)
				{
					foundFirstExperience = true;
					firstExperienceInList = centreWordPOStaggerExperienceTemp;
					currentExperienceInList = centreWordPOStaggerExperienceTemp;
				}
				else
				{
					currentExperienceInList->next = centreWordPOStaggerExperienceTemp;
					currentExperienceInList = currentExperienceInList->next;
				}
			}
		}
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}	
	if(foundFirstExperience)
	{
		//add blank experience at end of list (required)
		currentExperienceInList->next = new ANNexperience();
	}
		
	//generate predictions	
	GIApreprocessorPOStaggerDatabase.externalANNpredict(firstExperienceInList);
	
	//apply predictions	
	currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{		
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
	
		for(int wCentre=0; wCentre<sentenceContents->size(); wCentre++)
		{
			GIApreprocessorPlainTextWord* centreWord = (*sentenceContents)[wCentre];
			GIAfeature* currentFeatureInList = (*sentenceContents)[wCentre]->featureReferencePrelim;
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
			if(centreWord->POStaggerExperiencePermutations.size() > 0)
			{
				double experienceBackPropagationPassError = REALLY_LARGE_DOUBLE;
				ANNexperience* centreWordPOStaggerExperience = NULL;
				for(int i=0; i<centreWord->POStaggerExperiencePermutations.size(); i++)
				{
					ANNexperience* centreWordPOStaggerExperienceTemp2 = centreWord->POStaggerExperiencePermutations[i];
					if(centreWordPOStaggerExperienceTemp2->predictionError < experienceBackPropagationPassError)
					{
						experienceBackPropagationPassError = centreWordPOStaggerExperienceTemp2->predictionError;
						centreWordPOStaggerExperience = centreWordPOStaggerExperienceTemp2;
					}
				}
			#else
			if(centreWord->POStaggerExperience != NULL)
			{
				ANNexperience* centreWordPOStaggerExperience = centreWord->POStaggerExperience;
			#endif
				unsigned char centreWordPOSindexPrediction = centreWordPOStaggerExperience->classTargetValue;
				unsigned char centreWordPOSvalueFirstAmbiguousPrediction = 0;
				#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
				if(GIApreprocessorPOStaggerDatabase.verifyPOStaggerDatabasePredictionAgainstPOSambiguityInfo(centreWordPOSindexPrediction, centreWord->centreWordPOSambiguityInfo, &centreWordPOSvalueFirstAmbiguousPrediction))
				{
				#endif
					currentFeatureInList->GIAposType = centreWordPOSindexPrediction;

					#ifdef GIA_DEBUG_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_PRINT_PREDICTIONS
					cout << "**********************************************************" << endl;
					cout << "GIA POS Tagger neural network prediction:" << endl;
					cout << "centreWord = " << centreWord->tagName << endl;
					cout << "centreWordPOSindexPrediction = " << centreWordPOSindexPrediction << endl;
					cout << "currentFeatureInList->grammaticalWordType = " << currentFeatureInList->grammaticalWordType << endl;
					cout << "grammaticalWordTypeNameArray[currentFeatureInList->grammaticalWordType] = " << grammaticalWordTypeNameArray[currentFeatureInList->grammaticalWordType] << endl;
					cout << "**********************************************************" << endl;
					#endif
				#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
				}
				else
				{
					currentFeatureInList->GIAposType = centreWordPOSvalueFirstAmbiguousPrediction;
					
					cerr << "verifyPOStaggerDatabasePredictionAgainstPOSambiguityInfo failed" << endl;
					cerr << "centreWord = " << centreWord->tagName << endl;
					cerr << "wCentre = " << wCentre << endl; 
					cerr << "centreWordPOSindexPrediction =" << centreWordPOSindexPrediction << endl;
					cerr << "centreWord->centreWordPOSambiguityInfo = " << GIApreprocessorPOStaggerDatabase.DBconvertByteToBinaryString(centreWord->centreWordPOSambiguityInfo) << endl;
				}
				#endif
			}
		}
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	#endif
}

bool GIApreprocessorPOStaggerClass::predictPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, GIApreprocessorPlainTextWord* centreWord, bool* foundMatchingCentreWordPOSambiguityInfo, unsigned char* centreWordPOSindexPrediction, bool* centreWordPOSisAmbiguous, double* experienceBackPropagationPassError, int* maximumNumberOfInstances)
{
	bool result = true;
	
	//NB centreWordUnambiguousPOSindex is not set or used (as it is currently ambiguous; it has not yet been predicted)

	*foundMatchingCentreWordPOSambiguityInfo = false;

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
	ANNexperience* currentExperience = new ANNexperience();
	generateANNexperienceFromPOSambiguityInfoPermutation(POSambiguityInfoPermutation, centreWordUnambiguousPOSindex, currentExperience);
	*experienceBackPropagationPassError = 0.0;
	int idealClassTarget = INT_DEFAULT_VALUE;
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	if(GIApreprocessorPOStaggerDatabase.calculateIdealClassTargetOfInputExperience(currentExperience, &idealClassTarget, &experienceBackPropagationPassError))
	{
		*foundMatchingCentreWordPOSambiguityInfo = true;
		*centreWordPOSindexPrediction = idealClassTarget;
		*centreWordPOSisAmbiguous = false;
		cout << "centreWord = " << centreWord->tagName << ", idealClassTarget = " << idealClassTarget << ", experienceBackPropagationPassError = " << experienceBackPropagationPassError << endl;
	}
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_PERFORM_BATCH_PREDICTIONS
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
	centreWord->POStaggerExperiencePermutations.push_back(currentExperience);
	#else
	centreWord->POStaggerExperience = currentExperience;
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
	centreWord->centreWordPOSambiguityInfo = centreWordPOSambiguityInfo;
	#endif
	*foundMatchingCentreWordPOSambiguityInfo = false;	//currentFeatureInList->grammaticalWordType will be temporarily set to GRAMMATICAL_WORD_TYPE_UNDEFINED (until its predicted value has been generated in the batch external ANN operation)
	#else
	currentExperience->next = new ANNexperience();	//required for externalANNpredict:externalANNgenerateBatchDataExperiences
	if(GIApreprocessorPOStaggerDatabase.externalANNpredict(currentExperience))
	{
		*foundMatchingCentreWordPOSambiguityInfo = true;
		*centreWordPOSindexPrediction = currentExperience->classTargetValue;
		*centreWordPOSisAmbiguous = false;
	}
	#endif
	#endif
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM
	unsigned char entryIndexWithMaximumNumberOfInstances = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;
	vector<string> centreWordPOSambiguityInfoList;
	bool POSpermutationEntryExistent = GIApreprocessorPOStaggerDatabase.DBreadPOSpermutationEstimates(POSambiguityInfoPermutation, &centreWordPOSambiguityInfoList);
	if(POSpermutationEntryExistent)
	{
		*maximumNumberOfInstances = 0;
		for(int i=0; i<centreWordPOSambiguityInfoList.size(); i++)
		{
			string centreWordPOSambiguityReferenceString = centreWordPOSambiguityInfoList[i];
			string centreWordPOSambiguityInfoCurrentByteBaseXstring = centreWordPOSambiguityReferenceString.substr(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_START_POS, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_CODED_LENGTH);
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_AND_MAP_USE_6BIT_INDICES
			char centreWordPOSambiguityInfoCurrentByteBaseXchar = centreWordPOSambiguityInfoCurrentByteBaseXstring[0];
			unsigned char centreWordPOSambiguityInfoCurrentByte = GIApreprocessorPOStaggerDatabase.DBconvertBase64ToByte(centreWordPOSambiguityInfoCurrentByteBaseXchar);			
			#else
			unsigned char centreWordPOSambiguityInfoCurrentByte = GIApreprocessorPOStaggerDatabase.DBconvertHexToByte(centreWordPOSambiguityInfoCurrentByteBaseXstring);
			#endif
			string centreWordPOSambiguityInfoNumberInstancesCurrentIntString = centreWordPOSambiguityReferenceString.substr(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_START_POS, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH);
			int centreWordPOSambiguityInfoNumberInstancesCurrentInt = SHAREDvars.convertStringToInt(centreWordPOSambiguityInfoNumberInstancesCurrentIntString);

			if(centreWordPOSambiguityInfoNumberInstancesCurrentInt > *maximumNumberOfInstances)
			{						
				*foundMatchingCentreWordPOSambiguityInfo = true;
				*maximumNumberOfInstances = centreWordPOSambiguityInfoNumberInstancesCurrentInt;
				entryIndexWithMaximumNumberOfInstances = centreWordPOSambiguityInfoCurrentByte;
			}
		}
	}
	else
	{	
		//currentFeatureInList->grammaticalWordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;	
		//cout << "GIA_PREPROCESSOR_POS_TAGGER GIAposRelTranslatorHybridClass::predictPOStaggerDatabaseEntry{} warning: word POS cannot be determined (!POSpermutationEntryExistent): " << centreWord->tagName << endl;
	}
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
	*centreWordPOSisAmbiguous = false;
	*centreWordPOSindexPrediction = entryIndexWithMaximumNumberOfInstances;
	#else
	*centreWordPOSisAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(entryIndexWithMaximumNumberOfInstances, centreWordPOSindexPrediction, true);
	//NB centreWordPOSisAmbiguous should always be false
	#endif
	
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MAP
	unsigned char entryIndexWithMaximumNumberOfInstances = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;
	*maximumNumberOfInstances = 0;
	multimap<string, pair<uint64_t, int>>* POStaggerMap = getPOStaggerMap();
	pair<multimap<string, pair<unsigned char, int>>::iterator, multimap<string, pair<unsigned char, int>>::iterator> rangeFound = POStaggerMap->equal_range(POSambiguityInfoPermutationString);
	for(multimap<string, pair<unsigned char, int>>::iterator it = rangeFound.first; it != rangeFound.second; it++)
	{
		unsigned char centreWordPOSambiguityInfoCurrent = (it->second).first;
		int numberOfInstances = (it->second).second;
		if(numberOfInstances > *maximumNumberOfInstances)
		{
			*foundMatchingCentreWordPOSambiguityInfo = true;
			*maximumNumberOfInstances = numberOfInstances;
			entryIndexWithMaximumNumberOfInstances = centreWordPOSambiguityInfoCurrent;
		}
	}
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
	*centreWordPOSisAmbiguous = false;
	*centreWordPOSindexPrediction = entryIndexWithMaximumNumberOfInstances;
	#else
	*centreWordPOSisAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(entryIndexWithMaximumNumberOfInstances, centreWordPOSindexPrediction, true);
	//NB centreWordPOSisAmbiguous should always be false
	#endif
	#endif
	
	return result;	
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
			vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);

			for(int w=0; w<sentenceContents->size(); w++)
			{		
				GIApreprocessorPlainTextWord* currentWord = (*sentenceContents)[w];
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
			
	for(int wikiDumpFileBatchIndex=GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_START_FILE; wikiDumpFileBatchIndex<GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NUMBER_OF_FILES; wikiDumpFileBatchIndex++)
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
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR
	GIAtranslatorVariablesClass translatorVariablesNOTUSED;
	#endif
							
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	if(!GIApreprocessorWordIdentification.createWordIndexListFromLRPfiles())
	{
		result = false;
	}
	#else
	cerr << "GIApreprocessorPOStaggerClass::generatePOStaggerDatabaseFromWikiDumpText{} error: !GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES; word index list currently needs to be regenerated everytime POS tagger database generation occurs" << endl;
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
	int64_t numberOfNeuralNetworkFeeds = 0;
	//#endif
	
	//generate POS tagger database entries
	cout << "finished generatePreprocessorSentenceObjectsFromText" << endl;
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
	string XtrainBatchFileName = GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchFileName(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_X_TRAIN_BATCH_FILE_NAME_PARTA, wikiDumpFileBatchIndex);
	#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WITHOUT_TARGET_CLASSES
	string YtrainBatchFileName = GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchFileName(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_Y_TRAIN_BATCH_FILE_NAME_PARTA, wikiDumpFileBatchIndex);
	#endif
	ofstream XtrainBatchFileObject(XtrainBatchFileName.c_str());
	#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WITHOUT_TARGET_CLASSES
	ofstream YtrainBatchFileObject(YtrainBatchFileName.c_str());
	#endif
	#else
	ANNexperience* firstExperienceInList = new ANNexperience();
	ANNexperience* currentExperienceInList = firstExperienceInList;
	#endif
	#endif
	
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInWikiDumpText;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{	
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
		
		/*
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRINT_SENTENCES
		GIApreprocessorSentenceClassObject.printSentence(sentenceContents, false);
		#endif
		*/
		
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SENTENCE_MIN_WORDS
		if(sentenceContents->size() >= GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SENTENCE_MIN_WORDS)
		{
		#endif
			/*
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRINT_SENTENCES
			//cout << "\t";
			GIApreprocessorSentenceClassObject.printSentence(sentenceContents, false);
			#endif
			*/
			
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW
			int wCentre=-1;	//irrelevant (NOTUSED)
			#else
			#ifdef DEBUG_GIA_PREPROCESSOR_POS_TAGGER_DATABASE_CALCULATE_PERCENTAGE_POS_AMBIGUOUS_WORDS
			int nonAmbiguousWordCount = 0;
			#endif
			for(int wCentre=0; wCentre<sentenceContents->size(); wCentre++)
			{
			#endif
				//centre word calculations
				GIApreprocessorPlainTextWord* centreWord = (*sentenceContents)[wCentre];
				#ifdef GIA_PREPROCESSOR_POS_TAGGER_POS_DEBUG
				cout << "centreWord = " << centreWord->tagName << endl;
				#endif

				//cout << "centreWord = " << centreWord->tagName << endl;

				bool identifiedEveryWordInDatabasePOSpermutation = true;
				bool identifiedEveryWordInDatabasePOSpermutationIsUnambiguous = true;

				#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW
				string centreWordLowerCase = SHAREDvars.convertStringToLowerCase(&(centreWord->tagName));	//CHECKTHIS: verify that currentWord->tagName is case sensitive
				GIApreprocessorMultiwordReductionWord* centreWordFound = NULL;
				//bool identifiedCentreWordInDatabasePOSpermutation = true;
				uint64_t centreWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
				if(!findWordInWordListAllTypesWithPOSambiguityInfo(centreWordLowerCase, &centreWordFound, &centreWordPOSambiguityInfo))
				{
					//identifiedCentreWordInDatabasePOSpermutation = false;
					identifiedEveryWordInDatabasePOSpermutation = false;
				}
				#endif

				vector<uint64_t> POSambiguityInfoPermutation;
				if(generatePOSambiguityInfoPermutation(sentenceContents, wCentre, &identifiedEveryWordInDatabasePOSpermutation, &identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, &POSambiguityInfoPermutation))
				{
					//cout << "identifiedEveryWordInDatabasePOSpermutationIsUnambiguous = " << identifiedEveryWordInDatabasePOSpermutationIsUnambiguous << endl;

					#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW
					#ifdef GIA_PREPROCESSOR_POS_TAGGER_ONLY_ADD_DATABASE_ENTRY_IF_POS_AMBIGUITY_INFO_FOUND_FOR_EVERY_CONTEXT_WORD
					if(identifiedEveryWordInDatabasePOSpermutation)
					{
					#endif
						unsigned char centreWordUnambiguousPOSindex = INT_DEFAULT_VALUE;
						bool centreWordAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(centreWordPOSambiguityInfo, &centreWordUnambiguousPOSindex, true);
						#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
						if(identifiedEveryWordInDatabasePOSpermutationIsUnambiguous && !centreWordAmbiguous)
						{
						#endif
							#ifndef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION_EVEN_IF_AMBIGUOUS
							if(!centreWordAmbiguous)
							{
								#ifdef DEBUG_GIA_PREPROCESSOR_POS_TAGGER_DATABASE_CALCULATE_PERCENTAGE_POS_AMBIGUOUS_WORDS
								nonAmbiguousWordCount++;
								#endif
							#endif
					#endif
								#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
								/*
								bool allWordsHaveUnambigiousPOS = true;
								for(int i=0; i<POSambiguityInfoPermutation.size(); i++)
								{
									bool treatWordAsAmbiguousIfNullPOSvalue = true;
									int unambiguousPOSinfoIndexNOTUSED;
									if(GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(POSambiguityInfoPermutation[i], &,unambiguousPOSinfoIndexNOTUSED, treatWordAsAmbiguousIfNullPOSvalue);
									{
										allWordsHaveUnambigiousPOS = false;
									}
								}
								if(allWordsHaveUnambigiousPOS)
								*/
								if(identifiedEveryWordInDatabasePOSpermutationIsUnambiguous)
								{
									setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, &POSambiguityInfoPermutation);

									#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRINT_SENTENCES
									GIApreprocessorSentenceClassObject.printSentence(sentenceContents, false);
									#endif
								#else

								#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRINT_SENTENCES
								GIApreprocessorSentenceClassObject.printSentence(sentenceContents, false);
								#endif

								#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
								vector<vector<uint64_t>*> POSambiguityInfoUnambiguousPermutationArray;
								vector<uint64_t>* POSambiguityInfoUnambiguousPermutationNew = new vector<uint64_t>(POSambiguityInfoPermutation.size());
								POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
								generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, &POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
								for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray.size(); i++)
								{
									vector<uint64_t>* POSambiguityInfoPermutationTemp = POSambiguityInfoUnambiguousPermutationArray[i];
									#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR
									setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
									#endif

								#else
									vector<uint64_t>* POSambiguityInfoPermutationTemp = &POSambiguityInfoPermutation;
									#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR
									(sentenceContents->at(w))->POSambiguityInfo = (*POSambiguityInfoPermutation)[w];
									#endif
								#endif
								#endif

									#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR
									//cout << "GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR: executeTxtRelTranslatorNeuralNetwork" << endl;
									
									vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes = GIAposRelTranslatorRules.getGIAposRelTranslatorRulesGroupTypesGlobal();
									bool createNewConnections = true; 
									vector<GIAposRelTranslatorRulesGroupNeuralNetwork*> firstLayer;
									if(GIAposRelTranslatorSANIPropagateCompactGenerate.executeTxtRelTranslatorNeuralNetwork(&translatorVariablesNOTUSED, GIAposRelTranslatorRulesGroupTypes, sentenceContents, createNewConnections))
									{
										result = true;
									}
									#else
									
									#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WITHOUT_TARGET_CLASSES
									if(!addPOStaggerDatabaseEntry(POSambiguityInfoPermutationTemp, &XtrainBatchFileObject))
									#else
									#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
									#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
									if(!addPOStaggerDatabaseEntry(centreWordPOSambiguityInfo, POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSindex, &XtrainBatchFileObject, &YtrainBatchFileObject))
									#else
									if(!addPOStaggerDatabaseEntry(centreWordPOSambiguityInfo, POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSindex, currentExperienceInList))
									#endif
									#else
									if(!addPOStaggerDatabaseEntry(centreWordPOSambiguityInfo, POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSindex, &numberOfNeuralNetworkFeeds))
									#endif
									#endif
									{
										result = false;
									}
									#endif
								#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ONLY_TRAIN_UNAMBIGUOUS_PERMUTATIONS
								}
								#else
								#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
								}
								#endif
								#endif
					#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW			
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
					#endif
				}
				else
				{
					#ifndef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_DO_NOT_TRAIN_POS_INDEX_OUT_OF_SENTENCE_BOUNDS
					result = false;
					#endif
				}
			#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW
			}
			#ifdef DEBUG_GIA_PREPROCESSOR_POS_TAGGER_DATABASE_CALCULATE_PERCENTAGE_POS_AMBIGUOUS_WORDS
			int ambiguousWordCount = sentenceContents->size() - nonAmbiguousWordCount;
			cout << "ambiguousWordCount\tsentenceContentsSize\t" << ambiguousWordCount << "\t" << sentenceContents->size() << endl;
			#endif
			#endif

			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MEMORY_FREE_DELETE_PREPROCESSOR_SENTENCES_ONCE_PROCESSED
			GIApreprocessorSentence* previousGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList;
			#endif		
			currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MEMORY_FREE_DELETE_PREPROCESSOR_SENTENCES_ONCE_PROCESSED
			previousGIApreprocessorSentenceInList->next = NULL;	//prevents future sentences from being deleted
			delete previousGIApreprocessorSentenceInList;
			#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SENTENCE_MIN_WORDS
		}
		else
		{
			currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
		}
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
	#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WITHOUT_TARGET_CLASSES
	YtrainBatchFileObject.close();
	#endif
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


#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WITHOUT_TARGET_CLASSES

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
bool GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry(vector<uint64_t>* POSambiguityInfoPermutation, ofstream* XtrainBatchFileObject)
{
	bool result = true;
	
	ANNexperience* currentExperienceInList = new ANNexperience();
	unsigned char centreWordUnambiguousPOSindexUNUSED = 0; 
	generateANNexperienceFromPOSambiguityInfoPermutation(POSambiguityInfoPermutation, centreWordUnambiguousPOSindexUNUSED, currentExperienceInList);
	string experienceInputString = GIApreprocessorPOStaggerDatabase.externalANNgenerateBatchDataExperienceInput(currentExperienceInList);
	*XtrainBatchFileObject << experienceInputString << endl;
	delete currentExperienceInList;

	return result;	
}
#endif
#endif
#endif

#else

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
bool GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ofstream* XtrainBatchFileObject, ofstream* YtrainBatchFileObject)
#else
bool GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ANNexperience** currentExperienceInList)
#endif
#else
bool GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, int64_t* numberOfNeuralNetworkFeeds)
#endif
{
	bool result = true;
	//cout << "addPOStaggerDatabaseEntry" << endl;
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	ANNexperience* currentExperienceInList = new ANNexperience();
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET
	uint64_t centreWordUnambiguousPOSindexTemp = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET_TRAIN_EXPERIENCE_OUTPUT_VALUE_NOT_USED;
	#else
	uint64_t centreWordUnambiguousPOSindexTemp = centreWordUnambiguousPOSindex; 
	#endif
	generateANNexperienceFromPOSambiguityInfoPermutation(POSambiguityInfoPermutation, centreWordUnambiguousPOSindexTemp, currentExperienceInList);
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
	generateANNexperienceFromPOSambiguityInfoPermutation(POSambiguityInfoPermutation, centreWordUnambiguousPOSindex, currentExperienceInList);
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
		uint64_t val = (*POSambiguityInfoPermutation)[i]; 
		//cout << "val = " << val << endl;
	}
	//cout << "POSambiguityInfoPermutationString = " << POSambiguityInfoPermutationString << endl;
	//cout << "centreWordPOSambiguityInfoString = " << centreWordPOSambiguityInfoString << endl;
	if(!GIApreprocessorPOStaggerDatabase.DBwritePOSpermutationEstimate(POSambiguityInfoPermutation, centreWordPOSambiguityInfo))
	{
		result = false;
	}
	#elif defined GIA_PREPROCESSOR_POS_TAGGER_DATABASE_MAP
	int numberOfInstances = 0;
	bool incrementIfFound = true;
	if(!GIApreprocessorPOStaggerDatabase.findInstancePOStaggerMap(POSambiguityInfoPermutation, centreWordPOSambiguityInfo, &numberOfInstances, incrementIfFound))
	{
		numberOfInstances = 1;
		GIApreprocessorPOStaggerDatabase.insertInstanceInPOStaggerMap(POSambiguityInfoPermutation, centreWordPOSambiguityInfo, numberOfInstances);
	}	
	#endif
	
	return result;	
}
#endif

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
	//code replicated from GIApreprocessorClass::createPreprocessSentencesForGIA
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
		#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
		bool isQuery = false;

		string outputLRPTextPlainTXTFileName = GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WIKIDUMP_MULTIWORD_FILE_NAME;
		string outputLRPTextForNLPonlyPlainTXTFileNameBase = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_OUTPUT_FOR_NLP_ONLY_FILE_EXTENSION;

		string outputLRPTextPlainTXTFileNameWikiDumpMultiword = string(outputLRPTextPlainTXTFileName) + GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WIKIDUMP_MULTIWORD_FILE_EXTENSION;
		string outputLRPTextForNLPonlyPlainTXTFileNameWikiDumpMultiword = string(outputLRPTextForNLPonlyPlainTXTFileNameBase) + GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WIKIDUMP_MULTIWORD_FILE_EXTENSION;
		GIApreprocessorWordReduction.initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);
		GIApreprocessorWordReduction.setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	//required for local variable access
		if(!GIApreprocessorWordReduction.parseTextFileAndReduceLanguage(firstGIApreprocessorSentenceInText, outputLRPTextPlainTXTFileNameWikiDumpMultiword, outputLRPTextForNLPonlyPlainTXTFileNameWikiDumpMultiword))
		{
			result = false;
		}
		GIApreprocessorWordReduction.deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	//required so as not to intefere with GIA preprocessor
		#endif
	}
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR
	//code replicated from GIAposRelTranslatorClass::parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_DEFINE_WORD_TRANSLATOR_SENTENCE_ENTITY_INDEX_AT_START
	currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInText;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
		for(int w=0; w<sentenceContents->size(); w++)
		{	
			GIApreprocessorPlainTextWord* currentWord = sentenceContents->at(w);
			currentWord->translatorSentenceEntityIndex = GIAtranslatorOperations.convertSentenceContentsIndexToEntityIndex(w);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI
			currentWord->translatorSentenceWordIndex = w;
			#endif
		}
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	#endif
	#endif
	
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
			#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
			if(!GIApreprocessorWordClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
			{
			#endif
				punctuationMarkFound = true;
			#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
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
		if(!SHAREDvars.charInCharArray(currentToken, preprocessorLowerOrUpperCaseLettersArray, GIA_PREPROCESSOR_WORD_LOWER_OR_UPPER_CASE_LETTERS_NUMBER_OF_TYPES))
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
			{//do not add empty tag after closing quotation marks	//e.g. GIA_PREPROCESSOR_WORD_REDUCE_QUOTES_TO_SINGLE_WORDS or (newlineFound && interpretNewLinesAsNewSentences && previousCharacter==whitespace)
				lastWordBlank = false;
				GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, lastCharacterIndexOfLastWordBeingFilled);
			}
			
			if(punctuationMarkFound)
			{
				string punctuationMark = ""; 
				punctuationMark = punctuationMark + currentToken;
				GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &punctuationMark, &entityIndex, charCount);

				if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
				{
					#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
					if(!GIApreprocessorWordClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
					{
					#endif
						endOfSentencePunctuationMarkFound = true;
					#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
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
					GIApreprocessorWordClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
					currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
					currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
					currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
					currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
					firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
					currentWordInSentence = firstWordInSentence;
					sentenceContentsOriginalText = "";
					entityIndex = GIA_NLP_START_ENTITY_INDEX;
					sentenceIndex++;
				}
			}
			else if(newlineFound && interpretNewLinesAsNewSentences)
			{
				#ifdef GIA_PREPROCESSOR_DISALLOW_EMPTY_SENTENCE_OBJECTS
				if(firstWordInSentence->nextTag != NULL)
				{
				#endif
					GIApreprocessorWordClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
					currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
					currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
					currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
					currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
					firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
					currentWordInSentence = firstWordInSentence;
					sentenceContentsOriginalText = "";
					entityIndex = GIA_NLP_START_ENTITY_INDEX;
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
bool GIApreprocessorPOStaggerClass::generateANNexperienceFromPOSambiguityInfoPermutation(vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ANNexperience* currentExperience)
{
	return generateANNexperienceFromPOSambiguityInfoPermutationStandalone(POSambiguityInfoPermutation, centreWordUnambiguousPOSindex, currentExperience);
}

#endif

/*
int GIApreprocessorPOStaggerClass::convertGIAPOStaggerValueToGrammaticalWordType(const int POSvalue)
{
	int grammaticalWordType = GIAPOStaggerValueCrossReferenceGrammaticalWordTypeArray[POSvalue];
	return grammaticalWordType;
}
*/
	
bool GIApreprocessorPOStaggerClass::generatePOSambiguityInfoPermutation(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int wCentre, bool* identifiedEveryWordInDatabasePOSpermutation, bool* identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, vector<uint64_t>* POSambiguityInfoPermutation)
{
	bool result = true;
	
	GIApreprocessorPlainTextWord* centreWord = (*sentenceContents)[wCentre];

	//context word calculations
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_EXCLUDE_POS_INDEX_OUT_OF_SENTENCE_BOUNDS
	int wMin = 0;
	int wMax = sentenceContents->size()-1;
	int wSize = wMax - wMin;
	POSambiguityInfoPermutation->resize(sentenceContents->size(), GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN);	//GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION = max size of wSize	
	#else	
	int wMin = 0;
	int wMax = GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW_SENTENCE_MAX_NUM_WORDS;
	int wSize = wMax - wMin;	//GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION
	POSambiguityInfoPermutation->resize(GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_RAW_SENTENCE_MAX_NUM_WORDS, GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN);	//GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION = max size of wSize
	#endif
	#else
	int wMin = wCentre - (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2);
	int wMax = wCentre + (GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION/2);
	int wSize = wMax - wMin;	//GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION
	POSambiguityInfoPermutation->resize(GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION, GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN);	//GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION = max size of wSize
	#endif
	int POSambiguityInfoPermutationIndex = 0;	

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_DO_NOT_TRAIN_POS_INDEX_OUT_OF_SENTENCE_BOUNDS
	bool sequenceContainsPosIndexOutOfSentenceBounds = false;
	if((wMin < 0) || (wMax >= sentenceContents->size()))
	{
		sequenceContainsPosIndexOutOfSentenceBounds = true;
		result = false;
	}
	if(!sequenceContainsPosIndexOutOfSentenceBounds)
	{
	#endif
		for(int w=wMin; w<=wMax; w++)
		{	
			/*
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_INCLUDE_CENTRE_WORD_IN_POS_PERMUTATION_OLD
			if(w == wCentre)	
			{
				(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INPUT_CENTRE_WORD_NOTUSED, true);
				POSambiguityInfoPermutationIndex++;	//?
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
			if((w < 0) || (w >= sentenceContents->size()))
			{
				(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_INDEX_OUT_OF_SENTENCE_BOUNDS, true);	
				POSambiguityInfoPermutationIndex++;					
			}
			else
			{
				GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);

				bool contextWordPOSisAmbiguous = false;
				uint64_t contextWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
				unsigned char contextWordUnambiguousPOSindex = INT_DEFAULT_VALUE;	//GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
				if(!determinePOSambiguityInfo(contextWord, &contextWordPOSambiguityInfo, &contextWordPOSisAmbiguous, &contextWordUnambiguousPOSindex, identifiedEveryWordInDatabasePOSpermutation))
				{
					result = false;
				}
				if(contextWordPOSisAmbiguous)
				{
					*identifiedEveryWordInDatabasePOSpermutationIsUnambiguous = false;
				}

				(*POSambiguityInfoPermutation)[POSambiguityInfoPermutationIndex] = contextWordPOSambiguityInfo;
				/*
				cout << "\t\tcontextWordLowerCase = " << contextWordLowerCase << endl;
				cout << "\t\tPOSambiguityInfoPermutationIndex = " << POSambiguityInfoPermutationIndex << endl;
				cout << "\t\tcontextWordPOSambiguityInfo = " << contextWordPOSambiguityInfo <<endl;
				*/
				POSambiguityInfoPermutationIndex++;
			}
		}
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_DO_NOT_TRAIN_POS_INDEX_OUT_OF_SENTENCE_BOUNDS
	}
	#endif
		
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




#endif

#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES

void GIApreprocessorPOStaggerClass::generatePOSambiguityInfoUnambiguousPermutationArray(vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, vector<uint64_t>* POSambiguityInfoPermutation, vector<uint64_t>* POSambiguityInfoUnambiguousPermutationLocal, int wordIndex)
{
	if(wordIndex < POSambiguityInfoPermutation->size())	//OLD: (wordIndex < GIA_PREPROCESSOR_POS_TAGGER_MAX_WORDS_IN_DATABASE_POS_PERMUTATION)
	{
		uint64_t POSambiguityInfo = (*POSambiguityInfoPermutation)[wordIndex];
		int64_t maxValForPOSambiguousEntry = int64_t(1U) << GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES;
		//cout << "maxValForPOSambiguousEntry = " << maxValForPOSambiguousEntry << endl;
		//cout << "POSambiguityInfo = " << POSambiguityInfo << endl;
				
		if((POSambiguityInfo > 0) && (POSambiguityInfo < maxValForPOSambiguousEntry))
		{
			//remove POSambiguityInfoUnambiguousPermutationLocal from POSambiguityInfoUnambiguousPermutationArray as it will be replicated
			vector<vector<uint64_t>*>::iterator it = find(POSambiguityInfoUnambiguousPermutationArray->begin(), POSambiguityInfoUnambiguousPermutationArray->end(), POSambiguityInfoUnambiguousPermutationLocal);
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
			
			for(int x=0; x<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; x++)
			{
				if(SHAREDvars.getBitValue(POSambiguityInfo, x))
				{
					//cout << "x = " << x << endl;
					vector<uint64_t>* POSambiguityInfoUnambiguousPermutationNew = new vector<uint64_t>(*POSambiguityInfoUnambiguousPermutationLocal);
					POSambiguityInfoUnambiguousPermutationArray->push_back(POSambiguityInfoUnambiguousPermutationNew);
					(*POSambiguityInfoUnambiguousPermutationNew)[wordIndex] = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, x, true);	//create a new unambigious hypothetical POS value
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

#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfoWrapper(vector<GIApreprocessorPlainTextWord*>* sentenceContents, vector<uint64_t>* POSambiguityInfoPermutation, vector<string>* explicitWordList)
{
	bool result = true;
	
	//resetting of word->alreadyFoundMatch is required in case a higher level app (e.g. NLC) shares word objects between sentences:
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = sentenceContents->at(w);
		currentWord->alreadyFoundMatch = false;
	}

	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);

		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		string wordTextLowerCase = SHAREDvars.convertStringToLowerCase(&(contextWord->tagName));
		GIApreprocessorMultiwordReductionWord* nounBaseFound = NULL;
		int nounGrammaticalBaseTenseForm = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
		if(GIApreprocessorWordIdentification.determineNounPluralVariant(wordTextLowerCase, &nounBaseFound, &nounGrammaticalBaseTenseForm))
		{
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
			contextWord->wordNounVariantGrammaticalTenseForm = nounGrammaticalBaseTenseForm;	//will only be valid if wordPOStype == noun
			#else
			contextWord->wordNounVariantGrammaticalTenseForm = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_PLURAL;
			#endif
			//cout << "contextWord->wordNounVariantGrammaticalTenseForm == " << contextWord->wordNounVariantGrammaticalTenseForm << ", contextWord = " << contextWord->tagName << endl;
		}
		string verbBaseNameFound = "";
		int verbGrammaticalBaseTenseForm = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
		if(GIApreprocessorWordIdentification.determineVerbCaseStandardWithAdditional(wordTextLowerCase, &verbBaseNameFound, &verbGrammaticalBaseTenseForm))
		{
			contextWord->wordVerbVariantGrammaticalTenseForm = verbGrammaticalBaseTenseForm;	//will only be valid if wordPOStype == verb
			//cout << "contextWord->wordVerbVariantGrammaticalTenseForm == " << contextWord->verbGrammaticalBaseTenseForm << ", contextWord = " << contextWord->tagName << endl;
		}			
		#endif

		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		cout << "contextWord = " << contextWord->tagName << endl;
		#endif
		bool contextWordPOSisAmbiguous = false;
		bool identifiedEveryWordInDatabasePOSpermutationNOTUSED = true;
		uint64_t contextWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
		unsigned char contextWordUnambiguousPOSindex = INT_DEFAULT_VALUE;	//GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
		if(!determinePOSambiguityInfo(contextWord, &contextWordPOSambiguityInfo, &contextWordPOSisAmbiguous, &contextWordUnambiguousPOSindex, &identifiedEveryWordInDatabasePOSpermutationNOTUSED))
		{
			result = false;
		}

		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
		vector<string>::iterator explicitWordListIterator = find(explicitWordList->begin(), explicitWordList->end(), wordTextLowerCase);
		if(explicitWordListIterator != explicitWordList->end())
		{
			//cout << "found explicit word; wordTextLowerCase = " << wordTextLowerCase << endl;
			//add explicit case to contextWordPOSambiguityInfo
			contextWordPOSambiguityInfo = SHAREDvars.setBitValue(contextWordPOSambiguityInfo, GIA_PREPROCESSOR_POS_TYPE_EXPLICITWORDTEMP, true);	//set as explicitWordTemp
		}
		#endif
  
		POSambiguityInfoPermutation->push_back(contextWordPOSambiguityInfo);
	}
	
	return result;
}
#endif

bool GIApreprocessorPOStaggerClass::determinePOSambiguityInfo(GIApreprocessorPlainTextWord* contextWord, uint64_t* contextWordPOSambiguityInfo, bool* contextWordPOSisAmbiguous, unsigned char* contextWordUnambiguousPOSindex, bool* identifiedEveryWordInDatabasePOSpermutation)
{
	bool result = true;
	
	int entityIndex = (static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(contextWord))->entityIndex;
	//cout << "entityIndex = " << entityIndex << endl;
	string wordText = contextWord->tagName;
	
	bool foundWordInLists = false;
	GIApreprocessorMultiwordReductionWord* contextWordFound = NULL;
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES_SUPPORT_UPPERCASE_PROPERNOUN_WORD_LISTS
	//CHECKTHIS: verify that currentWord->tagName is case sensitive
	//cout << "wordText = " << wordText << endl;
	if(findWordInWordListAllTypesWithPOSambiguityInfo(wordText, &contextWordFound, contextWordPOSambiguityInfo))
	{
		//cout << "1 wordText = " << wordText << endl;
		foundWordInLists = true;
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD1
	if((!foundWordInLists) && GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(contextWord))
	{
		foundWordInLists = true;
		*contextWordPOSambiguityInfo = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT, true);	//set as propernoun
	}
	else
	{
	#endif
		#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES_SUPPORT_UPPERCASE_PROPERNOUN_WORD_LISTS_IF_FIRST_WORD_OF_SENTENCE_IS_IN_PROPERNOUN_LIST_THEN_CHECK_OTHER_LISTS_ALSO
		if((entityIndex == GIA_NLP_START_ENTITY_INDEX) || (!foundWordInLists))
		{
			if(GIApreprocessorWordClassObject.wordIsUpperCase(wordText))
			{
				string contextWordLowerCase = SHAREDvars.convertStringToLowerCase(&(wordText));
				uint64_t contextWordLowerCasePOSambiguityInfo = 0;
				//cout << "contextWordLowerCase = " << contextWordLowerCase << endl;
				if(findWordInWordListAllTypesWithPOSambiguityInfo(contextWordLowerCase, &contextWordFound, &contextWordLowerCasePOSambiguityInfo))
				{
					if((entityIndex == GIA_NLP_START_ENTITY_INDEX) && foundWordInLists)
					{
						//cout << "2 wordText = " << wordText << endl;
						//cout << "*contextWordPOSambiguityInfo = " << *contextWordPOSambiguityInfo << endl;
						//cout << "contextWordLowerCasePOSambiguityInfo = " << contextWordLowerCasePOSambiguityInfo << endl;
						*contextWordPOSambiguityInfo = (*contextWordPOSambiguityInfo | contextWordLowerCasePOSambiguityInfo);	//combine the potential pos types for upper case version and lower case version of word
					}
					else
					{
						*contextWordPOSambiguityInfo = contextWordLowerCasePOSambiguityInfo;
					}
					foundWordInLists = true;
				}
			}
		}
		#else
		if(!foundWordInLists)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD1
			if(midSentenceUppercaseWordSetToProperNoun(wordText, contextWordPOSambiguityInfo))
			{
				foundWordInLists = true;
			}
			else
			{
			#endif
				string contextWordLowerCase = SHAREDvars.convertStringToLowerCase(&(wordText));
				//cout << "contextWordLowerCase = " << contextWordLowerCase << endl;
				if(findWordInWordListAllTypesWithPOSambiguityInfo(contextWordLowerCase, &contextWordFound, contextWordPOSambiguityInfo))
				{
					foundWordInLists = true;
				}
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD1
			}
			#endif
		}
		#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD1
	}
	#endif
	#else
	string contextWordLowerCase = SHAREDvars.convertStringToLowerCase(&(wordText));
	if(findWordInWordListAllTypesWithPOSambiguityInfo(contextWordLowerCase, &contextWordFound, contextWordPOSambiguityInfo))
	{	
		foundWordInLists = true;
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD1
	if((!foundWordInLists) && GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(contextWord))
	{
		foundWordInLists = true;
		*contextWordPOSambiguityInfo = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT, true);	//set as propernoun
	}
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	GIApreprocessorMultiwordReductionPlainTextWord* contextWordMultiwordReductionPlainTextWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(contextWord);
	if(contextWordMultiwordReductionPlainTextWord->collapsedMultiwordWord || contextWordMultiwordReductionPlainTextWord->collapsedPhrasalVerbExactDefinedSection)
	{
		*contextWordUnambiguousPOSindex = contextWordMultiwordReductionPlainTextWord->collapsedMultiwordWordType;
		*contextWordPOSambiguityInfo = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, *contextWordUnambiguousPOSindex, true);
		*contextWordPOSisAmbiguous = false;
		foundWordInLists = true;
	}
	#endif

	if(!foundWordInLists)
	{
		/*
		cout << "!foundWordInLists" << endl;
		cout << "wordText = " << wordText << endl;
		exit(0);
		*/
		
		*identifiedEveryWordInDatabasePOSpermutation = false;
		*contextWordPOSisAmbiguous = true;
		
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBERS
		if(GIApreprocessorWordClassObject.isStringNumber(contextWordMultiwordReductionPlainTextWord->tagName))
		{
			//cout << "isStringNumber" << endl;
			*contextWordUnambiguousPOSindex = GIA_PREPROCESSOR_POS_TYPE_NUMBER;
			*contextWordPOSambiguityInfo = SHAREDvars.setBitValue(GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, *contextWordUnambiguousPOSindex, true);
			*contextWordPOSisAmbiguous = false;	
		}
		#endif
	}
	else
	{
		*contextWordPOSisAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(*contextWordPOSambiguityInfo, contextWordUnambiguousPOSindex, false);
	}
	
	return result;
}

		
bool GIApreprocessorPOStaggerClass::findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, uint64_t* POSambiguityInfoFound)
{	
	bool result = false;
		
	unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, uint64_t>>* wordListAllTypesWithPOSambiguityInfo = GIApreprocessorWordIdentification.getWordListAllTypesWithPOSambiguityInfo();

	unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, uint64_t>>::iterator it;
	it = wordListAllTypesWithPOSambiguityInfo->find(word);
	if(it != wordListAllTypesWithPOSambiguityInfo->end())
	{
		result = true;
		*wordFound = (it->second).first;
		*POSambiguityInfoFound = (it->second).second;
	}
	
	return result;
}	


bool GIApreprocessorPOStaggerClass::printPOSambiguityInfoPermutation(vector<uint64_t>* POSambiguityInfoPermutation)
{
	bool result = true;
	
	for(int i=0; i<POSambiguityInfoPermutation->size(); i++)
	{
		uint64_t POSambiguityInfo = (*POSambiguityInfoPermutation)[i];	
		printPOSambiguityInfo(POSambiguityInfo);
	}
	
	return result;
}

bool GIApreprocessorPOStaggerClass::printPOSambiguityInfo(uint64_t POSambiguityInfo)
{
	bool result = true;

	for(int x=0; x<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; x++)
	{
		if(SHAREDvars.getBitValue(POSambiguityInfo, x))
		{
			cout << "1";
		}
		else
		{
			cout << "0";
		}
	}		
	cout << "\n" <<endl;
	
	return result;
}


bool GIApreprocessorPOStaggerClass::printPOSambiguityInfoPermutationAssumeUnambiguous(vector<uint64_t>* POSambiguityInfoPermutation)
{
	bool result = true;
	
	for(int i=0; i<POSambiguityInfoPermutation->size(); i++)
	{
		uint64_t POSambiguityInfo = (*POSambiguityInfoPermutation)[i];	
		unsigned char unambiguousPOSinfoIndex = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
		bool wordAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(POSambiguityInfoPermutation->at(i), &unambiguousPOSinfoIndex, false);
		cout << "GIApreprocessorPOStypeNameArray[" << i << "] = " << GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] << endl;
	}
	
	return result;
}



#endif


#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
bool GIApreprocessorPOStaggerClass::generateANNexperienceFromPOSambiguityInfoPermutationStandalone(vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char outputNeuronExperienceValue, ANNexperience* currentExperience)
{
	bool result = true;
	
	//cout << "POSambiguityInfoPermutation->size() = " << POSambiguityInfoPermutation->size() << endl;
	
	int numberOfInputNeurons = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD*POSambiguityInfoPermutation->size();	//eg *GIA_PREPROCESSOR_POS_TAGGER_MAX_CONTEXT_WORDS_IN_DATABASE_POS_PERMUTATION
	
	vector<bool> inputNeuronExperienceValues(numberOfInputNeurons, false);
	for(int i=0; i<POSambiguityInfoPermutation->size(); i++)
	{
		int POSambiguityInfoNeuralNetworkInputNeuron = INT_DEFAULT_VALUE;
		vector<bool> inputNeuronExperienceValuesContextWord(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD, false);	

		int numberOfPOStypesToCheck = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS_PER_CONTEXT_WORD;
		for(int POStype = 0; POStype<numberOfPOStypesToCheck; POStype++)
		{
			bool bitValue = SHAREDvars.getBitValue((*POSambiguityInfoPermutation)[i], POStype);
			inputNeuronExperienceValuesContextWord[POStype] = bitValue;
		}

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
	
	//only feed neural network with experience if centre word POS is not ambiguous
	//now create an experience and train the network with the experience
	currentExperience->classTargetValue = outputNeuronExperienceValue;
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
	for(int i=0; i<numberOfInputNeurons; i++)
	{
		currentExperienceInput->inputValue = inputNeuronExperienceValues[i];
		currentExperienceInput->next = new ANNexperienceInput();
		currentExperienceInput = currentExperienceInput->next;
	}
	
	return result;
}
#endif

#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
bool GIApreprocessorPOStaggerClass::setSentenceContentsWordsUnambiguousPOSindex(vector<GIApreprocessorPlainTextWord*>* sentenceContents, vector<uint64_t>* POSambiguityInfoPermutationTemp)
{
	bool result = true;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_SENTENCES
	cout << "sentencePOS = ";
	#endif
	
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		unsigned char unambiguousPOSinfoIndex = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
		bool wordAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(POSambiguityInfoPermutationTemp->at(w), &unambiguousPOSinfoIndex, false);
		contextWord->unambiguousPOSindex = unambiguousPOSinfoIndex;
		//#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
		cout << "unambiguousPOSinfoIndex = " << int(unambiguousPOSinfoIndex) << endl;
		cout << "GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] = " << GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] << endl;
		#endif
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_SENTENCES
		cout << GIApreprocessorPOStypeCrossReferenceStanfordPos[unambiguousPOSinfoIndex] << " ";	//int(unambiguousPOSinfoIndex)
		#endif
	}
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_SENTENCES
	cout << endl;
	#endif


	return result;
}
#endif




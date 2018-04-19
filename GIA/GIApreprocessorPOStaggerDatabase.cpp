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



static string GIAposTaggerDatabaseFolderName;
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
ANNneuron* firstInputNeuronInNetworkGIAposTagger;
ANNneuron* firstOutputNeuronInNetworkGIAposTagger;
long numberOfInputNeuronsGIAposTagger;
long numberOfOutputNeuronsGIAposTagger;
long numberOfLayersGIAposTagger;	//static variable not currently utilised
#endif
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
void GIApreprocessorPOStaggerDatabaseClass::initialisePOStaggerDatabase(const string newGIAposTaggerDatabaseFolderName)
{
	GIAposTaggerDatabaseFolderName = newGIAposTaggerDatabaseFolderName;
	if(!SHAREDvars.directoryExists(&GIAposTaggerDatabaseFolderName))
	{
		cerr << "GIApreprocessorPOStaggerDatabaseClass::initialisePOStaggerDatabase{} error: GIAposTaggerDatabaseFolderName does not exist: " << GIAposTaggerDatabaseFolderName << endl;
		exit(EXIT_ERROR);	
	}
		
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
	//read existing neural network
	firstInputNeuronInNetworkGIAposTagger = new ANNneuron();
	//string neuralNetworkXmlFileName = GIAposTaggerDatabaseFolderName + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_DEFAULT_XML_FILE_NAME;	//alternate method
	string neuralNetworkXmlFileName = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_DEFAULT_XML_FILE_NAME;
	SHAREDvars.setCurrentDirectory(&GIAposTaggerDatabaseFolderName);
	if(SHAREDvars.fileExists(neuralNetworkXmlFileName))
	{
		firstOutputNeuronInNetworkGIAposTagger = ANNxmlConversion.readNetXMLfileAndRecordFormationVariables(neuralNetworkXmlFileName, firstInputNeuronInNetworkGIAposTagger, &numberOfInputNeuronsGIAposTagger, &numberOfOutputNeuronsGIAposTagger, &numberOfLayersGIAposTagger);
		cout << "numberOfInputNeuronsGIAposTagger = " << numberOfInputNeuronsGIAposTagger << endl;
		cout << "numberOfOutputNeuronsGIAposTagger = " << numberOfOutputNeuronsGIAposTagger << endl;
	}
	else
	{
		numberOfInputNeuronsGIAposTagger = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_INPUT_NEURONS;
		numberOfOutputNeuronsGIAposTagger = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_OUTPUT_NEURONS;
		numberOfLayersGIAposTagger = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_LAYERS;
		firstOutputNeuronInNetworkGIAposTagger = ANNformation.formNeuralNetWithOptimisedProperties(firstInputNeuronInNetworkGIAposTagger, numberOfInputNeuronsGIAposTagger, numberOfOutputNeuronsGIAposTagger, numberOfLayersGIAposTagger);
		ANNalgorithmBackpropagationTraining.resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuronInNetworkGIAposTagger);
	}
	#endif
}
#endif

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL
#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
void GIApreprocessorPOStaggerDatabaseClass::feedNeuralNetworkWithExperience(ANNexperience* currentExperience)
{
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET
	ANNalgorithmClassificationNetworkTraining.feedNeuralNetworkWithExperienceClassification(firstInputNeuronInNetworkGIAposTagger, numberOfInputNeuronsGIAposTagger, currentExperience);	
	#else
	ANNalgorithmBackpropagationTraining.feedNeuralNetworkWithExperienceBackpropagation(firstInputNeuronInNetworkGIAposTagger, firstOutputNeuronInNetworkGIAposTagger, numberOfInputNeuronsGIAposTagger, numberOfOutputNeuronsGIAposTagger, currentExperience);
	#endif
}
bool GIApreprocessorPOStaggerDatabaseClass::writeDatabaseNeuralNetwork()
{
	bool result = true;
	
	SHAREDvars.setCurrentDirectory(GIAposTaggerDatabaseFolderName);
	string neuralNetworkXmlFileName = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_DEFAULT_XML_FILE_NAME;
	if(!ANNxmlConversion.writeNetXMLfile(neuralNetworkXmlFileName, firstInputNeuronInNetworkGIAposTagger))
	{
		result = false;
	}
	
	return result;
}
#endif
bool GIApreprocessorPOStaggerDatabaseClass::calculateIdealClassTargetOfInputExperience(ANNexperience* experience, int* idealClassTarget, double* experienceBackPropagationPassError)
{
	bool result = false;
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET
	if(ANNalgorithmClassificationNetworkTraining.calculateIdealClassTargetOfInputExperience(firstInputNeuronInNetworkGIAposTagger, firstOutputNeuronInNetworkGIAposTagger, numberOfInputNeuronsGIAposTagger, numberOfOutputNeuronsGIAposTagger, experience, idealClassTarget, experienceBackPropagationPassError))
	{
		result = true;
	}	
	if(*idealClassTarget >= GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE)
	{
		cerr << "GIApreprocessorPOStaggerDatabaseClass::calculateIdealClassTargetOfInputExperience{} error: (*idealClassTarget >= GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE)" << endl;
		cerr << "classification network has identified the ambiguous word as a punctuation character" << endl;
		result = false;
	}
	#else
	if(ANNalgorithmBackpropagationTraining.calculateIdealClassTargetOfInputExperience(firstInputNeuronInNetworkGIAposTagger, firstOutputNeuronInNetworkGIAposTagger, numberOfInputNeuronsGIAposTagger, numberOfOutputNeuronsGIAposTagger, experience, idealClassTarget, experienceBackPropagationPassError))
	{
		result = true;
	}
	#endif
	
	return result;
}
#endif
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL

#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
bool GIApreprocessorPOStaggerDatabaseClass::externalANNgenerateBatchTrainData(ANNexperience* firstExperienceInList, const int batchIndex)
{
	string XtrainBatchFileName = externalANNgenerateBatchFileName(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_X_TRAIN_BATCH_FILE_NAME_PARTA, batchIndex);
	string YtrainBatchFileName = externalANNgenerateBatchFileName(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_Y_TRAIN_BATCH_FILE_NAME_PARTA, batchIndex);
	
	//generate train batch file
	vector<string> batchTrainDataInput;
	vector<string> batchTrainDataOutput;
	externalANNgenerateBatchDataExperiences(firstExperienceInList, &batchTrainDataInput, &batchTrainDataOutput);
	SHAREDvars.writeStringListToFile(XtrainBatchFileName, &batchTrainDataInput);
	SHAREDvars.writeStringListToFile(YtrainBatchFileName, &batchTrainDataOutput);
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_EXECUTE_FEED_SINGLE_BATCH_ONLY
	externalANNgenerateBatchTrainDataExecuteFeed();
	#endif
}

string GIApreprocessorPOStaggerDatabaseClass::externalANNgenerateBatchFileName(const string fileNamePrepend, const int batchIndex)
{
	SHAREDvars.setCurrentDirectory(GIAposTaggerDatabaseFolderName);	//save output files to output folder

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY
	string XtrainBatchFileName = fileNamePrepend + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_BATCH_FILE_EXTENSION;
	#else
	string formatString = generateIntFormatString(GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_NAME_PART_B_INDEX_NUMBER_OF_CHARACTERS);
	string XtrainBatchFileName = fileNamePrepend + SHAREDvars.convertIntToString(batchIndex, formatString) + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_BATCH_FILE_EXTENSION;
	#endif
	
	return XtrainBatchFileName;
}

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_EXECUTE_FEED
bool GIApreprocessorPOStaggerDatabaseClass::externalANNgenerateBatchTrainDataExecuteFeed()
{
	//immediately train the batch
	externalANNtrainEpochBatch();
}	
bool GIApreprocessorPOStaggerDatabaseClass::externalANNform()
{
	externalANNexecuteScript(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_NAME_FORM);
}
bool GIApreprocessorPOStaggerDatabaseClass::externalANNtrain()
{
	externalANNexecuteScript(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_NAME_TRAIN);
}
bool GIApreprocessorPOStaggerDatabaseClass::externalANNtrainEpoch()
{
	externalANNexecuteScript(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_NAME_TRAIN_EPOCH);
}
bool GIApreprocessorPOStaggerDatabaseClass::externalANNtrainEpochBatch()
{
	externalANNexecuteScript(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_NAME_TRAIN_EPOCH_BATCH);
}
bool GIApreprocessorPOStaggerDatabaseClass::externalANNtest()
{
	externalANNexecuteScript(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_NAME_TEST);
}
#endif

#endif

bool GIApreprocessorPOStaggerDatabaseClass::externalANNpredict(ANNexperience* firstExperienceInList)
{
	bool result = true;
	
	string batchExperiencesString = "";
	
	SHAREDvars.setCurrentDirectory(GIAposTaggerDatabaseFolderName);	//save output files to output folder

	string XpredictBatchFileName = string(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_X_PREDICT_BATCH_FILE_NAME_PREPEND) + (GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_BATCH_FILE_EXTENSION);
	string YpredictBatchFileName = string(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_Y_PREDICT_BATCH_FILE_NAME_PREPEND) + (GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_BATCH_FILE_EXTENSION);

	//generate predictions batch file
	vector<string> batchPredictionsDataInput;
	vector<string> batchPredictionsDataOutputNOTUSED;
	externalANNgenerateBatchDataExperiences(firstExperienceInList, &batchPredictionsDataInput, &batchPredictionsDataOutputNOTUSED);
	SHAREDvars.writeStringListToFile(XpredictBatchFileName, &batchPredictionsDataInput);
	
	//generate predictions
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_PREDICT_RETURN_ERRORS
	string externalANNscriptName = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_NAME_PREDICT_RETURN_ERRORS;	//will become mandatory in the future
	#else
	string externalANNscriptName = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_NAME_PREDICT;	//to be depreciated
	#endif
	if(!externalANNexecuteScript(externalANNscriptName))
	{
		result = false;
	}
	
	//read predictions batch file
	vector<string> YpredictBatchFileContents;
	int numberOfLinesInFile = 0;	//verify int is sufficient (else require long)
	if(!SHAREDvars.getLinesFromFile(YpredictBatchFileName, &YpredictBatchFileContents, &numberOfLinesInFile))
	{
		result = false;
	}
	ANNexperience* currentExperienceInList = firstExperienceInList;
	for(int i=0; i<YpredictBatchFileContents.size(); i++)
	{
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_PREDICT_RETURN_ERRORS
		int positionOfDelimiter = YpredictBatchFileContents[i].find(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_BATCH_FILE_DELIMITER);
		string classTargetValueString = "";
		string predictionErrorString = "";
		if(positionOfDelimiter != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			classTargetValueString = YpredictBatchFileContents[i].substr(0, positionOfDelimiter);
			predictionErrorString = YpredictBatchFileContents[i].substr(positionOfDelimiter+1);
		}
		else
		{
			cerr << "GIApreprocessorPOStaggerDatabaseClass::externalANNpredict{} error: (positionOfDelimiter == CPP_STRING_FIND_RESULT_FAIL_VALUE)" << endl;
			exit(EXIT_ERROR);		
		}
		#else
		string classTargetValueString = YpredictBatchFileContents[i];
		#endif
		
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_Y_PREDICT_HOT_ENCODED
		cerr << "GIApreprocessorPOStaggerDatabaseClass::externalANNpredict{} error: GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_Y_PREDICT_HOT_ENCODED not coded" << endl;
		exit(EXIT_ERROR);
		#else
		long classTargetValue = long(SHAREDvars.convertStringToDouble(classTargetValueString));	//assume that YpredictBatchFileContents is not hot encoded
		cout << "GIApreprocessorPOStaggerDatabaseClass::externalANNgeneratePredictions{}: classTargetValue = " << classTargetValue << endl;
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_PREDICT_RETURN_ERRORS
		double predictionError = SHAREDvars.convertStringToDouble(predictionErrorString);
		cout << "GIApreprocessorPOStaggerDatabaseClass::externalANNgeneratePredictions{}: predictionError = " << predictionError << endl;
		#endif
		#endif
		
		currentExperienceInList->classTargetValue = classTargetValue;
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_PREDICT_RETURN_ERRORS
		currentExperienceInList->predictionError = predictionError;
		#endif
		currentExperienceInList = currentExperienceInList->next;
	}

	return result;
}
bool GIApreprocessorPOStaggerDatabaseClass::externalANNgenerateBatchDataExperiences(ANNexperience* firstExperienceInList, vector<string>* batchDataInput, vector<string>* batchDataOutput)
{
	bool result = true;
	
	//generate predictions batch file
	ANNexperience* currentExperienceInList = firstExperienceInList;
	while(currentExperienceInList->next != NULL)
	{
		string experienceInputString = externalANNgenerateBatchDataExperienceInput(currentExperienceInList);
		batchDataInput->push_back(experienceInputString);
		string experienceOutputString = externalANNgenerateBatchDataExperienceInput(currentExperienceInList);
		batchDataOutput->push_back(experienceOutputString);
		
		currentExperienceInList = currentExperienceInList->next;
	}
}

string GIApreprocessorPOStaggerDatabaseClass::externalANNgenerateBatchDataExperienceInput(ANNexperience* currentExperienceInList)
{	
	string experienceInputString = "";
	ANNexperienceInput* currentExperienceInput = currentExperienceInList->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		string format = "%g";	//"%0.0f";
		experienceInputString = experienceInputString + SHAREDvars.convertDoubleToString(currentExperienceInput->inputValue, format) + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_BATCH_FILE_DELIMITER;
		currentExperienceInput = currentExperienceInput->next;
	}
	return experienceInputString;
}

string GIApreprocessorPOStaggerDatabaseClass::externalANNgenerateBatchDataExperienceOutput(ANNexperience* currentExperienceInList)
{	
	string experienceOutputString = "";
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_Y_TRAIN_HOT_ENCODED
	int numberOfOutputNeurons = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_NUMBER_OF_OUTPUT_NEURONS;
	for(long i = 0; i < numberOfOutputNeurons; i++)
	{		
		string format = "%g";	//"%0.0f";
		if(i == currentExperienceInList->classTargetValue)
		{
			experienceOutputString = experienceOutputString + SHAREDvars.convertDoubleToString(1.0, format);
		}
		else
		{
			experienceOutputString = experienceOutputString + SHAREDvars.convertDoubleToString(0.0, format);
		}
		experienceOutputString = experienceOutputString + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_BATCH_FILE_DELIMITER;
	}
	#else
	experienceOutputString = SHAREDvars.convertLongToString(currentExperienceInList->classTargetValue);
	#endif
	return experienceOutputString;
}

bool GIApreprocessorPOStaggerDatabaseClass::externalANNexecuteScript(string scriptName)
{
	SHAREDvars.setCurrentDirectory(GIAposTaggerDatabaseFolderName);
	scriptName = string("python ") + scriptName + string(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_EXTENSION);
	//scriptName = string("./") + scriptName + string(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_SCRIPT_EXTENSION);	//requires "#!/usr/bin/env python" to be added to top of each py file
	cout << "scriptName = " << scriptName << endl;
	system(scriptName.c_str());
}
#endif
#endif


#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM
string GIApreprocessorPOStaggerDatabaseClass::DBgenerateFileName(const string POSambiguityInfoPermutation)
{
	//eg network/server/GIAPOStaggerDatabase/ffff/ffff/ffff/ffff/ffff/POSpermutationffffffffffffffffffffff.pos
	//string serverName = GIAdatabase.DBgenerateServerDatabaseName(&(GIAconnectionistNetworkPOStypeNameAbbreviationArray[firstFeatureInList->GIAsemanticParserPOStype]), fileType, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME, GIAposTaggerDatabaseFolderName);
	string serverName = GIAposTaggerDatabaseFolderName;
	string fileName = serverName;

	GIAdatabase.DBsetCurrentDirectory(&fileName);

	int numberOfPOSpermutationLevels = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS;
	string POSpermutationFileName = "";
	for(int level=0; level<numberOfPOSpermutationLevels; level++)
	{
		//cout << "level = " << level << endl;
		string folderName = DBgenerateSubFolderName(POSambiguityInfoPermutation, level, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_WORDS_PER_LEVEL);
		//cout << "folderName = " << folderName << endl;
		POSpermutationFileName = POSpermutationFileName + folderName;
		fileName = fileName + folderName + "/";
		GIAdatabase.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&folderName);
	}
	
	int numberOfRemainingWordsInPOSpermutation = (POSambiguityInfoPermutation.length()-GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS*GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_WORDS_PER_LEVEL);
	if(numberOfRemainingWordsInPOSpermutation > 0)
	{
		POSpermutationFileName = POSpermutationFileName + DBgenerateSubFolderName(POSambiguityInfoPermutation, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS, numberOfRemainingWordsInPOSpermutation);
	}
	
	POSpermutationFileName = string(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_FILE_NAME_PARTA) + POSpermutationFileName + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_FILE_NAME_EXTENSION;
	fileName = fileName + POSpermutationFileName;
	//cout << "fileName = " << fileName << endl;

	return fileName;
}

string GIApreprocessorPOStaggerDatabaseClass::DBgenerateSubFolderName(const string POSambiguityInfoPermutation, const int level, const int numberOfWordsPerLevel)
{
	string folderName = "";
	for(int i=level*numberOfWordsPerLevel; i<(level*numberOfWordsPerLevel)+numberOfWordsPerLevel; i++)
	{
		unsigned char POSpermutationIndexByte = (unsigned char)(POSambiguityInfoPermutation[i]);
		folderName = folderName + DBconvertByteToHex(POSpermutationIndexByte);
	}
	return folderName;
}

string GIApreprocessorPOStaggerDatabaseClass::DBconvertByteToHex(unsigned char byte)
{
	/*
	stringstream ss;
    	ss << hex << (int)byte;
	string hexString = ss.str();
	*/
	//cerr << "int(byte) = " << int(byte) << endl;
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_USE_4BIT_INDICES
	if(byte >= GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_MAX_SIZE)
	{
		cerr << "GIApreprocessorPOStaggerDatabaseClass::DBconvertByteToHex: error, (byte >= GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_MAX_SIZE{16})" << endl;
		cerr << "int(byte) = " << int(byte) << endl;
		exit(EXIT_ERROR);
	}
	string formatString = "%01x";
	#else
	string formatString = "%02x";
	#endif
	string hexString = SHAREDvars.convertIntToString(int(byte), formatString);
	/*
	cout << "GIApreprocessorPOStaggerDatabaseClass::DBconvertByteToHex:" << endl;
	cout << "byte = " << int(byte) << endl;
	cout << "hexString = " << hexString << endl;
	*/
	return hexString;
}

unsigned char GIApreprocessorPOStaggerDatabaseClass::DBconvertHexToByte(string hexString)
{
	unsigned char byte = (unsigned char)strtol(hexString.c_str(), NULL, 16);
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_USE_4BIT_INDICES
	if(byte >= GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_MAX_SIZE)
	{
		cerr << "GIApreprocessorPOStaggerDatabaseClass::DBconvertHexToByte: error, (byte >= GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_MAX_SIZE{16})" << endl;
		exit(EXIT_ERROR);
	}
	#endif
	/*
	cout << "GIApreprocessorPOStaggerDatabaseClass::DBconvertHexToByte:" << endl;
	cout << "byte = " << int(byte) << endl;
	cout << "hexString = " << hexString << endl;
	*/
	return byte;
}










bool GIApreprocessorPOStaggerDatabaseClass::DBreadPOSpermutationEstimates(const string POSambiguityInfoPermutation, vector<string>* centreWordPOSambiguityInfoList)
{
	bool POSpermutationEntryExistent = false;
	
	/*
	Format:
	ff 0000000000 [1 8 bit char (central POStag), space, then 1 32 bit numReferences]
	ff 0000000000
	ff 0000000000
	ff 0000000000
	...
	*/

	string referencesFileName = DBgenerateFileName(POSambiguityInfoPermutation);
	//now read file

	int numberOfLinesInFile = 0;
	if(SHAREDvars.getLinesFromFile(referencesFileName, centreWordPOSambiguityInfoList, &numberOfLinesInFile))
	{
		if(numberOfLinesInFile != 0)
		{
			POSpermutationEntryExistent = true;
		}
		else
		{
			cout << "GIApreprocessorPOStaggerDatabaseClass::DBreadPOSpermutationEstimates{} error: numberOfLinesInFile == 0; referencesFileName, " << referencesFileName << endl;		
		}
	}
	else
	{
		//cout << "GIApreprocessorPOStaggerDatabaseClass::DBreadPOSpermutationEstimates{} error: !SHAREDvars.getLinesFromFile; referencesFileName, " << referencesFileName << endl;		
	}
	
	return POSpermutationEntryExistent;
}

#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
bool GIApreprocessorPOStaggerDatabaseClass::DBwritePOSpermutationEstimate(const string POSambiguityInfoPermutation, const unsigned char centreWordPOSambiguityInfo)
{
	bool result = true;
	
	vector<string> centreWordPOSambiguityInfoList;
	bool POSpermutationEntryExistent = DBreadPOSpermutationEstimates(POSambiguityInfoPermutation, &centreWordPOSambiguityInfoList);
	
	bool foundMatchingCentreWordPOSambiguityInfo = false;
	if(POSpermutationEntryExistent)
	{
		for(int i=0; i<centreWordPOSambiguityInfoList.size(); i++)
		{
			string centreWordPOSambiguityReferenceString = centreWordPOSambiguityInfoList[i];
			string centreWordPOSambiguityInfoCurrentByteHexString = centreWordPOSambiguityReferenceString.substr(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_HEX_START_POS, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_HEX_LENGTH);
			unsigned char centreWordPOSambiguityInfoCurrentByte = DBconvertHexToByte(centreWordPOSambiguityInfoCurrentByteHexString);
			if(centreWordPOSambiguityInfoCurrentByte == centreWordPOSambiguityInfo)
			{				
				foundMatchingCentreWordPOSambiguityInfo = true;
				string centreWordPOSambiguityInfoNumberInstancesCurrentIntString = centreWordPOSambiguityReferenceString.substr(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_START_POS, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH);
				int centreWordPOSambiguityInfoNumberInstancesCurrentInt = SHAREDvars.convertStringToInt(centreWordPOSambiguityInfoNumberInstancesCurrentIntString);

				#ifdef GIA_PREPROCESSOR_POS_TAGGER_POS_DEBUG
				cout << "foundMatchingCentreWordPOSambiguityInfo (incrementing)" << endl;
				cout << "centreWordPOSambiguityInfoCurrentByte = " << centreWordPOSambiguityInfoCurrentByte << endl;
				cout << "centreWordPOSambiguityInfoNumberInstancesCurrentInt = " << centreWordPOSambiguityInfoNumberInstancesCurrentInt << endl;
				#endif
				
				#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_EFFICIENT_OVERWRITE_EXISTENT_REFERENCES_WITH_NEW_NUMBER_OF_INSTANCES
					//FUTURE: efficiently overwrite the number of instances of the references entry
				cerr << "GIApreprocessorPOStaggerDatabaseClass::DBwritePOSpermutationEstimate error{}: GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_EFFICIENT_OVERWRITE_EXISTENT_REFERENCES_WITH_NEW_NUMBER_OF_INSTANCES has not been coded" << endl;
				exit(EXIT_ERROR);
				#else
				centreWordPOSambiguityInfoNumberInstancesCurrentInt = centreWordPOSambiguityInfoNumberInstancesCurrentInt + 1;
				string formatString = generateIntFormatString(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH);
				centreWordPOSambiguityInfoNumberInstancesCurrentIntString = SHAREDvars.convertIntToString(centreWordPOSambiguityInfoNumberInstancesCurrentInt, formatString);	//rewrite number of instances
				centreWordPOSambiguityReferenceString = centreWordPOSambiguityReferenceString.replace(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_START_POS, GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH, centreWordPOSambiguityInfoNumberInstancesCurrentIntString);
				centreWordPOSambiguityInfoList[i] = centreWordPOSambiguityReferenceString;
				#endif
			}
		}
	}
	
	if(!foundMatchingCentreWordPOSambiguityInfo)
	{		
		//create a new entry in reference file
		int numberOfInstances = 1;
		//cout << "create a new entry in reference file" << endl;
		string centreWordPOSambiguityInfoCurrentHexString = DBconvertByteToHex(centreWordPOSambiguityInfo);
		string formatString = generateIntFormatString(GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH);
		string centreWordPOSambiguityInfoNumberInstancesCurrentIntString = SHAREDvars.convertIntToString(numberOfInstances, formatString);

		string centreWordPOSambiguityReferenceString = centreWordPOSambiguityInfoCurrentHexString + CHAR_SPACE + centreWordPOSambiguityInfoNumberInstancesCurrentIntString;
		centreWordPOSambiguityInfoList.push_back(centreWordPOSambiguityReferenceString);
		
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_POS_DEBUG
		cout << "!foundMatchingCentreWordPOSambiguityInfo (create new entry)" << endl;
		cout << "centreWordPOSambiguityReferenceString = " << centreWordPOSambiguityReferenceString << endl;
		#endif
		
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_EFFICIENT_OVERWRITE_EXISTENT_REFERENCES_WITH_NEW_NUMBER_OF_INSTANCES
		string referencesFileName = DBgenerateFileName(POSambiguityInfoPermutation);
		SHAREDvars.writeStringListToFile(referencesFileName, &centreWordPOSambiguityInfoList);
		#endif
	}
	
	#ifndef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_EFFICIENT_OVERWRITE_EXISTENT_REFERENCES_WITH_NEW_NUMBER_OF_INSTANCES
	string referencesFileName = DBgenerateFileName(POSambiguityInfoPermutation);
	SHAREDvars.writeStringListToFile(referencesFileName, &centreWordPOSambiguityInfoList);		//rewrite entire file
	#endif
	
	return result;
}
#endif


#endif

string GIApreprocessorPOStaggerDatabaseClass::generateIntFormatString(int numberOfCharacters)
{
	string formatString = string("%0") + SHAREDvars.convertIntToString(numberOfCharacters) + "d";
	return formatString;
}


string GIApreprocessorPOStaggerDatabaseClass::DBconvertByteToBinaryString(unsigned char byte)
{
	string binaryString = ""; 
	for(int i=0; i<8; i++)
	{
		int val = SHAREDvars.getBitValue(byte, i);
		binaryString = binaryString + SHAREDvars.convertIntToString(val);
	}
	return binaryString;
}

#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PREDICTION_VERIFICATION
bool GIApreprocessorPOStaggerDatabaseClass::verifyPOStaggerDatabasePredictionAgainstPOSambiguityInfo(const unsigned int centreWordPOSvaluePrediction, const unsigned int centreWordPOSambiguityInfo, int* centreWordPOSvalueFirstAmbiguousPrediction)
{
	bool predictionMatchesPOSambiguityInfo = false;
	if(centreWordPOSambiguityInfo == 0)
	{
		predictionMatchesPOSambiguityInfo = true;
	}
	else
	{
		for(int i=0; i<GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE; i++)
		{
			if(SHAREDvars.getBitValue(centreWordPOSambiguityInfo, i))
			{
				*centreWordPOSvalueFirstAmbiguousPrediction = i;

				if(centreWordPOSvaluePrediction == i)
				{
					predictionMatchesPOSambiguityInfo = true;
				}
			}
		}
	}
	return predictionMatchesPOSambiguityInfo;
}
#endif



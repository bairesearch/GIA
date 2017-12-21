import keras
from keras.models import Sequential
from keras.layers import Dense
from keras.models import model_from_json
#from keras.models import model_from_yaml
import numpy
import os

from GIAPOStaggerNeuralNetworkInitialise import *
from GIAPOStaggerNeuralNetworkLoad import loadModel

model = loadModel()

#generate predictions
XpredictBatchFileName = XpredictBatchFileNamePrepend + XtrainBatchFileExtension
YpredictBatchFileName = YpredictBatchFileNamePrepend + YtrainBatchFileExtension
XpredictBatch = numpy.genfromtxt(XpredictBatchFileName, delimiter=experienceDataFileDelimiter)
classes = model.predict_classes(XpredictBatch) 
numpy.savetxt(YpredictBatchFileName, classes, experienceDataFileDelimiter)

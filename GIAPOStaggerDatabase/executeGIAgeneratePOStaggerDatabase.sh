#!/bin/bash

exefolder="/home/systemusername/source/source"
lrpDataFolderName="/home/systemusername/source/source/LRPdata"
POStaggerDatabaseFolderName="/home/systemusername/source/GIAPOStaggerDatabase"
wikiDumpFolderName="/home/systemusername/soft/wiki/output"
numberOfWikiDumpBatchFiles="1203"
wikiDumpFileBatchIndex=0	#wikiDumpFileBatchIndex=0

while [  $wikiDumpFileBatchIndex -lt $numberOfWikiDumpBatchFiles ]; do
	
	echo "wikiDumpFileBatchIndex = $wikiDumpFileBatchIndex"
	GIAgeneratePOStaggerDatabaseCommand="$exefolder/GIAgeneratePOStaggerDatabase.exe -dbpostaggerfolder $POStaggerDatabaseFolderName -lrp -lrpfolder $lrpDataFolderName -wikiDumpFolder $wikiDumpFolderName -wikiDumpFileBatchIndex $wikiDumpFileBatchIndex"
	echo $GIAgeneratePOStaggerDatabaseCommand
	$GIAgeneratePOStaggerDatabaseCommand
	
	let wikiDumpFileBatchIndex=wikiDumpFileBatchIndex+1 
	
done

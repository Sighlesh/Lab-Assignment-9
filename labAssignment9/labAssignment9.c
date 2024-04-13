#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order;
};

// Fill out this structure
struct HashType
{
	struct RecordType* record;
	struct HashType* next;
	int key;
};

// Compute the hash function
int hash(int x)
{
	return x % 33; // 33,23, 37 seem valid
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	//
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType* pRecord;
	*ppData = NULL;
	//
	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		//null check
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}
		fclose(inFile);
	}
	return dataSz;
}

// prints the records
//given
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
	int i;
	printf("\nHash Records:\n");
	for (i = 0; i < hashSz; ++i)
	{
		// if index is occupied with any records, print all
		int key = pHashArray[i].key;
		if (key >= 0)
		{
			printf("\tindex %d -> ", i);
			printf("%d, %c, %d\n", pHashArray[key].record->id, pHashArray[key].record->name, pHashArray[key].record->order);
			if (pHashArray[key].next != NULL)
			{
				printf(" -> ");
				printf("%d, %d , %c, %d\n", pHashArray[key].next->key, pHashArray[key].next->record->id, pHashArray[key].next->record->name, pHashArray[key].next->record->order);
			}
		}
	}
}

int main(void)
{
	//givens
	struct RecordType* pRecords;
	int recordSz = 0;
	//name changed to open
	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	//Allocate mem
	struct HashType* hashArray = (struct HashType*)malloc(sizeof(struct HashType) * recordSz);

	for (int i = 0; i < recordSz; i++)
	{
		int key = hash(pRecords[i].id);

		if (hashArray[key].key != key)
		{
			//Create and intilize our entries
			hashArray[key].record = (struct RecordType*)malloc(sizeof(struct RecordType));
			hashArray[key].next = NULL;
			hashArray[key].key = key;
			hashArray[key].record->id = pRecords[i].id;
			hashArray[key].record->name = pRecords[i].name;
			hashArray[key].record->order = pRecords[i].order;
		}
		else if (hashArray[key].key == key)
		{
			//helper point to help go through the LLs
			struct HashType* helper = &hashArray[key];
			//while not tail, iterate to the next node
			while (helper->next != NULL) {
				//next node, stops at final node before null
				helper = helper->next;
			}
			//Create and intilize our entries
			helper->next = (struct HashType*)malloc(sizeof(struct HashType));
			helper->next->record = (struct RecordType*)malloc(sizeof(struct RecordType));
			//Fill out with info
			helper->next->key = key;
			helper->next->record->id = pRecords[i].id;
			helper->next->record->name = pRecords[i].name;
			helper->next->record->order = pRecords[i].order;
			helper->next->next = NULL;
		}
	}
	displayRecordsInHash(hashArray, recordSz);
	free(hashArray);
}//
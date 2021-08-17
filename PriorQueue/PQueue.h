#pragma once
#include "global.h"
#include <limits.h>

#define PRIOR_ERROR INT_MIN
#define POS_ERROR -1


typedef struct 
{
	PQINFO* pInfo;    
	int nPrior;    
} PQItem;        


typedef struct  
{
	PQItem* pPQueue;    // Tablica elementów
	int nPQSize;      // Maksymalna liczba elementów w kolejce
	int nPQCurrSize;  // Aktualna liczba elementów w kolejce
} PQueue;

PQueue* PQInit(int size );
int PQisEmpty( PQueue* q );  
int PQSize( PQueue* q ); 
int PQMaxPrior( PQueue* q );
int PQEnqueue( PQueue* q, PQINFO* pInfo, int prior );  
PQINFO* PQDequeue( PQueue* q  );
void PQClear( PQueue* q, void(*FreeMem)(const void*) );
void PQRelease( PQueue** q, void( *FreeMem )( const void* ) );
void PQPrint( PQueue* q, void( *PrintInfo )( const void* ),int i=0 );
int PQSetPrior( PQueue* q, PQINFO* pInfo,int prior, int( *CompareInfo )( const void*, const void* ) );
int PQsetPrior( PQueue* q, int index, int prior);
int PQGetPrior( PQueue* q, PQINFO* pInfo, int( *CompareInfo )( const void*, const void* ) );
int PQgetPrior( PQueue* q, int index  ); 
int PQFind( PQueue* q, PQINFO* pInfo, int( *CompareInfo )( const void*, const void* ) );
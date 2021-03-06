#include "global.h"
#include "PQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define QUEUE_SIZE 20
#define MAX_PRIOR 50

void FreeUsertype( const void* pItem );
int CompareUsertype( const void* pItem1, const void* pItem2 );
void PrintItem( const void* Item );

int main()
{
	srand( (unsigned)time( NULL ) );
	PQueue* pTestingQueue = PQInit( QUEUE_SIZE );
	if( !pTestingQueue )
	{
		printf( "Error: Queue cannot be initialized");
		return -1;
	}
	for( int i = 0; i<6; i++ )
	{
		PQINFO* pInfo = AllocateUsertype( i );
		if( !pInfo )
		{
			printf( "Error%d: Allocating usertype went wrong", i+1 );
			return -2;
		}
		int prior = 4*i+3; // 3 7 11 ... 23
		PQEnqueue( pTestingQueue, pInfo, prior );
	}
	PQPrint( pTestingQueue, PrintItem );
	PrintItem( PQDequeue( pTestingQueue ) );
	for( int i = 6; i<8; i++ )
	{
		PQINFO* pInfo = AllocateUsertype( i );
		if( !pInfo )
		{
			printf( "Error%d: Allocating usertype went wrong", i+1 );
			return -2;
		}
		int prior = 15*i-80;  // i = 6 -> prior 10 i=7 -> prior 25
		PQEnqueue( pTestingQueue, pInfo, prior );
	}
	PQINFO* pSought = AllocateUsertype( 4 );
	if( !pSought )
	{
		printf( "ErrorS: Allocating usertype went wrong" );
		return -2;
	}
	int index = PQFind( pTestingQueue, pSought, CompareUsertype );
	int unique_prior = 2;
	printf("\nIndex of sought item: %d\n", index);
	PQsetPrior( pTestingQueue, index, unique_prior  );
	pSought->nKey = 2;
	unique_prior = 17;
	PQSetPrior( pTestingQueue, pSought, unique_prior, CompareUsertype );
	PQPrint( pTestingQueue, PrintItem );
	PQRelease( &pTestingQueue, FreeUsertype );


   return 0;
}

void FreeUsertype( const void* pItem )
{
	PQINFO* pNew = (PQINFO*)pItem;
	free( pNew->sName );
	free( pNew );
}
int CompareUsertype( const void* pItem1, const void* pItem2 )
{
	int key1 = ( (PQINFO*)pItem1 )->nKey;
	int key2 = ( (PQINFO*)pItem2 )->nKey;
	if( key1==key2 ) return 0;
	if( key1>key2 ) return 1;
	return -1;
}
void PrintItem( const void* pItem )
{
	PQINFO* pItem2 = (PQINFO*)pItem;
	printf( "nKey: %d\tName: %s", pItem2->nKey, pItem2->sName);
}

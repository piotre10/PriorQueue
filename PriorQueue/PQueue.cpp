#include "PQueue.h"
#include <stdlib.h>
#include <stdio.h>


void UpdateDown( PQItem* pTab, int l, int p );
void UpdateUp( PQItem* pTab, int l, int p );

PQueue* PQInit( int size )
{ 
	if( size<1 ) return NULL;
	PQueue* pRes = (PQueue*)calloc( 1, sizeof( PQueue ) );
	if( !pRes ) return NULL;
	PQItem* pItemTab = (PQItem*)calloc( size, sizeof( PQItem ) );
	if( !pItemTab ) return NULL;
	pRes->pPQueue = pItemTab;
	pRes->nPQSize = size;
	pRes->nPQCurrSize = 0;
	return pRes;
}
int PQisEmpty( PQueue* q )
{ 
	return !(q->nPQCurrSize && q) ;
}
int PQSize( PQueue* q )
{
	return !q ? 0 : q->nPQCurrSize;
}
int PQMaxPrior( PQueue* q )
{
	return PQisEmpty( q ) ? PRIOR_ERROR : q->pPQueue->nPrior;
}
int PQEnqueue( PQueue* q, PQINFO* pInfo, int prior )
{ 
	if( !q ) return 0;
	if( PQSize( q )>=q->nPQSize ) return 0;
	PQItem* pNew = (PQItem*)calloc( 1, sizeof( PQItem ) );
	if( !pNew ) return 0;
	pNew->pInfo = pInfo;
	pNew->nPrior = prior;
	q->pPQueue[PQSize(q)] = *pNew;
	UpdateUp( q->pPQueue, 0, q->nPQCurrSize++ );
	return 1;
}
PQINFO* PQDequeue( PQueue* q )
{ 
	if( PQisEmpty(q) ) return NULL;
	PQINFO* pInfo = q->pPQueue->pInfo;
	q->pPQueue[0]= q->pPQueue[--( q->nPQCurrSize )];
	UpdateDown( q->pPQueue, 0, PQSize(q)-1 );
	return pInfo;
}
void PQClear( PQueue* q, void( *FreeMem )( const void* ) )
{
	while( !PQisEmpty(q) )
		FreeMem( PQDequeue( q ) );
}
void PQRelease( PQueue** q, void( *FreeMem )( const void* ) )
{
	if( !( *q ) )
	{
		printf( "Error: Queue does not exist (1)" );
		return;
	}
	PQClear( *q, FreeMem );
	free( (*q)->pPQueue );
	free( *q );
}
void PQPrint( PQueue* q, void( *PrintInfo )( const void* ), int i )
{
	if( !q )
	{
		printf( "Error: Queue does not exist (2)" );
		return;
	}
	if( i>=PQSize( q )||i<0 ) return;
	printf( "prior: %d\tItem: ", q->pPQueue[i].nPrior );
	PrintInfo( q->pPQueue[i].pInfo );
	printf( "\t(%d)\n", i );
	PQPrint( q, PrintInfo, 2*i+1 );
	PQPrint( q, PrintInfo, 2*i+2 );
}
int PQSetPrior( PQueue* q, PQINFO* pInfo,int prior, int( *CompareInfo )( const void*, const void* ) )
{ 
	return PQsetPrior( q, PQFind( q, pInfo, CompareInfo ), prior );
}
int PQsetPrior( PQueue* q, int index, int prior )
{ 
	int res = PRIOR_ERROR;
	if( ( index>=0 )&&( index<PQSize(q) ) )
	{
		res = q->pPQueue[index].nPrior;
		q->pPQueue[index].nPrior = prior;
		UpdateDown( q->pPQueue, index, q->nPQCurrSize-1 );
		UpdateUp( q->pPQueue, 0, index );
	}
	return res;
}
int PQGetPrior( PQueue* q, PQINFO* pInfo, int( *CompareInfo )( const void*, const void* ) )
{
	return PQgetPrior( q, PQFind( q, pInfo, CompareInfo ) );
}
int PQgetPrior( PQueue* q, int index )
{
	return ( index>=0 )&&( index<PQSize( q ) ) ? q->pPQueue->nPrior : PRIOR_ERROR;
}
int PQFind( PQueue* q, PQINFO* pInfo, int( *CompareInfo )( const void*, const void* ) )
{ 
	if( !q ) return POS_ERROR;
	int i = 0;
	for(; i<q->nPQCurrSize; i++ )
		if( !CompareInfo( q->pPQueue[i].pInfo, pInfo ) ) break;
	return  i>=q->nPQCurrSize  ? POS_ERROR : i;
}

void UpdateDown(PQItem* pTab, int l, int p )
{
	if( !pTab )
	{
		printf( "Error: Array does not exist (1)" );
		return;
	}
	PQItem x = pTab[l];
	int i = l;
	int j = 2*l+1;
	while( j<=p )
	{
		if( ( j<p )&&( pTab[j].nPrior<pTab[j+1].nPrior ) ) j++;
		if( x.nPrior>=pTab[j].nPrior ) break;
		pTab[i] = pTab[j];
		i = j;
		j = 2*i+1;
	}
	pTab[i] = x;
}
void UpdateUp( PQItem* pTab, int l, int p )
{
	if( !pTab )
	{
		printf( "Error: Array does not exist (2)" );
		return;
	}
	PQItem x = pTab[p];
	int i = p;
	int j = ( i-1 )/2;
	while( i>l )
	{
		if( pTab[j].nPrior>=x.nPrior ) break;
		pTab[i] = pTab[j];
		i = j;
		j = ( i-1 )/2;
	}
	pTab[i] = x;
}
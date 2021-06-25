#include "PQueue.h"
#include <stdlib.h>
#include <stdio.h>


void UpdateDown( PQItem* pTab, int l, int p );
void UpdateUp( PQItem* pTab, int l, int p );
PQItem* AllocateItem( );

PQueue* PQInit( int size )
{ 
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
	if( !q ) return 1;
	return !q->nPQCurrSize ;
}
int PQSize( PQueue* q )
{
	if( !q ) return 0;
	return q->nPQCurrSize;
}
int PQMaxPrior( PQueue* q )
{
	if( PQisEmpty( q ) ) return PRIOR_ERROR;
	return q->pPQueue->nPrior;
}
int PQEnqueue( PQueue* q, PQINFO* pInfo, int prior )
{ 
	if( !q ) return 0;
	if( PQSize( q )>=q->nPQSize ) return 0;
	PQItem* pNew = AllocateItem( );
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
	PQINFO* pInfo = AllocateUsertype( );
	if( !pInfo ) return NULL;
	pInfo = q->pPQueue->pInfo;
	q->pPQueue[0]= q->pPQueue[--( q->nPQCurrSize )];
	UpdateDown( q->pPQueue, 0, PQSize(q)-1 );
	return pInfo;
}
void PQClear( PQueue* q, void( *FreeMem )( const void* ) )
{
	while( !PQisEmpty )
		FreeMem( PQDequeue( q ) );
}
void PQRelease( PQueue** q, void( *FreeMem )( const void* ) )
{
	if( !( *q ) ) return;
	PQClear( *q, FreeMem );
	free( (*q)->pPQueue );
	free( *q );
}
void PQPrint( PQueue* q, void( *PrintInfo )( const void* ) )
{
	if( !q ) return;
	printf( "######## Queue: ########\n" );
	for( int i = 0; i<PQSize(q); i++ )
	{
		printf( "%d. prior: %d  Item: ", i+1, q->pPQueue[i].nPrior );
		PrintInfo( q->pPQueue[i].pInfo );

	}
	printf( "########################\n" );
}
int PQSetPrior( PQueue* q, PQINFO* pInfo,int prior, int( *CompareInfo )( const void*, const void* ) )
{ 
	int index = PQFind( q, pInfo, CompareInfo );
	if( ( index>=0 )&&( index<PQSize( q ) ) ) return PRIOR_ERROR;
	int res = q->pPQueue[index].nPrior;
	q->pPQueue[index].nPrior = prior;
	UpdateDown( q->pPQueue, index, PQSize(q)-1 );
	UpdateUp( q->pPQueue, 0, index );
	return res;
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
	int index = PQFind( q, pInfo, CompareInfo );
	if( ( index>=0 )&&( index<PQSize( q ) ) ) return PRIOR_ERROR;
	int res = q->pPQueue[index].nPrior;
	return res;
}
int PQgetPrior( PQueue* q, int index )
{
	int res = PRIOR_ERROR;
	if( ( index>=0 )&&( index<PQSize(q) ) ) res = q->pPQueue->nPrior;
	return res;
}
int PQFind( PQueue* q, PQINFO* pInfo, int( *CompareInfo )( const void*, const void* ) )
{ 
	if( !q ) return POS_ERROR;
	int i = 0;
	for(; i<q->nPQCurrSize; i++ )
		if( !CompareInfo( q->pPQueue[i].pInfo, pInfo ) ) break;
	if( i>=q->nPQCurrSize ) return POS_ERROR;
	return i;
}


PQItem* AllocateItem( )
{
	PQItem* pRes = (PQItem*)calloc( 1, sizeof( PQItem ) );
	if( !pRes ) return NULL;
	PQINFO* pInfo = AllocateUsertype( );
	if( !pInfo ) return NULL;
	pRes->pInfo = pInfo;
	pRes->nPrior = PRIOR_ERROR;
	return pRes;
}
void UpdateDown(PQItem* pTab, int l, int p )
{
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
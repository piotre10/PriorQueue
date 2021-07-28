#include "PQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void UpdateDown( PQItem* pTab, int l, int p );
void UpdateUp( PQItem* pTab, int l, int p );

PQueue* PQInit( int size )
{ 
	if( size<1 ) return NULL;
	PQueue* pRes = (PQueue*)calloc( 1, sizeof( PQueue ) );
	if( !pRes ) return NULL;
	pRes->pPQueue = (PQItem*)calloc( size, sizeof( PQItem ) );
	if( !pRes->pPQueue ) return NULL;
	pRes->nPQSize = size;
	return pRes;
}
int PQisEmpty( PQueue* q )
{ 
	return !q || !q->nPQCurrSize ;
}
int PQSize( PQueue* q )
{
	return ( !q ) ? PRIOR_ERROR : q->nPQCurrSize;
}
int PQMaxPrior( PQueue* q )
{
	return PQgetPrior( q, 0 );
}
int PQEnqueue( PQueue* q, PQINFO* pInfo, int prior )
{ 
	if( !q ) return 0;
	int nSize = PQSize( q );
	if( nSize<0 || nSize >=q->nPQSize ) return POS_ERROR;
	q->pPQueue[nSize].pInfo = pInfo;
	q->pPQueue[nSize].nPrior = prior;
	UpdateUp( q->pPQueue, 0, nSize);
	q->nPQCurrSize++;
	return 1;
}
PQINFO* PQDequeue( PQueue* q )  // Do poprawki
{ 
	if( PQisEmpty(q) ) return NULL;
	PQINFO* pInfo = q->pPQueue->pInfo;
	int index = -- q->nPQCurrSize;
	if( index>0 && index < q->nPQSize )
	{
		q->pPQueue[0] = q->pPQueue[index];
		memset( &q->pPQueue[index], 0, sizeof( PQItem ) );
		UpdateDown( q->pPQueue, 0, index-1 );
		return pInfo;
	}
	return NULL;
}
void PQClear( PQueue* q, void( *FreeMem )( const void* ) )
{
	if( !q || !FreeMem )
	{
		printf( "Error: Queue does not exist or free function is invalid (3)" );
		return;
	}
	while( !PQisEmpty(q) )
		FreeMem( PQDequeue( q ) );
}
void PQRelease( PQueue** q, void( *FreeMem )( const void* ) )
{
	if( !q || !( *q ))
	{
		printf( "Error: Queue does not exist (1)" );
		return;
	}
	PQClear( *q, FreeMem );
	free( (*q)->pPQueue );
	free( *q );
	*q = NULL;
}
void PQPrint( PQueue* q, void( *PrintInfo )( const void* ), int i )
{
	if( !q || i<0 )
	{
		printf( "Error: Queue does not exist (2)" );
		return;
	}
	if( i>=PQSize( q ) ) return;
	printf( "prior: %d\tItem: ", q->pPQueue[i].nPrior );
	if( PrintInfo ) PrintInfo( q->pPQueue[i].pInfo );
	printf( "\t(%d)\n", i );
	PQPrint( q, PrintInfo, 2*i+1 );
	PQPrint( q, PrintInfo, 2*i+2 );
}
int PQSetPrior( PQueue* q, PQINFO* pInfo, int prior, int( *CompareInfo )( const void*, const void* ) )
{
	return PQsetPrior( q, PQFind( q, pInfo, CompareInfo ), prior ); /*CompareInfo == NULL i q == NULL
		wylapia odpowiednio funkcje PQFind i PQsetPrior i w konsekwencji funkcja i tak zwroci prior error
		wiec nie dawalem oddzielnego sprawdzenia w tej funkcji*/
}
int PQsetPrior( PQueue* q, int index, int prior )
{
	if( !q ) return PRIOR_ERROR;
	int res = PQgetPrior(q, index);
	if( res!=PRIOR_ERROR )
	{
		int old_prior = q->pPQueue[index].nPrior;
		q->pPQueue[index].nPrior = prior;
		if(prior<old_prior) UpdateDown( q->pPQueue, index, PQSize(q)-1 );
		else UpdateUp( q->pPQueue, 0, index );
	}
	return POS_ERROR;
}
int PQGetPrior( PQueue* q, PQINFO* pInfo, int( *CompareInfo )( const void*, const void* ) )
{
	return PQgetPrior( q, PQFind( q, pInfo, CompareInfo ) ); /*CompareInfo == NULL i q == NULL
		wylapia odpowiednio funkcje PQFind i PQgetPrior i w konsekwencji funkcja i tak zwroci prior error
		wiec nie dawalem oddzielnego sprawdzenia w tej funkcji*/
}

int PQgetPrior( PQueue* q, int index )
{
	if( PQisEmpty( q ) ) return PRIOR_ERROR;
	return ( index>=0 )&&( index<PQSize( q ) ) ? q->pPQueue->nPrior : POS_ERROR; 
	// jeœli q == NULL to PQSize = 0 i funkcja zwraca prior error
}
int PQFind( PQueue* q, PQINFO* pInfo, int( *CompareInfo )( const void*, const void* ) )
{ 
	if( !q ) return PRIOR_ERROR;
	if( !CompareInfo ) return POS_ERROR;
	for(int i=0; i<PQSize(q); i++ )
		if( !CompareInfo( q->pPQueue[i].pInfo, pInfo ) ) return i;
	return POS_ERROR;
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
	while( i>l &&pTab[j].nPrior<x.nPrior )
	{
		pTab[i] = pTab[j];
		i = j;
		j = ( i+1 )/2 - 1;  // = (i-1)/2 ??
	}
	pTab[i] = x;
}
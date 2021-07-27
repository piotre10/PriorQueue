#include "global.h"
#include <stdlib.h>

PQINFO* AllocateUsertype( int n )
{
	PQINFO* pItem = (PQINFO*)calloc( 1, sizeof( PQINFO ) );
	if( !pItem ) return NULL;
	int j = ( n==0 ) ?  2 : 1; //////////////////   Jakos lepiej?
	for( int i = n; i>0; i /= 10 ) j++; ////////////////
	pItem->sName = (char*)calloc( j, sizeof( char ) );
	if( !pItem->sName ) return NULL;
	pItem->nKey = n;
	itoa(n, pItem->sName,10);
	return pItem;
}
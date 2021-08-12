#include "global.h"
#include <stdlib.h>

PQINFO* AllocateUsertype( int n )
{
	PQINFO* pItem = (PQINFO*)calloc( 1, sizeof( PQINFO ) );
	if( !pItem ) return NULL;
	int i = n;
	int j = 1;
	do
	{
		j++;
		i /= 10;
	} while( i>0 );
	pItem->sName = (char*)calloc( j, sizeof( char ) );
	if( !pItem->sName ) return NULL;
	pItem->nKey = n;
	itoa(n, pItem->sName,10);
	return pItem;
}
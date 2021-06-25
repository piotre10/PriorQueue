#include "global.h"
#include <stdlib.h>

PQINFO* AllocateUsertype( )
{
	PQINFO* pItem = (PQINFO*)calloc( 1, sizeof( PQINFO ) );
	if( !pItem ) return NULL;
	int* t = (int*)calloc( 2, sizeof( int ) );
	if( !t ) return NULL;
	pItem->nKey = 0;
	pItem->pTab = t;
	return pItem;
}
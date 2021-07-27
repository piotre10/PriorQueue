#pragma once

typedef struct
{
	int nKey;
	char* sName;
}PQInfo;
#define PQINFO PQInfo

PQINFO* AllocateUsertype( int n );

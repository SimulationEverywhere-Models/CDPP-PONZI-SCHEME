/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Elizabeth Hosang
*
*  EMAIL: mailto://eahosang@scs.carleton.ca
*
*  DATE: 25/10/2011
*
*******************************************************************/

#include "modeladm.h"
#include "mainsimu.h"
#include "InitialPool.h"                // Initial Pool
#include "LongTermPoolType.h"	// class LongTermPoolType
#include "FundType.h"
#include "PonziType.h"

void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<InitialPool>() , "InitialPool" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<LongTermPoolType>() , "LongTermPoolType" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<FundType>() , "FundType" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<PonziType>() , "PonziType" ) ;
}

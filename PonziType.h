/*******************************************************************
*
*  DESCRIPTION: Initial Investor Pool for Ponzi Scheme Model
*
*  AUTHOR: Elizabeth Hosang
*
*  EMAIL: mailto://eahosang@scs.carleton.ca
*
*  DATE: 25/10/2011
*
*******************************************************************/
#ifndef __PONZITYPE_H
#define __PONZITYPE_H
/** include files **/
#include "atomic.h"  // class Atomic

/** forward declarations **/
//class Distribution ;

/** declarations **/
class PonziType: public Atomic
{
public:

	PonziType( const string &name = "PonziType" ) ;	 // Default constructor

	~PonziType();					// Destructor

	virtual string className() const
		{return "PonziType";}

	enum PoolState {Initial, Recruiting, Successful, Flee, Disabled};
	static const int DisableFlag = 4;

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage &msg );

	Model &internalFunction( const InternalMessage &msg );

	Model &outputFunction( const InternalMessage &msg );

private:
	//  Income Ports
	const Port &fund_in ;   // Status from bank account
	Port &principal_out ;    // Amount of money requested from the bank
    Port &investorCount_out;    // Number of investors moving to longer term
    Port &statsOut_out;     // Used for reporting status

	//Distribution *dist ;
    double m_initialPayment;  // Amount invested by current investors
    double m_randnumber;
    int  m_timePeriod;    // Time period between new investors
    bool m_timerStarted;
    PoolState m_state;    // Internal state of the model

	//Distribution &distribution()
		//	{return *dist;}
};	// class InitialPool


#endif    //__PONZITYPE_H

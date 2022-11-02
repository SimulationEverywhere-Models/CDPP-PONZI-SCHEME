/*******************************************************************
*
*  DESCRIPTION: Initial Investor Pool for Ponzi Scheme Model
*
*  AUTHOR: Elizabeth Hosang
*
*  EMAIL: mailto://eahosang@scs.carleton.ca
*
*
*  DATE: 25/10/2011
*
*******************************************************************/
#ifndef __INITIALPOOL_H
#define __INITIALPOOL_H
/** include files **/
#include "atomic.h"  // class Atomic

/** forward declarations **/
//class Distribution ;

/** declarations **/
class InitialPool: public Atomic
{
public:

	InitialPool( const string &name = "InitialPool" ) ;	 // Default constructor

	~InitialPool();					// Destructor

	virtual string className() const
		{return "InitialPool";}

	enum PoolState {NoBalance, Counting, PayoutPending, Disabled};
	static const int DisableFlag = 1;

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage &msg );

	Model &internalFunction( const InternalMessage &msg );

	Model &outputFunction( const InternalMessage &msg );

private:
	//  Income Ports
	const Port &count_in ;   // Number of new investors to add to the pool
	const Port &initialAmount_in;   // Amount of money contributed by new investors
	const Port &rate_in;     // Current interest rate paid to investors
	const Port &payout_in;   // Amount of money paid to investors
	const Port &control_in;
	//const Port &control_in; // Control for testing
	Port &payoutReq_out ;    // Amount of money requested from the bank
    Port &longCount_out;    // Number of investors moving to longer term
    Port &longAmount_out;   // Contribution to longer term investment by new long term
    Port &statsOut_out;     // Used for reporting status
    Port &totalCount_out;   // Total count of investors
    int    m_totalInvestors;    // All investors who ever entered the fund
    int    m_currentInvestors;  // Current count of initial investors
    int    m_tmpInvestors;
    double m_tmpAmount;
	//Distribution *dist ;
    double m_currentAmount;  // Amount invested by current investors
    double m_randnumber;
    double m_rate;         	// Interest rate paid on initial investments
    int  m_payoutPeriod;  // Amount of time to wait before triggering a Payout
    int  m_timePeriod;    // Time period
    int  m_interval;
    bool m_timerStarted;
    double m_lastPayoutReq;
    PoolState m_state;    // Internal state of the model
	//Distribution &distribution()
		//	{return *dist;}
};	// class InitialPool


#endif    //__INITIALPOOL_H

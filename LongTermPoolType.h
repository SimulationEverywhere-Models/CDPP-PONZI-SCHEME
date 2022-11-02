/********************************************************************
*
*   DESCRIPTION: Long Term Investor Pool for Ponzi Scheme Model
*
*   AUTHOR: Elizabeth Hosang
*
*   Date: 25/10/2011
*
*********************************************************************/

#ifndef __LongTermPoolType_H
#define __LongTermPoolType_H

/** include files **/
#include "atomic.h"  // class Atomic

/** forward declarations **/
// Ej: class Distribution ;

/** declarations **/
class LongTermPoolType: public Atomic
{
public:
	LongTermPoolType( const string &name = "LongTermPoolType" ) ;	 // Default constructor
	~LongTermPoolType();					// Destructor
	virtual string className() const
		{return "LongTermPoolType";}

	enum PoolState {NoBalance, Counting, PayoutPending, Disabled};
	static const int DisableFlag = 2;

protected:
	Model &initFunction();	
	Model &externalFunction( const ExternalMessage &msg );
	Model &internalFunction( const InternalMessage &msg );
	Model &outputFunction( const InternalMessage &msg );

private:

	const Port &count_in;  	// Number of investors to be added to the long term pool
	const Port &amount_in; 	// Amount of principal invested by investors in the long term pool
	const Port &payout_in;  // Money received from the bank account - represents payout
	const Port &rate_in;	// Interest in
	const Port &control_in; // Control for testing
	Port &newRecruits_out; 	// New investors recruited by members of the Long Term investors pool
	Port &payoutReq_out;   	// Requested payout from the bank account - interest only
	Port &newPrincipal_out;	// Amount of principal contributed by new investors
	Port &statsOut_out;     // Used for reporting status

	double m_rate;  	// Interest rate paid on principal
	int m_currentInvestors;		// number of long term investors
	double m_principal;		// Total principal held by long term investors.  Interest is not added to this amount
	int m_payoutPeriod;		// number of cycles between payouts
	PoolState m_state;
	int m_tmpInvestors;
	double m_tmpPrincipal;
	int m_timePeriod;
	int m_interval;
	bool m_timerStarted;
	double m_initial;
	double m_lastPayoutReq;

	Time sigma;
	
	// Time elapsed since the last state transition until now
	Time elapsed;
	
	// Time remaining to complete the last programmed Lifetime
	Time timeLeft;	
};	// class LongTermPoolType


#endif   //__LongTermPoolType_H 

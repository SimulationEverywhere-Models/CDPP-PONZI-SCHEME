/*******************************************************************
*
*  DESCRIPTION: Bank Account model for Ponzi Scheme Model
*
*  AUTHOR: Elizabeth Hosang
*
*  EMAIL: mailto://eahosang@scs.carleton.ca
*
*
*  DATE: 25/10/2011
*
*******************************************************************/
#ifndef __FUNDTYPE_H
#define __FUNDTYPE_H
/** include files **/
#include "atomic.h"  // class Atomic

/** forward declarations **/
//class Distribution ;

/** declarations **/
class FundType: public Atomic
{
public:

	FundType( const string &name = "FundType" ) ;	 // Default constructor

	~FundType();					// Destructor

	virtual string className() const
		{return "FundType";}

	enum FundState {NoBalance, Balance, PayoutPending, Empty, TargetReached, Disabled};
	static const int DisableFlag = 3;

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage &msg );

	Model &internalFunction( const InternalMessage &msg );

	Model &outputFunction( const InternalMessage &msg );

private:
	//  Income Ports
	const Port &payment_in ;   // Number of new investors to add to the pool
	const Port &payReq_in;   // Amount of money contributed by new investors
	const Port &rate_in;     // Current interest rate paid to investors
	const Port &control_in; // Control for testing
	Port &rate_out;     	// Inform accounts of interest rate change
	Port &payout_out ;    	// Amount of money requested from the bank
    Port &alert_out;    	// alert to fund runner
    Port &statsOut_out;     // Used for reporting status
    Port &balance_out;		// Reporting the regular balance

    double    m_balance;    // All investors who ever entered the fund
    double 		m_target;
	//Distribution *dist ;
    double m_randnumber;
    double m_rate;         	// Interest rate paid on initial investments
    FundState m_state;    // Internal state of the model
    double m_payout;
    int m_paymentPeriod;
    double m_payout2;    // for when the payout requests overlap

	//Distribution &distribution()
		//	{return *dist;}
};	// class InitialPool


#endif    //__FUNDTYPE_H

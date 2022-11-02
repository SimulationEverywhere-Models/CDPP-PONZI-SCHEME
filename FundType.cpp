/*******************************************************************
*
*  DESCRIPTION: Bank fund model for Ponzi Scheme Model
*
*  AUTHOR: Elizabeth Hosang
*
*  EMAIL: mailto://eahosang@scs.carleton.ca
*
*
*  DATE: 25/10/2011
*
*******************************************************************/

#include <stdlib.h>
#include <math.h>            // fabs( ... )
#include "randlib.h"         // Random numbers library
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include "FundType.h"

/*******************************************************************
* Function Name: Bank account register
* Description: constructor
********************************************************************/
FundType::FundType( const string &name ): Atomic( name )
, payment_in(addInputPort("payment_in"))
, payReq_in(addInputPort("payReq_in"))
, rate_in(addInputPort("rate_in"))
, rate_out(addOutputPort("rate_out"))
, payout_out(addOutputPort("payout_out"))
, alert_out(addOutputPort("alert_out"))
, statsOut_out(addOutputPort("statsOut_out"))
, control_in(addInputPort("&control_in"))
, balance_out(addOutputPort("balance_out"))
{
	m_balance = 0.0;

	//Distribution *dist ;
	m_randnumber = 0;
	m_rate = 0.05;
	m_target = 0;
	m_state = NoBalance;
	string parm( MainSimulator::Instance().getParameter( "FundType", "targetBalance" ) ) ;
	m_target = str2Int(parm);   // Will be loaded from initial event file later
	m_paymentPeriod = 5*2;
	m_payout2 = 0;
	m_payout = 0;
}

/*******************************************************************
* Function Name: InitialPool
* Description: constructor
********************************************************************/
FundType::~FundType()
{
	/*if (dist != null)
	{
		delete dist;
		dist = null;
	} */
}

Model &FundType::initFunction()
{
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: Process Incoming data
********************************************************************/
Model &FundType::externalFunction( const ExternalMessage &msg )
{
	int inputAmount;
	inputAmount = msg.value();
	if (m_state == TargetReached || m_state == Empty )
	{
		passivate();
		return *this;
	}

	if (msg.port() == payment_in)
	{
		m_balance += inputAmount;
		if (m_state == NoBalance && m_balance > 0)
		{
			m_state = Balance;
		}
		//sendOutput(msg.time(), statsOut_out, m_target);
		sendOutput(msg.time(), balance_out, m_balance);
		//sendOutput(msg.time(), statsOut_out, 1015);
		if (m_balance >= m_target)
		{
			m_state = TargetReached;
			//sendOutput(msg.time(), statsOut_out, 2015);
			holdIn(active, Time::Zero);
		}
		else
		{
			//sendOutput(msg.time(), statsOut_out, 3015);
		}

	}
	else if (msg.port() == payReq_in)
	{
		m_state = PayoutPending;
		if (m_payout > 0)
		{
			m_payout2 = msg.value();
		}
		else
		{
			m_payout = msg.value();
		}
		//sendOutput(msg.time(), statsOut_out, 1015);
		//sendOutput(msg.time(), statsOut_out, m_payout);
		// Delay paying to make sure we have money to pay
		holdIn(active, Time(0, 0, m_paymentPeriod, 0));
	}
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &FundType::internalFunction( const InternalMessage &msg )
{
	passivate();
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &FundType::outputFunction( const InternalMessage &msg )
{
	//sendOutput(msg.time(), statsOut_out, 5010);
	if (m_state == PayoutPending)
	{
		if (m_payout + m_payout2 >= m_balance)
		{
			// Fund is out of money
			m_state = Empty;
			sendOutput(msg.time(), statsOut_out, 0);
			sendOutput(msg.time(), alert_out, 0);
			m_payout = 0;
		}
		else
		{
			//Fund can make payout
			m_balance -= m_payout;

			//sendOutput(msg.time(), payout_out, 2015);
			sendOutput(msg.time(), payout_out, m_payout);
			m_payout = 0;

			if (m_payout2 > 0)
			{
				m_balance -= m_payout2;
				sendOutput(msg.time(), payout_out, m_payout2);
				m_payout2 = 0;
			}
			sendOutput(msg.time(), balance_out, m_balance);
		}
	}
	else if (m_state == TargetReached)
	{
		//sendOutput(msg.time(), statsOut_out, 3010);
		sendOutput(msg.time(), statsOut_out, 1);
		sendOutput(msg.time(), alert_out, 1);
		passivate();
	}
	return *this;
}

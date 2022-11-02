/********************************************************************
*
*   DESCRIPTION: Long Term Investor Pool for Ponzi Scheme Model
*
*   AUTHOR: Elizabeth Hosang
*
*   Date: 25/10/2011
*
*********************************************************************/

/** include files **/
#include "LongTermPoolType.h"           // base header
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"		// str2float ...


/*******************************************************************
* Function Name: LongTermPoolType
* Description: constructor
********************************************************************/
LongTermPoolType::LongTermPoolType( const string &name )
: Atomic( name )
, count_in(addInputPort("count_in"))
, amount_in(addInputPort("amount_in"))
, payout_in(addInputPort("payout_in"))
, rate_in(addInputPort("rate_in"))
, newRecruits_out(addOutputPort("newRecruits_out"))
, payoutReq_out(addOutputPort("payoutReq_out"))
, newPrincipal_out(addOutputPort("newPrincipal_out"))
, statsOut_out(addOutputPort("statsOut_out"))
, control_in(addInputPort("control_in"))
{
	m_rate = 0.05;
	m_currentInvestors = 0;
	m_principal = 0.0;
	m_state = NoBalance;
	m_tmpInvestors = 0;
	m_tmpPrincipal = 0.0;
	m_timerStarted = false;
	//string parm( MainSimulator::Instance().getParameter( description(), "timeSlice" ) ) ;
	//m_timePeriod = str2Int(parm);   // Will be loaded from initial event file later
	m_timePeriod = 5;
	//string parm2(MainSimulator::Instance().getParameter( description(), "timePeriod" ) );
	//m_payoutPeriod = str2Int(parm2);  // Period before payout request
	m_payoutPeriod = 4;
	m_interval = m_payoutPeriod * m_timePeriod;
	m_initial = 100;
	m_lastPayoutReq = 0;

}

/*******************************************************************
* Function Name: initFunction
********************************************************************/
Model &LongTermPoolType::initFunction()
{

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: This method executes when an external event is received.
********************************************************************/
Model &LongTermPoolType::externalFunction( const ExternalMessage &msg )
{
	int inputAmount;
	Time payoutTime = Time(0, 0, m_interval, 0);
	//sendOutput(msg.time(), statsOut_out, 2005);
	if (m_state == Disabled)
	{
		passivate();
	}
	if (this->state() == passive)
	{
		if (msg.port() == control_in)
		{
			m_state = Disabled;
			passivate();
		}
		inputAmount = msg.value();
		//sendOutput(msg.time(), statsOut_out, 3005);
		if (msg.port() == count_in)
		{
			//sendOutput(msg.time(), statsOut_out, 4005);
			if (m_state == Counting || m_state == NoBalance)
			{
				//sendOutput(msg.time(), statsOut_out, 5005);
				m_state = Counting;
				sendOutput(msg.time(), statsOut_out, inputAmount);
				m_currentInvestors += inputAmount;
				sendOutput(msg.time(), statsOut_out, m_currentInvestors);
			}
			else
			{
				//sendOutput(msg.time(), statsOut_out, 6005);
				m_tmpInvestors += inputAmount;
			}
		}
		else if (msg.port() == amount_in)
		{
			//sendOutput(msg.time(), statsOut_out, 5005);
			if (m_state == Counting || m_state == NoBalance)
			{
				//sendOutput(msg.time(), statsOut_out, 6005);
				m_state = Counting;
				m_principal += inputAmount;
				if (!m_timerStarted)
				{
					holdIn(passive, Time(payoutTime));
					m_timerStarted = true;
				}

			}
			else
			{
				m_tmpPrincipal += inputAmount;
				holdIn(passive, nextChange());
			}
		}
		else if (msg.port() == rate_in)
		{
			//sendOutput(msg.time(), statsOut_out, 8005);
			double rate = inputAmount;
			m_rate = rate / 100.0;
			holdIn(passive, nextChange());
		}
		else if (msg.port() == payout_in)
		{
			if (m_state == PayoutPending)
			{
				//sendOutput(msg.time(), statsOut_out, 11005);

				// Make sure this is my payment
				if (msg.value() == m_lastPayoutReq)
				{
					//sendOutput(msg.time(), statsOut_out, 13005);
					// Payout received; restart counting cycle
					if (!m_timerStarted)
					{
						holdIn(passive, 0);
					}
				}
			}
		}
	}
	else
	{
		//sendOutput(msg.time(), statsOut_out, 12005);
	}
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
* Description: This method executes when the TA has expired, right after the outputFunction has finished.
* 			   The new state and TA should be set.
********************************************************************/
Model &LongTermPoolType::internalFunction( const InternalMessage &msg )
{

	if (m_state == Disabled)
	{
		passivate();
		return *this;
	}
	if (m_state == Counting)
	{
		if (!m_timerStarted)
		{
			m_timerStarted = true;
			//sendOutput(msg.time(), statsOut_out, 9995);
			holdIn(passive, Time(0, 0, m_interval, 0));
		}
		else
		{
			//sendOutput(msg.time(), statsOut_out, 7775);
			passivate();
		}
	}
	else
	{
		//sendOutput(msg.time(), statsOut_out, 6665);
		passivate();
	}
	return *this;

}

/*******************************************************************
* Function Name: outputFunction
* Description: This method executes when the TA has expired. After this method the internalFunction is called.
*              Output values can be send through output ports
********************************************************************/
Model &LongTermPoolType::outputFunction( const InternalMessage &msg )
{
	if (m_state == Counting)
	{
		double amount = m_rate * m_principal;
		sendOutput(msg.time(), payoutReq_out, amount);
		sendOutput(msg.time(), statsOut_out, amount);
		m_lastPayoutReq = amount;
		m_state = PayoutPending;
		m_timerStarted = false;
	}
	else if (m_state == PayoutPending)
	{
		//sendOutput(msg.time(), statsOut_out, 8885);
		m_currentInvestors += m_tmpInvestors;
		m_tmpInvestors = 0;
		m_principal += m_tmpPrincipal;
		m_tmpPrincipal = 0.0;
		m_state = Counting;

		// Calculate the number of new investors to create
		//sendOutput(msg.time(), statsOut_out, 1005);
		int newInvestors = m_currentInvestors * 0.2;
		sendOutput(msg.time(), statsOut_out, m_currentInvestors);
		sendOutput(msg.time(), newRecruits_out, newInvestors);
		sendOutput(msg.time(), statsOut_out, newInvestors);
		sendOutput(msg.time(), newPrincipal_out, newInvestors * m_initial);

		// Calculate the number of investors leaving

		m_state = Counting;
	}
	return *this;

}

LongTermPoolType::~LongTermPoolType()
{
	//TODO: add destruction code here. Free distribution memory, etc. 
}

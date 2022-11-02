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

#include <stdlib.h>
#include <math.h>            // fabs( ... )
#include "randlib.h"         // Random numbers library
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include "InitialPool.h"

/*******************************************************************
* Function Name: Balanceverifier
* Description: constructor
********************************************************************/
InitialPool::InitialPool( const string &name ): Atomic( name )
, count_in(addInputPort("count_in"))
, initialAmount_in(addInputPort("initialamount_in"))
, rate_in(addInputPort("rate_in"))
, payout_in(addInputPort("payout_in"))
, payoutReq_out(addOutputPort("payoutReq_out"))
, longCount_out(addOutputPort("longCount_out"))
, longAmount_out(addOutputPort("longAmount_out"))
, statsOut_out(addOutputPort("statsOut_out"))
, totalCount_out(addOutputPort("totalCount_out"))
, control_in(addInputPort("control_in"))
{
	m_totalInvestors = 0;
	m_currentInvestors = 0;
	m_tmpInvestors = 0;
	m_tmpAmount = 0.0;
	//*dist ;
	m_currentAmount = 0.0;
	m_randnumber = 0;
	m_timerStarted = false;
	m_rate = .05;

	string parm( MainSimulator::Instance().getParameter( description(), "timeSlice" ) ) ;
	m_timePeriod = str2Int(parm);   // Will be loaded from initial event file later
	string parm2(MainSimulator::Instance().getParameter( description(), "timePeriod" ) );
	m_payoutPeriod = str2Int(parm2);  // Period before payout request
	m_interval = m_payoutPeriod * m_timePeriod;
	m_state = NoBalance;
	m_lastPayoutReq = 0;
/*	try
	{
		dist = Distribution::create( MainSimulator::Instance().getParameter( description(), "distribution" ) );

		MASSERT( dist );
	}
	catch( InvalidDistribution &e )
	{
		e.addText( "The model " + description() + " has distribution problems!" ) ;
		e.print(cerr);
		MTHROW( e ) ;
	} catch( MException &e )
	{
		MTHROW( e ) ;
	}
	*/

}

/*******************************************************************
* Function Name: InitialPool
* Description: constructor
********************************************************************/
InitialPool::~InitialPool()
{
	/*if (dist != null)
	{
		delete dist;
		dist = null;
	} */
}

Model &InitialPool::initFunction()
{

}

/*******************************************************************
* Function Name: externalFunction
* Description: Process Incoming data
********************************************************************/
Model &InitialPool::externalFunction( const ExternalMessage &msg )
{
	int inputAmount;
	Time payoutTime = Time(0, 0, m_interval, 0);
	//sendOutput(msg.time(), statsOut_out, m_interval);
	//sendOutput(msg.time(), statsOut_out, 1001);

	if (msg.port() == control_in)
	{
		m_state = Disabled;
	}
	if (m_state == Disabled)
	{
		passivate();
		return *this;
	}
  if ( this->state() == passive)
   {
	  inputAmount = msg.value();
	  //sendOutput(msg.time(), statsOut_out, 2001);

	  if (msg.port() == count_in)
	  {
		  if (m_state == Counting || m_state == NoBalance)
		  {
			  // New investors in
			  //sendOutput(msg.time(), statsOut_out, 3001);
			  m_currentInvestors += inputAmount;
			  m_totalInvestors += inputAmount;
			  sendOutput(msg.time(), statsOut_out, m_currentInvestors);
			  sendOutput(msg.time(), totalCount_out, m_totalInvestors);
		  }
		  else
		  {
			  // Use temporary values while we wait for payout return
			  //sendOutput(msg.time(), statsOut_out, 4001);
			  m_tmpInvestors += inputAmount;
			  //sendOutput(msg.time(), statsOut_out, m_tmpInvestors);
		  }
	  }
	  else if (msg.port() == initialAmount_in)
	  {
		  // New amount in
		  //sendOutput(msg.time(), statsOut_out, 5001);
		  if (m_state == Counting || m_state == NoBalance)
		  {
			  m_state = Counting;
			  m_currentAmount += inputAmount;
			  // if needed, start timer for payout
			  if (!m_timerStarted)
			  {
				  // Start hold for next event
				  //sendOutput(msg.time(), statsOut_out, 6001);
				  holdIn(passive, Time( payoutTime));
				  m_timerStarted = true;
			  }
		  }
		  else
		  {
			  m_tmpAmount += inputAmount;
			  //sendOutput(msg.time(), statsOut_out, 7001);
			  holdIn(passive, nextChange());
		  }
	  }
	  else if (msg.port() == rate_in)
	  {
		  // Update to investment rate in; no other changes
		  double rate = inputAmount;
		  m_rate = inputAmount/100.0;
		  //sendOutput(msg.time(), statsOut_out, 6000);
		  //sendOutput(msg.time(), statsOut_out, inputAmount);
		  //sendOutput(msg.time(), statsOut_out, rate);
		  //sendOutput(msg.time(), statsOut_out, m_rate);
		  holdIn(passive, nextChange());
	  }
	  else if (msg.port() == payout_in)
	  {
		  if (m_state == PayoutPending)
		  {
			  if (msg.value() == m_lastPayoutReq)
			  {
				  // Payout received; restart counting cycle.
				  sendOutput(msg.time(), statsOut_out, 7000);
				  // Internal values reset in outputFunction
				  if (!m_timerStarted)
				  {
					  // schedule next payout transaction
					  //sendOutput(msg.time(), statsOut_out, 2000);
					  holdIn(active, 0);
				  }
				  else
				  {
					  //sendOutput(msg.time(), statsOut_out, 3000);
				  }
			  }
			  else
			  {
				  //sendOutput(msg.time(), statsOut_out, 8000);
				  passivate();
			  }
		  }
	  }
   }
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &InitialPool::internalFunction( const InternalMessage &msg )
{
	if (m_state == Counting)
	{
		if (!m_timerStarted)
		{
			m_timerStarted = true;
			//sendOutput(msg.time(), statsOut_out, m_interval);
			holdIn(passive, Time(0, 0, m_interval, 0));
		}
		else
		{
			passivate();
		}
	}
	else
	{
		//sendOutput(msg.time(), payoutReq_out, 11000);
		passivate();
	}

	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &InitialPool::outputFunction( const InternalMessage &msg )
{

    // Calculate requested withdrawl
	if (m_state == Counting)
	{
		double amount = (m_rate + 1) * m_currentAmount;
		sendOutput(msg.time(), payoutReq_out, amount);
		m_lastPayoutReq = amount;
		sendOutput(msg.time(), totalCount_out, m_totalInvestors);
		m_state = PayoutPending;
		m_timerStarted = false;
	}
	else if (m_state == PayoutPending)
	{
		// Move investors to Long Term
		sendOutput(msg.time(), statsOut_out, 1000);
		sendOutput(msg.time(), longCount_out, m_currentInvestors);
		m_currentInvestors = m_tmpInvestors;
		m_totalInvestors += m_tmpInvestors;
		//sendOutput(msg.time(), statsOut_out, m_currentInvestors);
		m_tmpInvestors = 0;
		sendOutput(msg.time(), longAmount_out, m_currentAmount);
		m_currentAmount = m_tmpAmount;
		//sendOutput(msg.time(), statsOut_out, m_currentAmount);
		//sendOutput(msg.time(), statsOut_out, 7000);
		//sendOutput(msg.time(), statsOut_out, m_totalInvestors);
		m_tmpAmount = 0.0;
		m_state = Counting;
	}
	return *this;
}

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
#include "PonziType.h"           // base header
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator


/*******************************************************************
* Function Name: LongTermPoolType
* Description: constructor
********************************************************************/
PonziType::PonziType( const string &name )
: Atomic( name )
, fund_in(addInputPort("fund_in"))
, principal_out(addOutputPort("principal_out"))
, investorCount_out(addOutputPort("investorCount_out"))
, statsOut_out(addOutputPort("statsOut_out"))
{
    m_initialPayment = 100;
    m_randnumber = 0;
    m_timePeriod = 2*5;
    m_timerStarted = false;
    m_state = Initial;    // Internal state of the model

}

/*******************************************************************
* Function Name: initFunction
********************************************************************/
Model &PonziType::initFunction()
{
	m_state = Recruiting;
	m_timerStarted = true;
	holdIn(passive, Time(0, 0, m_timePeriod, 0));
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: This method executes when an external event is received.
********************************************************************/
Model &PonziType::externalFunction( const ExternalMessage &msg )
{
	// The main input is a report from the bank of whether the bank balance
	// hit 0 or reached the target value for cashing out.

	if (msg.port() == fund_in)
	{
		if (m_state == Recruiting)
		{
			if (msg.value() == 1)
			{
				// Successful
				m_state = Successful;
				sendOutput(msg.time(), statsOut_out, msg.value());
				// Trigger Generate Output
				holdIn(active, 0);
			}
			else if (msg.value() == 0)
			{
				// Fund empty
				m_state = Flee;
				sendOutput(msg.time(), statsOut_out, msg.value());
				passivate();
			}
		}
		else if (m_state == Successful)
		{
			// Final balance reported
			sendOutput(msg.time(), statsOut_out, msg.value());
			passivate();
		}
	}
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
* Description: This method executes when the TA has expired, right after the outputFunction has finished.
* 			   The new state and TA should be set.
********************************************************************/
Model &PonziType::internalFunction( const InternalMessage &msg )
{

	if (m_state == Recruiting)
	{
		holdIn(passive, Time(0, 0, m_timePeriod, 0));
	}
	else if (m_state == Successful || m_state == Flee)
	{
		passivate();
	}

	return *this;

}

/*******************************************************************
* Function Name: outputFunction
* Description: This method executes when the TA has expired. After this method the internalFunction is called.
*              Output values can be send through output ports
********************************************************************/
Model &PonziType::outputFunction( const InternalMessage &msg )
{
	if (m_state == Recruiting)
	{
		sendOutput(msg.time(), principal_out, m_initialPayment);
		sendOutput(msg.time(), investorCount_out, 1);
	}
	else if (m_state == Successful)
	{
		// Send flag to cash out
		sendOutput(msg.time(), principal_out, 0);
	}
	return *this;

}

PonziType::~PonziType()
{
	//TODO: add destruction code here. Free distribution memory, etc.
}

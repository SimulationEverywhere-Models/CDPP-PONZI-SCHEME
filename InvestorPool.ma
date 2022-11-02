[top]
components : ponzi@PonziType fund1@FundType pool
out : totalInvestor_out ponziStatus_out balance_out payout_out longTerm_out payoutReq_out ponziRecruit_out
out : initialPoolStats_out fundStatus_out
in : payoutReq_in  

Link : alert_out@fund1 fund_in@ponzi
Link : principal_out@ponzi payment_in@fund1
Link : statsOut_out@fund1 fundStatus_out
Link : payoutReq_in payReq_in@fund1
Link : payout_out@fund1 payout_out
Link : investorCount_out@ponzi ponziRecruit_out
Link : statsOut_out@ponzi ponziStatus_out
Link : investorCount_out@ponzi count_in@pool
Link : principal_out@ponzi initial_in@pool
Link : rate_out@fund1 rate_in@pool
Link : payoutReq_out@pool payReq_in@fund1
Link : payout_out@fund1 payout_in@pool
Link : totalCount_out@pool totalInvestor_out
Link : longStats_out@pool  longTerm_out
Link : payoutReq_out@pool payoutReq_out
Link : initialStats_out@pool initialPoolStats_out
Link : balance_out@fund1 balance_out
Link : statsOut_out@ponzi control_in@pool
Link : principal_out@pool payment_in@fund1

[InitialPool]
timeSlice : 5
timePeriod : 3
initialInvestment : 100

[LongTermPoolType]
timeSlice : 5
timePeriod : 3

[FundType]
targetBalance : 100000

[pool]
components : initialPool@InitialPool longTermPool@LongTermPoolType
in : count_in initial_in rate_in payout_in control_in
out : payoutReq_out totalCount_out longStats_out initialStats_out principal_out

Link : count_in 	count_in@initialPool
Link : initial_in 	initialAmount_in@initialPool
Link : rate_in 		rate_in@initialPool
Link : payoutReq_out@initialPool 	payoutReq_out
Link : longCount_out@initialPool 	count_in@longTermPool
Link : longAmount_out@initialPool 	amount_in@longTermPool
Link : longAmount_out@initialPool	principal_out
Link : payout_in 	payout_in@initialPool
Link : statsOut_out@initialPool 	initialStats_out
Link : totalCount_out@initialPool 	totalCount_out
Link : payoutReq_out@longTermPool 	payoutReq_out
Link : rate_in 		rate_in@longTermPool
Link : payout_in 	payout_in@longTermPool
Link : statsOut_out@longTermPool 	longStats_out
Link : newPrincipal_out@longTermPool 	initialAmount_in@initialPool
Link : newPrincipal_out@longTermPool   	principal_out
Link : newRecruits_out@longTermPool 	count_in@initialPool 
Link : control_in control_in@longTermPool
Link : control_in control_in@initialPool
[Top]
components : initialPool@InitialPool  
out : payoutReq_out longAmount_out longCount_out stats_out totalCount_out
in : gRate_in amount_in count_in payout_in long_in control_in
Link : gRate_in rate_in@initialPool
Link : amount_in initialAmount_in@initialPool
Link : count_in count_in@initialPool
Link : payout_in payout_in@initialPool
Link : payoutReq_out@initialPool payoutReq_out
Link : longCount_out@initialPool longCount_out
Link : longAmount_out@initialPool longAmount_out
Link : statsOut_out@initialPool stats_out
Link : totalCount_out@initialPool totalCount_out

[InitialPool]
timeSlice : 5
timePeriod : 3
initialInvestment : 100
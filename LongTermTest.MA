[Top]
components : longTermPool@LongTermPoolType  
out : payoutReq_out longAmount_out longCount_out stats_out totalCount_out
in : gRate_in amount_in count_in payout_in long_in control_in
Link : gRate_in rate_in@longTermPool
Link : amount_in amount_in@longTermPool
Link : count_in count_in@longTermPool
Link : payout_in payout_in@longTermPool
Link : payoutReq_out@longTermPool payoutReq_out
Link : newRecruits_out@longTermPool longCount_out
Link : newPrincipal_out@longTermPool longAmount_out
Link : statsOut_out@longTermPool stats_out

[InitialPool]
timeSlice : 5
timePeriod : 3
initialInvestment : 100

[LongTermPoolType]
timeSlice : 5
timePeriod : 3


[Top]
components : fund@FundType  
out :  alert_out balance_out payout_out
in : payoutReq_in  amount_in 

Link : amount_in payment_in@fund
Link : payoutReq_in payReq_in@fund

Link : payout_out@fund payout_out
Link : balance_out@fund balance_out
Link : alert_out@fund alert_out

[InitialPool]
timeSlice : 5
timePeriod : 3
initialInvestment : 100

[LongTermPoolType]
timeSlice : 5
timePeriod : 3

[FundType]
targetBalance : 500
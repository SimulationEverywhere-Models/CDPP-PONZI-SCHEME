[Top]
components : ponzi@PonziType  
out :  principal_out investorCount_out status_out
in : fundState_in   

Link : fundState_in fund_in@ponzi

Link : principal_out@ponzi principal_out
Link : investorCount_out@ponzi investorCount_out
Link : statsOut_out@ponzi status_out

function loss = lossFun(param,data,model,n,K,r,T,S0)
% Calculates loss function
if satisfies_boundary(param)
    param = num2cell(param);
    loss = sum( (data.IVolSurf - getIV(param{:}, model,n,K,S0,r,T)).^2, "all");
else 
    loss = 10^10;
end
end
function y = BisecBSIV(PutCall,S,K,rf,T,a,b,MktPrice,Tol,MaxIter)

% Bisection algorithm
% PutCall = "P"ut or "C"all
% S = Spot price
% K = Strike price
% rf = Risk free rate
% T = Maturity
% a = Starting point lower value of vol (for example, a = 0.001)
% b = Starting point upper value of vol (for example, b = 10);
% MktPrice = Market price
% Tol = Tolerance (for example, Tol = 1e-10)
% MaxIter = maximum number of iterations (for example, MaxIter = 1000)

% Functions for the Black-Scholes call and put  
BSC = @(s,K,rf,v,T) (s.*normcdf((log(s./K) + (rf+v.^2./2).*T)./v./sqrt(T)) - K.*exp(-rf.*T).*normcdf((log(s./K) + (rf+v.^2./2).*T)./v./sqrt(T) - v.*sqrt(T)));
BSP = @(s,K,rf,v,T) (K.*exp(-rf.*T).*normcdf(-(log(s./K) + (rf+v.^2./2).*T)./v./sqrt(T) + v.*sqrt(T)) - s.*normcdf(-(log(s./K) + (rf+v.^2./2).*T)./v./sqrt(T)));
  
if strcmp(PutCall,'C')
	lowCdif  = MktPrice - BSC(S,K,rf,a,T);
	highCdif = MktPrice - BSC(S,K,rf,b,T);
else
	lowCdif  = MktPrice - BSP(S,K,rf,a,T);
	highCdif = MktPrice - BSP(S,K,rf,b,T);
end

if lowCdif*highCdif > 0
	y = -1;
else
	for x=1:MaxIter
		midP = (a+b)/2;
		if strcmp(PutCall,'C')
			midCdif = MktPrice - BSC(S,K,rf,midP,T);
		else
			midCdif = MktPrice - BSP(S,K,rf,midP,T);
		end
		if abs(midCdif)<Tol
			break
		else
			if midCdif>0
				a = midP;
			else
				b = midP;
			end
		end
	end
	y = midP;
end


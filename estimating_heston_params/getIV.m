function IV = getIV(V0, theta, kappa, eta, rho, model,n,K,S0,r,T)
% calculates implied volatility for given parameters
lambda = 0;
kappaQ = kappa + lambda;
thetaQ = kappa*theta/(kappa + lambda);

parameters = {V0, thetaQ, kappaQ, eta, rho};

a = 0.001;
b = 10;
Tol = 1e-10;
MaxIter = 1000;

[m,k] = size(T);
IV = zeros(m,k);
for i = 1:m
    for j = 1:k
        marketPrice = S0.*CallPricingFFT(model, n, ...
    1, K(i,j)./S0, T(i,j), r, 0, ...
    parameters{:});
        IV(i,j) = BisecBSIV('C',S0,K(i,j),r,T(i,j),a,b,marketPrice,Tol,MaxIter);
    end
end
end
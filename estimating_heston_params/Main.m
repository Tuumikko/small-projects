load("empVolatilitySurfaceData.mat")

% Optimization settings
OptSettings.MaxFunEvals = 500;
OptSettings.MaxIter = 200;
OptSettings.TolFun = 1e-10;
OptSettings.TolX = 1e-10;
OptSettings.Display = 'iter';
OptSettings.FunValCheck = 'on';
OptSettings.OutputFcn = @plot_iter;

model = "Heston";

S0 = data.S0;
K = data.K;
r = data.r;
T = data.T;

[K, T] = meshgrid(K,T);

n=13;
a = 0.001;
b = 10;
Tol = 1e-10;
MaxIter = 1000;
s = calibrationSettings();

% initial guess 
param0 = s.parameters0;

fun = @(param) lossFun(param, data, model,n,K,r,T,S0);

% estimate kappa, theta, eta, rho, V0
[param_final, fFinal, exitFlag] = fminsearch(fun, param0, OptSettings);



param_final_cell = num2cell(param_final);

[V0, theta, kappa, eta, rho] = param_final_cell{:}


T = 1;
H = 0.85;
M = 100000;
N = s.tradingDaysInYear;

% price option by simulating
price = priceAsianDownInCallHeston(S0, V0, r, kappa, theta, eta, rho, T, N, M, H);
price

% Hessian numerically
H_numerical = hessian(fun, param_final);

% Standard error
n = numel(data.IVolSurf);
sig2 = sum(fun(param_final)/(n-numel(param_final))); 
se_numerical_H = sqrt(sig2*diag(inv(0.5*H_numerical)));

% Standard error table
tbl = array2table([param_final' se_numerical_H]);
tbl.Properties.RowNames = {'V0'; 'theta'; 'kappa'; 'eta'; 'rho'};
tbl.Properties.VariableNames = {'Parameter estimates','Standard errors'};
tbl
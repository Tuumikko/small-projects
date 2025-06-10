function stop = plot_iter(x, optimValues, state)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
load("empVolatilitySurfaceData.mat")

stop = false;
model = "Heston";
S0 = data.S0;
K = data.K;
r = data.r;
T = data.T;

[K, T] = meshgrid(K,T);

n=13;

params = num2cell(x);
IV = getIV(params{:}, model,n,K,S0,r,T);

figure(1)
surf(K,T,IV)
hold on
surf(K,T,data.IVolSurf)
hold off
end
function condition = satisfies_boundary(params)
% Checks if parameters satisfies boundary conditions
s = calibrationSettings();

min_values = [s.minV0 s.minTheta s.minKappa s.minEta s.minRho];
max_values = [s.maxV0 s.maxTheta s.maxKappa s.maxEta s.maxRho];

condition = all((min_values < params) & (params < max_values));
end
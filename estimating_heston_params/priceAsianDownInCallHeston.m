function price = priceAsianDownInCallHeston(S0, v0, mu, kappa, theta, sigma, rho, T, N, M, H)
% PRICEASIANDOWNINCALLHESTON
%   Prices a down-and-in arithmetic average strike Asian call option
%   under the Heston stochastic volatility model using Monte Carlo,
%   Milstein with truncation, and antithetic variates.

dt = T / N;
payoff = zeros(M, 1);

lambda = 0;
kappaQ = kappa + lambda;
thetaQ = kappa*theta/(kappa + lambda);

for m = 1:M
    % Pre-allocate paths
    S = zeros(N+1, 1); S(1) = S0;
    S_anti = S;
    v = zeros(N+1, 1); v(1) = v0;
    v_anti = v;

    barrier_hit = false;
    barrier_hit_anti = false;

    for t = 2:N+1
        z1 = randn;
        e12 = randn;

        z2 = rho * z1 + sqrt(1 - rho^2) * e12;
        z1_a = -z1;
        e12_a = -e12;
        z2_a = rho * z1_a + sqrt(1 - rho^2) * e12_a;

        % Milstein with truncation for variance
        v_t = v(t-1);
        dW2 = sqrt(dt) * z2;
        v_temp = v_t + kappaQ * (thetaQ - v_t) * dt + sigma * sqrt(v_t) * dW2 + ...
                 0.25 * sigma^2 * (dW2^2 - dt);
        v(t) = max(v_temp, 0);

        v_ta = v_anti(t-1);
        dW2_a = sqrt(dt) * z2_a;
        v_temp_a = v_ta + kappaQ * (thetaQ - v_ta) * dt + sigma * sqrt(v_ta) * dW2_a + ...
                   0.25 * sigma^2 * (dW2_a^2 - dt);
        v_anti(t) = max(v_temp_a, 0);

        % Stock price update
        dW1 = sqrt(dt) * z1;
        dW1_a = sqrt(dt) * z1_a;
        S(t) = S(t-1) * exp((mu - 0.5 * v(t-1)) * dt + sqrt(v(t-1)) * dW1);
        S_anti(t) = S_anti(t-1) * exp((mu - 0.5 * v_anti(t-1)) * dt + sqrt(v_anti(t-1)) * dW1_a);

        if S(t) < H
            barrier_hit = true;
        end
        if S_anti(t) < H
            barrier_hit_anti = true;
        end
    end

    % Arithmetic average
    Savg = mean(S(2:end));
    Savg_anti = mean(S_anti(2:end));

    payoff1 = max(S(end) - Savg, 0) * barrier_hit;
    payoff2 = max(S_anti(end) - Savg_anti, 0) * barrier_hit_anti;

    payoff(m) = 0.5 * (payoff1 + payoff2);
end

price = mean(payoff);  % Money market numeraire => no discounting
fprintf('Asian Down-and-In Call Option Price: %.4f\n', price);
end

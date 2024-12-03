n = 2; % Dimension of xmeasure and H
xmeasure = randn(n, 1); % Random example data
H = sdpvar(n, n, 'symmetric'); % Symmetric decision variable
con_b = [1 xmeasure'; xmeasure H] >= 1e-7 * eye(n + 1);
objective = trace(H); % Example objective

% Solve the problem
diagnostics = optimize(con_b, objective, sdpsettings('solver', 'mosek', 'verbose', 2));

% Check results
if diagnostics.problem == 0
    disp('con_b solved successfully.');
    disp('H:');
    disp(value(H));
else
    disp('con_b failed.');
    disp(diagnostics.info);
end

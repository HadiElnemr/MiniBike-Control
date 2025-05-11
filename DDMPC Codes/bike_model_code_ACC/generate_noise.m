% Set parameters
nSteps = 150;
% nStates = size(A_feedback, 1);

% Model dimension
n = 3;

% Set noise characteristics
epsilon = 10^-6;  % adjust to your needs

% Generate Gaussian noise for each step
% process_noise = process_noise_std * randn(nSteps, nStates);
process_noise = -sqrt(epsilon)/sqrt(n)+2*sqrt(epsilon)/sqrt(n)*rand(n, nSteps);
% Save to file
save('process_noise2.mat', 'process_noise');
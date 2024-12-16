function [u,x,K_lqr] = data_generate(A,B,epsilon,N,x_d0,u_bound)
n = size(A,2);%dimension of state
m = size(B, 2);%dimension of input

%% Generate data using LQR with additive noise
% Q_lqr = [1/100 0 0; 0 1/100 0; 0 0 1/100] ; % Set using Bryson's rule
C = [1 0 0];
% Q_lqr = [100 0 0; 0 100 0; 0 0 100] ; % Set using Bryson's rule
Q_lqr = 0.01 * C' * C; % Set using Bryson's rule
R_lqr = 1.0;
[K_lqr,~,~] = lqr(A, B, Q_lqr, R_lqr);
[K_dlqr,~,~] = dlqr(A, B, Q_lqr, R_lqr);

% display(K_lqr)
%%

u = zeros(m, N);
x = zeros(n, N+1);
x(:,1) = x_d0;
for i = 1:N
    omega = -epsilon/sqrt(n)+2*epsilon/sqrt(n)*rand(n, 1);
    % u(:,i) = -u_bound+2*u_bound*rand(m, 1);
    u(:,i) = - K_lqr * x(:,i);
    % u(:,i) = - K_dlqr * x(:,i);

    x(:,i+1) = A*x(:,i)+B*u(:,i)+omega;
end
u = u(:);
x = x(:);%make matrix to vector

end
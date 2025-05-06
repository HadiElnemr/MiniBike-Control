% This is the code for the paper "Data-driven Min-Max MPC for Linear
% System"
% Toolbox:
% 1. Generate a sequence of data for noisy system
% x_{t+1} = A_s x_t + B_s u_t + w_t, w_t satisfies instantaneous constraint


% clear workspace, close open figures
clear all
close all
clc

%% Parameters of the system

% state dimension and input dimension
n = 2;
m = 1;

% true system matrices
A_s = [0.9749 -0.0135;
    0.0004 0.9888];
B_s = 10^-4*[0.041;5.934];

% constraint on noise
epsilon = 10^-6;

% input constraint and state constraint
u_max = 10;
u_hat = 0;
x_hat = [0;0];
S_u = 0.01;
S_x = [1000 0; 0 500];

%% Generate data with addictive noise

% lenghth of the initial available data
T = 200;

% initial state for data generate
x_d0 = [rand(1); -rand(1)];

% generate the data
[u_g, x_g] = data_generate(A_s,B_s,epsilon,T,x_d0,u_max);

% load('offline_data_1');

%% Simulation

% equilibrium point
x_e = [0 ; 0];
u_e = 0;

% weighting matrices
Q = eye(n);
R = 0.0001;

% Cholesky factorization of the weighting matrix: Q= MQ'*MQ, R= MR'*MR
MQ = chol(Q);
MR = chol(R);

% initial state
x_init = [-0.01;-0.04];

% Number of MPC iterations
mpciterations = 300;

% set options for the solver
option = sdpsettings('solver','mosek','verbose',1,'debug',1)

% option = sdpsettings('solver', 'sedumi', 'sedumi.eps', 1e-8, ...
%                 'sedumi.cg.qprec', 1, 'sedumi.cg.maxiter', 100, ...
%                 'sedumi.stepdif', 2);

% define symbolic decision variables
H = sdpvar(n,n,'symmetric');%symmetric
gamma = sdpvar(1);%objective
L = sdpvar(1,n);%L=FP^{-1}=FH
tau = sdpvar(1,T);

% LMI constraints without input and state constraints
Pi_tau = zeros(2*n+m,2*n+m);
Pi_mu = zeros(2*n+m,2*n+m);
for i=1:T
    M_x = [eye(n) x_g(n*i+1:n*(i+1));
           zeros(n,n) -x_g(n*(i-1)+1:n*i);
           zeros(m,n) -u_g(m*(i-1)+1:m*i)];
    Pi_tau = Pi_tau + tau(1,i)*M_x*blkdiag(epsilon*eye(n),-eye(1))*M_x';
end
con_c = [[-H zeros(n,n+m);zeros(n+m,n) zeros(n+m,n+m)]+Pi_tau [zeros(n,n);H;L] zeros(m+2*n,m+n);
          [zeros(n,n);H;L]' -H  [MR*L;MQ*H]';
          zeros(n+m,m+2*n) [MR*L;MQ*H] -gamma*eye(n+m,n+m)]<=-10^-12*eye(4*n+2*m);
con_d = tau(:)>=10^-12;
con_e = [H L'; L inv(S_u)]>=10^-12*eye(n+m);
con_f = [H H; H inv(S_x)]>=10^-12*eye(2*n);

% initial state
xmeasure = x_init;

% Set variables for output
x = [];
u = [];
gamma_op = [];
H_op = [];
L_op = [];
tau_op = [];
F_op = [];

for ii=1:mpciterations

    t_Start = tic;

    % the constraint [gamma x_t'; x_t H]>=0, change with time
    con_b = [1 xmeasure'; xmeasure H]>=10^-7*eye(n+1);

    % solve the problem with LIM constraints
    LMI = [con_b,con_c,con_d,con_e,con_f];

    % solution
    P = optimizer(LMI,gamma,option,[],{gamma,H,L,tau});
    sol = P();
    t_Elapsed = toc( t_Start );

    % optimal solution
    gamma_star = sol{1};
    H_star = sol{2};
    L_star = sol{3};
    tau_star = sol{4};

    % optimal feedback gain
    F_star = L_star*inv(H_star);

    % store optimization value
    gamma_op = [gamma_op,gamma_star];
    H_op = [H_op,H_star];
    L_op = [L_op;L_star];
    tau_op = [tau_op;tau_star];
    F_op = [F_op;F_star];

    % store closed loop data
    x = [ x, xmeasure ];
    u = [ u, F_star*xmeasure];

    % update closed-loop system (apply first control move to system)
    omega = -sqrt(epsilon)/sqrt(n)+2*sqrt(epsilon)/sqrt(n)*rand(n, 1);
    xmeasure = A_s*xmeasure+B_s*F_star*xmeasure+omega;

    % print numbers
    fprintf(' %3d  | %+11.6f %+11.6f %+11.6f  %+6.3f\n', ii, u(end),...
            x(1,end), x(2,end),t_Elapsed);




end

% plot closed-loop state trajetories
    f1 = figure(1);
    plot(x(1,:),x(2,:),'b'), grid on, hold on,
    plot(x(1,:),x(2,:),'ob'), grid on, hold on,
    E = ellipsoid(x_hat, inv(S_x));
    plot(E,'r');
    xlabel('x(1)');
    ylabel('x(2)');
    drawnow

    % plot input trajetories
    f2 = figure(2);
    plot(u,'g'), grid on, hold on,
    xlabel('t');
    ylabel('u');
    drawnow


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
n = 3; 
m = 1; 

% Bike data
g = 9.8;
h = 0.088; % m (hieght of the centre of mass)
v = 0.634; % m/s or v = 0.634
a = 0.055; % m (distance between rear wheel and centre of gravity projection)
w = 0.167; % m (Distance between front and rear wheels and ground contact points)
lambda =  75/180 * pi; % in rad = 70 degrees  (fork angle)
wheel_radius = 0.0375; % m (diameter is around 7.5cm)
r_tau = wheel_radius * tan(pi/2 - lambda); %(distance between front wheel and intersection of fork with ground)
l = w; % length


A23 = (v^2*h-a*r_tau*g) * sin(lambda) / (h^2*l);

% true system matrices
A_c = [0   1   0;
      g/h  0  A23;
       0   0   0];
B_c = [0 ; 
       a*v*sin(lambda)/(h*l) ; % Try with negative: probably no
       1];

sysc = ss(A_c,B_c, [1 0 0], 0);
sysd = c2d(sysc, 0.01);
A_s = sysd.A;
B_s = sysd.B;

% constraint on noise
epsilon = 10^-6;

% input constraint and state constraint
u_max = 0.01;
u_hat = 0;
x_hat = [0;0;0];
% S_u = 0.01;
% S_x = [10000 0 0; 0 5000 0; 0 0 3000];

S_u = 0.01;
S_x = [10 0 0; 0 5 0; 0 0 3];
% Lean/roll angle max is 40 degrees and motor disconnects after that


%% Generate data with additive noise

% length of the initial available data
T = 15;

% initial state for data generate
% x_d0 = [rand(1); -rand(1)];
x_d0 = [0.001*rand(1); -0.01*rand(1); 0.001*rand(1)]; % Change ()very small

% generate the data
[u_g, x_g, K_lqr] = data_generate(A_s,B_s,epsilon,T,x_d0,u_max);

% load('offline_data_1');

%% Simulation

% equilibrium point
x_e = [0 ; 0 ; 0];
u_e = 0;

% weighting matrices
Q = eye(n);
% R = 10^-4;
R = 1;

% Cholesky factorization of the weighting matrix: Q= MQ'*MQ, R= MR'*MR
MQ = chol(Q);
MR = chol(R);

% initial state 
x_init = [-0.01;-0.04; 0];


% Number of MPC iterations
% mpciterations = 300;
mpciterations = 100;

% set options for the solver
option = sdpsettings('solver','mosek','verbose',2,'debug',1) % ,'mosek.MSK_DPAR_INTPNT_CO_TOL_REL_GAP', 1e-8

% option = sdpsettings('solver', 'sedumi', 'sedumi.eps', 1e-8, ...
%                 'sedumi.cg.qprec', 1, 'sedumi.cg.maxiter', 100, ...
%                 'sedumi.stepdif', 2);

% define symbolic decision variables
H = sdpvar(n,n,'symmetric');%symmetric
gamma = sdpvar(1);%objective
L = sdpvar(1,n);%L=FP^{-1}=FH
% tau_1 = sdpvar(1);
% for i=1:T
%     tau(1,i) = tau_1;       
% end
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
% ?? con_f = [S_x eye(); eye H]>=10^-12*eye(2*n); ??

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
t = [];

for ii=1:mpciterations

    t_Start = tic;

    % the constraint [gamma x_t'; x_t H]>=0, change with time
    con_b = [1 xmeasure'; xmeasure H]>=10^-7*eye(n+1);
    
    % solve the problem with LMI constraints
    LMI = [con_b,con_c,con_d,con_e,con_f];

    % solution
    P = optimizer(LMI,gamma,option,[],{gamma,H,L,tau});
    sol = P();
    t_Elapsed = toc( t_Start ); 
    t=[t;t_Elapsed];

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
    xmeasure = A_s*xmeasure+B_s*F_star*xmeasure;

    % print numbers
    fprintf(' %3d  | %+11.6f %+11.6f %+11.6f  %+6.3f\n', ii, u(end),...
            x(1,end), x(2,end),t_Elapsed);
    
    


end

% plot closed-loop state trajetories    
f1 = figure(1);
plot(x(1,:),x(2,:),'b'), grid on, hold on,
plot(x(1,:),x(2,:),'ob'), grid on, hold on,
% E = ellipsoid(x_hat, inv(S_x));
% plot(E,'r');
xlabel('x(1)');
ylabel('x(2)');
drawnow

% plot input trajetories    
f2 = figure(2);
plot(u,'g'), grid on, hold on,
xlabel('t');
ylabel('u');
drawnow


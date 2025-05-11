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
% lambda =  75/180 * pi; % in rad = 70,75 degrees  (fork angle)
lambda =  90/180 * pi; % in rad = 70,75 degrees  (fork angle)
wheel_radius = 0.0375; % m (diameter is around 7.5cm)
r_tau = wheel_radius * tan(pi/2 - lambda); %(distance between front wheel and intersection of fork with ground)
l = w; % length


A23 = -(v^2*h-a*r_tau*g) * sin(lambda) / (h^2*l);

% true system matrices
A_c = [0   1   0;
      g/h  0  A23;
       0   0   0];
B_c = [0 ;
       - a*v*sin(lambda)/(h*l) ; % Try with negative: yes
       1];

sysc = ss(A_c,B_c, [1 0 0], 0);
Ts = 0.020; % 50 Hz
sysd = c2d(sysc, Ts);
A_s = sysd.A;
B_s = sysd.B;

% constraint on noise
% epsilon = 0.001;
epsilon = 1e-6;

% input constraint and state constraint
u_max = 0.01;
u_hat = 0;
x_hat = [0;0;0];
% S_u = 0.01;
% S_x = [10000 0 0; 0 5000 0; 0 0 3000];

% S_u = 0.01;
% S_x = [10 0 0; 0 5 0; 0 0 3];
% S_x = [5.9e-5??? 0 0; 0 0.01 0; 0 0 3]; % in radians
% S_u = 0.08207;
% S_x = [2.05 0 0; 0 0.32828 0; 0 0 1.62]; % in radians

% IN RADIANS:
S_u = 0.005836;
S_x = [2.6798 0 0; 0 0.32828 0; 0 0 2.05]; % in radians, recalculated

% IN DEGREES:
% S_u = 1.7777777777777777e-06; % in degrees
% S_x = [0.00444 0 0; 0 0.0001 0; 0 0 0.000625]; % in degrees

% Lean/roll angle max is 40 degrees and motor disconnects after that


%% Generate data with additive noise

% length of the initial available data
T = 40;
% T = 4;

% initial state for data generate
% x_d0 = [rand(1); -rand(1)];
% x_d0 = [0.001*rand(1); -0.01*rand(1); 0.001*rand(1)]; % Change ()very small
% x_d0 = [0.5; -0.5; 0.5]; % Change ()very small
x_d0 = [0.5; -0.1; 0.1]; % Change ()very small

% generateep the data
[u_g, x_g, K_lqr] = data_generate(A_s,B_s,epsilon,T,x_d0,u_max);

% load('offline_data_1');
load('process_noise')
%% Simulation

% equilibrium point
x_e = [0 ; 0 ; 0];
u_e = 0;

% weighting matrices
% Q = eye(n);
Q = [300  0    0;
     0   0.1   0;
     0    0   300];

% R = 10^-4;
R = 1;

% Cholesky factorization of the weighting matrix: Q= MQ'*MQ, R= MR'*MR
MQ = chol(Q);
MR = chol(R);

% initial state
% x_init = [-0.01;-0.04; 0];
% x_init = [-0.001;-0.004; 0];
x_init = [0.0873; 0; 0];

% Number of MPC iterations
mpciterations = 150; % Choose for DDMPC
% mpciterations = 1; % Choose for DD State-Feedback controller

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

total_cost = 0;

%% Start MPC iterations
for ii=1:mpciterations

    t_Start = tic;

    % the constraint [gamma x_t'; x_t H]>=0, change with time
    con_b = [1 xmeasure'; xmeasure H]>=10^-7*eye(n+1);

    % solve the problem with LMI constraints
    LMI = [con_b,con_c,con_d,con_e,con_f];
    % LMI = [con_b,con_c,con_d];
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
    
    u_k = F_star*xmeasure;
    total_cost = total_cost + (xmeasure' * Q * xmeasure) + (u_k' * R * u_k);

    % update closed-loop system (apply first control move to system)
    omega_k = process_noise(:,ii);
    xmeasure = A_s*xmeasure+B_s*F_star*xmeasure + omega_k;

    % print numbers
    fprintf(' %3d  | %+11.6f %+11.6f %+11.6f  %+6.3f\n', ii, u(end),...
            x(1,end), x(2,end),t_Elapsed);
end
if mpciterations > 1
    fprintf('Total cost: %.4f\n', total_cost);
end

%% plot closed-loop state trajetories
f1 = figure(1);
plot(x(1,:),x(2,:),'b'), grid on, hold on,
plot(x(1,:),x(2,:),'ob'), grid on, hold on,
plot(x(1,1),x(2,1),'or'), grid on, hold on,
% E = ellipsoid(x_hat, inv(S_x));
% [x, y, z] = ellipsoid(x_hat(1), x_hat(2), x_hat(3), a, b, c);
% plot(x,y,z,'r');
% xlabel('x(1)');
% ylabel('x(2)');
% drawnow

%% Ellipsoid drawing 
%%%%%%%%%%%%%%% Ellipsoid drawing %%%%%%%%%%%%%%%
% center = x_hat;
% Sx_inv = inv(S_x);
% [V, D] = eig(Sx_inv);
% radii = sqrt(diag(D));
% [xe, ye, ze] = ellipsoid(0, 0, 0, radii(1), radii(2), radii(3));
% % Transform to align with eigenvectors and shift to center
% ellipsoid_pts = V * [xe(:)'; ye(:)'; ze(:)'];
% xe_new = reshape(ellipsoid_pts(1,:), size(xe)) + center(1);
% ye_new = reshape(ellipsoid_pts(2,:), size(ye)) + center(2);
% ze_new = reshape(ellipsoid_pts(3,:), size(ze)) + center(3);
% % Plot
% figure(5)
% surf(xe_new, ye_new, ze_new, 'FaceAlpha', 0.3, 'EdgeColor', 'none', 'FaceColor', 'r');
% xlabel('x(1)');
% ylabel('x(2)');
% zlabel('x(3)');
% axis equal
% grid on
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% plot input trajetories
f2 = figure(2);
plot(u,'g'), grid on, hold on,
xlabel('t');
ylabel('u');
drawnow

%%% New to test

%% Analyse Data Driven Controller acquired
% F_star = [-103.459312862395	-9.74002829326616	8.76219082450918]; % Required F_star
% F_star = [0.2131    0.4008   -3.6443];
F_star = - F_star;

fprintf('F_star is [');
fprintf('%g ', F_star);
fprintf(']\n');



x0 = [0.0873; 0; 0]; % bike sims
% x0 = [-0.01;-0.04; 0];
Ts = 0.020; % Treat it as sampling time
t = 0:Ts:3;
u = zeros(size(t));

Tc = ctrb(A_s,B_s);
if (rank(Tc)==3)

    fprintf('This system is controllable! \n');

    % Q matrix
    Q = [300 0 0; 0 0.1 0; 0 0 300];

    % R matrix
    R = 1;

    % Calculate state feedback
    A_feedback = A_s-B_s*F_star;

    if (any(isnan(F_star)))
        fprintf('F_star has NaN values, returning \n');
        close all;
        return;
    end
    if (all(abs(eig(A_s-B_s*F_star)) <= 1))
        fprintf('State Feedback System is stable!: Eigen values are less than or equal to one \n');
        if(any(abs(eig(A_s-B_s*F_star)) == 1))
            fprintf('Lyapunov stable \n')
        end
    else
        fprintf('State Feedback System is Unstable!: Eigen values are larger than one \n');
        close all;
        return;
    end
    f3 = figure(3);
    Tc = ctrb(A_feedback ,B_s);
    % if (rank(Tc)~=3)
    %     fprintf('Feedback system is uncontrollable! \n');

    % y = dlsim(G2,H,C,D,u,x0);
    C = [1 0 0;
         0 0 1];
    D = 0;
    
    if (mpciterations == 1) % If data-driven state-feedback controller
        nSteps = 150;
        nOutputs = size(C, 1);
        y = zeros(nSteps, nOutputs);
    
        % x = x0;
        
        total_cost = 0;
        for k = 1:nSteps
            % Add process noise to state evolution
            omega_k = process_noise(:,k);  % ensure it's a column vector
    
            % Compute output (no output noise here)
            y(k,:) = C * x;
            
            u_k = - F_star * x;
    
            % Add to the total cost
            total_cost = total_cost + (x' * Q * x) + (u_k' * R * u_k);
    
            % Update state with process noise
            x = A_feedback * x + omega_k;
        end
        fprintf('Total cost: %.4f\n', total_cost);


        % y = dlsim(A_feedback,B_s,C,D,u,x0);
        y_theta = y * 180 / pi;
        subplot(2,1,1)
        plot(t(1:end-1), y_theta(:,1),'b.-','LineWidth',1.5);
        xlabel('Time(s)');
        ylabel('\phi (degrees)');
        grid on
        subplot(2,1,2)
        plot(t(1:end-1), y_theta(:,2),'b.-','LineWidth',1.5);
        xlabel('Time (s)');
        ylabel('\delta (degrees)');
        grid on
        % save('DDSFB_output_90_cost=273.1727.mat', 'y_theta');
    else                    % If data-driven MPC
        y_theta = x * 180 / pi;
        subplot(2,1,1)
        plot(t(1:end-1), y_theta(1,:),'b.-','LineWidth',1.5);
        xlabel('Time (s)');
        ylabel('\phi (degrees)');
        grid on
        subplot(2,1,2)
        plot(t(1:end-1), y_theta(3,:),'b.-','LineWidth',1.5);
        xlabel('Time (s)');
        ylabel('\delta (degrees)');
        grid on
        % save('DDMPC_output_75_cost=341.8561.mat', 'y_theta');
    end
end
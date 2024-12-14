% Bike data with no fork
g = 9.8;
h = 0.088; % m (hieght of the centre of mass)
% v = 0.634; % m/s or v = 0.634
v = 4; % m/s or v = 0.634
a = 0.055; % m (distance between rear wheel and centre of gravity projection)
w = 0.167; % m (Distance between front and rear wheels and ground contact points)
lambda =  pi/2; % in rad = 70 degrees  (fork angle)
wheel_radius = 0.0375; % m (diameter is around 7.5cm)
r_tau = wheel_radius * tan(pi/2 - lambda); %(distance between front wheel and intersection of fork with ground)
l = w; % length


A23 = (v^2*h-a*r_tau*g) * sin(lambda) / (h^2*l);

% true system matrices
A_s = [0   1   0;
      g/h  0  v^2/h/w;
       0   0   0];

B_s = [0 ; 
       a*v/(h*l) ;
       1];

sys = ss(A_s, B_s, [1 0 0],0);

pole(sys);

[K, S, P] = lqr(A_s,B_s, eye(3), 1)

sys2 = ss(A_s-B_s*K, B_s, [1 0 0],0);
% sys2 = feedback(sys, K,-1)
step(sys2)
eig(sys2)



% lqr


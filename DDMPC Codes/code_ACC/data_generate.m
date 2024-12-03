function [u,x] = data_generate(A_s,B_s,epsilon,N,x_d0,u_bound)
n = size(A_s,2);%dimension of state
m = size(B_s, 2);%dimension of input

u = zeros(m, N);
x = zeros(n, N+1);
x(:,1) = x_d0;
for i = 1:N
    omega = -epsilon/sqrt(n)+2*epsilon/sqrt(n)*rand(n, 1);
    u(:,i) = -u_bound+2*u_bound*rand(m, 1);
    x(:,i+1) = A_s*x(:,i)+B_s*u(:,i)+omega;
end
u = u(:);
x = x(:);%make matrix to vector

end
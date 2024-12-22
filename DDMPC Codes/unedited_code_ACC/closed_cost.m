load('result_R10-4.mat')

cost = 0;
for i=1:300
    stage_cost(i) = x(:,i)'*Q*x(:,i)+u(:,i)'*R*u(:,i);
    cost = cost + stage_cost(i);
end

mean(t)

function [epsilon] = calculate_epsilon(sysd, x_g, u_g)
%calculate_epsilon Given the discretised system, x_g, and u_g collected
%data,ouput epsilon.
%  calculate epsilon as max error using norm 2
%  epsilon = max(||x_g - sysd.A*x_g - sysd.B*u_g||)

N = size(x_g,1)/3;
epsilon = 0;
for i = 2:N
    current_state = x_g(3*(i-1)+1:3*(i-1)+3);
    previous_state = x_g(3*(i-2)+1:3*(i-2)+3);

    % predict = sysd.A*x_g(3*(i-2)+1:3*(i-2)+3) + sysd.B*u_g(i-1);
    predict = sysd.A * previous_state + sysd.B * u_g(i-1);
    % error = x_g(i+1) - sysd.A*x_g(3*(i-2)+1:3*(i-2)+3) - sysd.B*u_g(i-1);
    error = predict - current_state;
    epsilon = max(norm(error, 2), epsilon);
end

display(epsilon);

% % 1. Calculate the error
% error = x_g - sysd.A*x_g - sysd.B*u_g;
%
% % 2. Calculate the norm 2 of the error
% epsilon = norm(error, 2);

end
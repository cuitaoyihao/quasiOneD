function[f] = Contourf(x)
%Objective function
y=Contoury(x);
f=-20*exp(-(x(1)-1)^2-0.25*(x(2)-1)^2)+y(1)+cos(y(2));

end


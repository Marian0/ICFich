1;

function y=tempActual( t_anterior , t_exterior, i, v, a)
	if (a == 0)
		y = 0.912*t_anterior + 0.088 * t_exterior + 0.604 * i^2 - 0.121 * 10^(-1) * v; 
	else
		y = 0.169*t_anterior + 0.813 * t_exterior + 0.112 * i^2 - 0.002 * v; 
	end
	
endfunction
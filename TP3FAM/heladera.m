1;

function y=heladera_apagado(x)
	if ( x < 0)
		y = 1;
	else if (x >= 0 && x < 1562.5)
		y = -6.4*10^-4 * x + 1;
	else
		y = 0;
	end
endfunction


function y=heladera_medio(x)
	if ( x < 2500 && x > 937.5)
		y = 6.4*10^-4 * x -0.6;
	else if (x >= 2500 && x < 4062.5)
		y = -6.4*10^-4 * x + 2.6;
	else
		y = 0;
	end
endfunction


function y=heladera_medio(x)
	if ( x < 5000 && x > 3437.5)
		y = 6.4*10^-4 * x - 2.2;
	else if (x > 5000 )
		y = 1;
	else
		y = 0;
	end
endfunction
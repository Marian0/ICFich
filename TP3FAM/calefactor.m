1;

function y=calefactor_apagado(x)
	if ( x < -0.7)
		y = 1;
	else if (x >= 0.7 && x < 0.5)
		y = -5/6 * x + 5/12;
	else
		y = 0;
	end
endfunction


function y=calefactor_minimo(x)
	if ( x < 1.1 && x > -0.1 )
		y = 5/6 * x + 1/12;
	else if (x >= 1.1 && x < 2.3)
		y = -5/6 * x + 23/12;
	else
		y = 0;
	end
endfunction


function y=calefactor_medio(x)
	if ( x < 2.9 && x > 1.7 )
		y = 5/6 * x - 17/12;
	else if (x >= 2.9 && x < 4.1)
		y = -5/6 * x + 41/12;
	else
		y = 0;
	end
endfunction


function y=calefactor_maximo(x)
	if ( x < 4.7 && x > 3.5 )
		y = 5/6 * x - 35/12;
	else if (x >= 4.7)
		y = 1;
	else
		y = 0;
	end
endfunction
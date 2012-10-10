1;

function y=tempActual( t_anterior , t_exterior, i, v, a)
	if (a == 0)
		y = 0.912*t_anterior + 0.088 * t_exterior + 0.604 * i^2 - 0.121 * 10^(-1) * v; 
	else
		y = 0.169*t_anterior + 0.813 * t_exterior + 0.112 * i^2 - 0.002 * v; 
	end
	
endfunction


function y=evaluarTrapecio( coordenadas, x )
	if (length(coordenadas) < 4)
		disp("evaluarTrapecio cantidad de coordenadas inválida");
	end

	if (coordenadas(2) == coordenadas(3))
		y = evaluarTriangulo( [coordenadas(1) coordenadas(2) coordenadas(4)], x);
		return;
	end

	if (x > coordenadas(1) && x <= coordenadas(2))
		y = (x - coordenadas(2)) / (coordenadas(3) - coordenadas(2));
	elseif (x > coordenadas(2) && x <= coordenadas(3) )
		y = 1;
	elseif (x > coordenadas(3) && x <= coordenadas(4) )
		y = ( x + coordenadas(3) ) / ( coordenadas(4) - coordenadas(3) ) - 1;
	else
		y = 0;
	end
endfunction


function y=evaluarTriangulo( coordenadas, x )
	if (length(coordenadas) < 3)
		disp("evaluarTrapecio cantidad de coordenadas inválida");
	end

	if (x > coordenadas(1) && x <= coordenadas(2))
		y = (x - coordenadas(1)) / (coordenadas(2) - coordenadas(1));
	elseif (x > coordenadas(2) && x <= coordenadas(3) )
		y = ( -x + coordenadas(2) ) / ( coordenadas(3) - coordenadas(2) ) + 1;
	else
		y = 0;
	end
endfunction


function coordenadas=obtenerCoordenadasTrapecio( centro, base_mayor, base_menor )
	coordenadas(1) = centro - base_mayor /2;
	coordenadas(2) = centro - base_menor /2;
	coordenadas(3) = centro + base_menor /2;
	coordenadas(4) = centro + base_mayor /2;
endfunction


function coordenadas=obtenerCoordenadasTriangulo( centro, base_mayor )
	coordenadas(1) = centro - base_mayor /2;
	coordenadas(2) = centro;
	coordenadas(3) = centro + base_menor /2;
endfunction


function area=areaTrapecio(coordenadas) 
	if (length(coordenadas) < 4)
		disp("areaTrapecio cantidad de coordenadas inválida");
	end

	base_mayor = abs(coordenadas(4) - coordenadas(1));
	base_menor = abs(coordenadas(3) - coordenadas(2));

	area = (base_mayor + base_menor) / 2;
endfunction


function area=areaTriangulo(coordenadas) 
	if (length(coordenadas) < 3)
		disp("areaTriangulo cantidad de coordenadas inválida");
	end

	base = coordenadas(3) - coordenadas(1);

	area = base / 2;
endfunction


function centroide=centroideTrapecio(coordenadas)
	if (length(coordenadas) < 4)
		disp("centroideTrapecio cantidad de coordenadas inválida");
	end

	centroide = abs(coordenadas(4) - coordenadas(1)) / 2;
	
endfunction


%Devuelve un centroide para los dos trapecios indicados como coordenadas.
function centroide=centroideTrapecios(coordenadas1 , coordenadas2)
	if (length(coordenadas1) < 4 || length(coordenadas2) < 4)
		disp("centroideTrapecios cantidad de coordenadas inválida");
	end

	area1 = areaTrapecio(coordenadas1);
	area2 = areaTrapecio(coordenadas2);

	%Centroides en x
	c1 = (coordenadas1(4) - coordenadas1(1)) / 2;
	c2 = (coordenadas2(4) - coordenadas2(1)) / 2;

	centroide = (c1*area1 + c2*area2) / (area1 + area2);
endfunction
1;

source("funciones.m");

%Generamos los conjuntos

%Entrada
%conjuntos_T es una matriz que tiene los valores de cada uno de los trapecios
%conjuntos_T = [ MC; C; N; F; MF];
conjuntos_T = [  [-15 -10 -10 -7 ]  ;  [ -8 -5 -5 -2 ]  ;  [-3 0 0 3]  ;  [2 5 5 8]  ;  [7 10 10 15] ];
conjuntos_T = [  [-17 -10 -10 -5 ]  ;  [ -10 -5 -5 -0 ]  ;  [-5 0 0 5]  ;  [0 5 5 10]  ;  [5 10 10 17] ];
cantidad_conjuntos_T = size(conjuntos_T, 1);
cantidad_conjuntos_calor = floor(cantidad_conjuntos_T/2);
cantidad_conjuntos_frio = cantidad_conjuntos_calor;

%Salidas
%Heladera
%conjuntos_H = [Ap; Mi; Me; Ma];
conjuntos_H = [  [-5000 4999 0 1562.5]  ;  [937.5 2500 2500 4062.5]   ;  [3437.5 5000 10000 10001]  ];

%Calefactor
%conjuntos_C = [Ap; Mi; Me; Ma];
conjuntos_C = [  [-1000 -999 -0.7 0.5 ]  ;  [-0.1 1.1 1.1 2.3]  ;  [1.7 2.9 2.9 4.1]  ;  [3.5 4.7 1000 1001]  ];


%Reglas: (DT; Heladera, Calefactor)
%DT = 0: Normal, no hago nada
% (Normal; Apagado, Apagado)

%DT<0: calor -> aumento frio
% (Calor; Medio, Apagado)
% (MuchoCalor; Maximo, Apagado)

%DT>0: frio -> aumento calefactor
% (Frio; Apagado, Medio);
% (MuchoFrio; Apagado; Maximo)


%Generamos las temperaturas deseadas
x18 = 18*ones(1,60);
x22 = 22*ones(1,60);

Tdeseada = [x18 x22 x18 x22 x18 x22];

%Generamos las temperaturas externas
Te = [22*ones(1,60) 15*ones(1,60) 22*ones(1,60) 10*ones(1,60) 22*ones(1,60) 25*ones(1,60)];

%Generamos los intervalos donde se va a abrir la puerta (es aleatorio)
apertura_puerta = zeros(1,360);
for i=1:360
    num = rand;
    if (num < 1/360)
        apertura_puerta(i) = 1;
    else
        apertura_puerta(i) = 0;
    end
end

%inicializamos la temperatura interior
Ti = zeros(1,361);
Ti(1) = 20;
V = [];
I = [];
%bucle principal
for w=1:360
    %comprueba si la puerta esta abierta y calcula el DT
    if (apertura_puerta(w) == 0)
        DT = Tdeseada(w) - 0.912*Ti(w) - 0.088*Te(w);
    else
        DT = Tdeseada(w) - 0.169*Ti(w) - 0.831*Te(w);
    end
    
    %Convertir DT a conjunto borroso
    %Para cada conjunto de T_, calculo su membresia
    membresias = zeros(1,cantidad_conjuntos_T);
    for i=1:cantidad_conjuntos_T
        membresias(i) = evaluarTrapecio(conjuntos_T(i,:), DT);
    end
   
    %se guardaran los trapecios activados y su membresia:
    % [trapecio_1, membresia_1; trapecio_2, membresia_2]
    trapecios_activados = [];
 
    %hace calor y debo aumentar heladera 
    if (DT < 0)
        %se recorren todos los que pertenecen a calor y guardo los trapecios y sus activaciones
        
        %se activa la regla de mucho calor
        if (membresias(1) > 0) 
             trapecios_activados = [trapecios_activados; conjuntos_T(1,:) membresias(1)];
        end
        
        %se activa la regla de calor
        if (membresias(2) > 0) 
             trapecios_activados = [trapecios_activados; conjuntos_T(2,:) membresias(2)];
        end
    end
    
    %hace frio y debo aumentar calefactor
    if (DT > 0)
        %se recorren todos los que pertenecen a frio y guardo los trapecios y sus activaciones
        
        %se activa la regla de frio
        if (membresias(4) > 0) 
             trapecios_activados = [trapecios_activados; conjuntos_T(4,:) membresias(4)];
        end
        
        %se activa la regla de mucho frio
        if (membresias(5) > 0) 
             trapecios_activados = [trapecios_activados; conjuntos_T(5,:) membresias(5)];
        end
    end
   
    %calcular centroide de trapecios
    nactivados = size(trapecios_activados,1);

    nuevo_v = 0; %no le pongo heladera
    nuevo_i = 0; %no le pongo calefactor
    if (DT < 0) %actualizaremos la heladera

        if (nactivados == 2) %Cantidad de Filas
            nuevo_v = centroideTrapecios( trapecios_activados(1,:), trapecios_activados(2,:) );
        elseif (nactivados == 1)
            nuevo_v = centroideTrapecio(trapecios_activados(1,:));
        else
            nuevo_v = 0;
        end
    elseif (DT > 0) %actualizaremos el calefactor
        if (nactivados == 2) %Cantidad de Filas
            nuevo_i = centroideTrapecios( trapecios_activados(1,:), trapecios_activados(2,:) );
        elseif (nactivados == 1)
            nuevo_i = centroideTrapecio(trapecios_activados(1,:));
        else
            nuevo_i = 0;
        end
    end

    V = [V nuevo_v];
    I = [I nuevo_i];



    if (apertura_puerta(w) == 0) %no esta abierta la puerta
        Tnueva = 0.912*Ti(w) + 0.088*Te(w) + 0.604*nuevo_i*nuevo_i - 0.0121*nuevo_v;
    else
        Tnueva = 0.169*Ti(w) + 0.831*Te(w) + 0.112*nuevo_i*nuevo_i - 0.002*nuevo_v;
    end


    %Notar que se asigna Ti(w+1) para la iteracion siguiente
    Ti(w+1) = Tnueva; 
end

hold on;
figure(2);
plot(V, 'b');
plot(I, 'r');

legend('Heladera', 'Calefactor');
%graficar y demases
hold on;

figure(1);
plot(Ti, 'r');
plot(Tdeseada, 'b');
legend('Temp Actual', 'Temp Deseada');




pause; %VERY IMPORTANT

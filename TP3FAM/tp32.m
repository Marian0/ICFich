source("funciones.m");

%Generamos los conjuntos

%Entrada
%conjuntos_T es una matriz que tiene los valores de cada uno de los trapecios
%conjuntos_T = [Sof; MC; C; N; F; MF; Cong];
%conjuntos_T = [  [-12 -12 -7 -6 ] ; [ -7 -5 -5 -3 ] ; [-4 -3 -3 -1] ; [-1.5 0 0 2] ; [1 3 3 4] ; [3 5 5 7] ; [6 7 10 10] ]; 

conjuntos_T = [ [-14 -12 -9 -8] ; [-9 -8 -6 -5] ; [-6 -5 -2 -1] ; [-2.5 -1 1 2.5] ; [1 2 5 6] ; [5 6 8 9] ; [8 9 12 14] ];

cantidad_conjuntos_T = size(conjuntos_T, 1);

%Salidas
%Heladera
%conjuntos_H = [Mi; Me; Ma];
%conjuntos_H = 1.45.*[ [0 40 40 125] ; [60 130 130 280] ; [ 200 350 350 600] ];
%conjuntos_H = [ [0 120 120 170] ; [90 180 180 300] ; [ 210 330 340 340] ];
conjuntos_H = [ [0 0 100 160]; [100 160 220 280] ; [220 280 340 340] ]; 

%Calefactor
%conjuntos_C = [Mi; Me; Ma];
%conjuntos_C = 1.85.*[  [0.0 0.8 0.8 1.7] ; [1.3 2.3 2.3 3.3] ; [2.7 3.7 3.7 4.7] ];
%conjuntos_C =  [  [0.0 1.6 1.6 2.8] ; [2.5 3.0 3.0 4.0] ; [4 5 10 10] ];
conjuntos_C =  [ [0 0 0.08 0.14] ; [0.08 0.14 0.22 0.28] ; [0.22 0.28 0.34 0.34] ]; 

%Reglas: (DT; Heladera, Calefactor)
%DT = 0: Normal, no hago nada
% (Normal; Apagado, Apagado)

%DT<0: calor -> aumento frio
% (Calor;       Minimo, Apagado)
% (MuchoCalor;  Medio,  Apagado)
% (Sofocante;   Maximo, Apagado)

%DT>0: frio -> aumento calefactor
% (Frio;        Apagado, Minimo);
% (MuchoFrio;   Apagado, Medio)
% (Congelante;  Apagado, Maximo)


%Generamos las temperaturas deseadas
x18 = 18*ones(1,60);
x22 = 22*ones(1,60);

Tdeseada = [x18 x22 x18 x22 x18 x22];

%Generamos las temperaturas externas
Te = [22*ones(1,60) 15*ones(1,60) 22*ones(1,60) 10*ones(1,60) 22*ones(1,60) 25*ones(1,60)];

%Generamos los intervalos donde se va a abrir la puerta (es aleatorio)
apertura_puerta = zeros(1,360);
dibuja_puerta = zeros(1,360);
for i=1:360
    num = rand;
    if (num < 1/360)
        apertura_puerta(i) = 1;
        dibuja_puerta(i) = 25;
    end
end

%inicializamos la temperatura interior
Ti = zeros(1,360);
Ti(1) = 20;
V = [];
I = [];

DThistory = [];
%bucle principal
for w=2:360
    DT = Tdeseada(w) - Ti(w-1);
    
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
        
        %se activa la regla de sofocante
        if (membresias(1) > 0) 
             trapecios_activados = [trapecios_activados; conjuntos_H(3,:) membresias(1)];
        end
        
        %se activa la regla de mucho calor
        if (membresias(2) > 0) 
             trapecios_activados = [trapecios_activados; conjuntos_H(2,:) membresias(2)];
        end
        
        %se activa la regla de calor
        if (membresias(3) > 0) 
             trapecios_activados = [trapecios_activados; conjuntos_H(1,:) membresias(3)];
        end
    end
    
    %hace frio y debo aumentar calefactor
    if (DT > 0)
        %se recorren todos los que pertenecen a frio y guardo los trapecios y sus activaciones
        
              
       %se activa la regla de frio
       if (membresias(5) > 0) 
            trapecios_activados = [trapecios_activados; conjuntos_C(1,:) membresias(5)];
       end
       
       %se activa la regla de mucho frio
       if (membresias(6) > 0) 
            trapecios_activados = [trapecios_activados; conjuntos_C(2,:) membresias(6)];
       end
      
       %se activa la regla de congelante
       if (membresias(7) > 0) 
            trapecios_activados = [trapecios_activados; conjuntos_C(3,:) membresias(7)];
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

    %trampa no borrosa
    % if (abs(DT) < 1.5)
    %     nuevo_i = nuevo_i*0.85;
    %     nuevo_v = nuevo_v*0.85;
    % end
    
    V = [V nuevo_v];
    I = [I nuevo_i];

    if (apertura_puerta(w) == 0) %no esta abierta la puerta
        Tnueva = 0.912*Ti(w-1) + 0.088*Te(w) + 0.604*nuevo_i*nuevo_i - 0.0121*nuevo_v;
    else
        Tnueva = 0.169*Ti(w-1) + 0.831*Te(w) + 0.112*nuevo_i*nuevo_i - 0.002*nuevo_v;
    end

    DThistory = [DThistory DT];
    Ti(w) = Tnueva; 
end

figure(2);
hold on;
plot(Ti, 'r');
plot(Tdeseada, 'b');
plot(Te, 'k');
stem(dibuja_puerta, 'g');
stem(DThistory, 'k');
legend('Temp Actual', 'Temp Deseada', 'Temp Externa', 'Apertura Puerta', 'DeltaTemperatura');
title('Temperatura de la habitacion');
xlabel('Tiempo');
ylabel('Temperatura');


%Graficacion de trapecios
%  figure(3);
%  hold on;
%  dibujaTrapecio(conjuntos_T(1,:),1);
%  dibujaTrapecio(conjuntos_T(2,:),3);
%  dibujaTrapecio(conjuntos_T(3,:),1);
%  dibujaTrapecio(conjuntos_T(4,:),3);
%  dibujaTrapecio(conjuntos_T(5,:),1);
%  dibujaTrapecio(conjuntos_T(6,:),3);
%  dibujaTrapecio(conjuntos_T(7,:),1);
%  title('Conjuntos Delta Temperatura');

%  figure(4);
%  hold on;
%  dibujaTrapecio(conjuntos_H(1,:),1);
%  dibujaTrapecio(conjuntos_H(2,:),3);
%  dibujaTrapecio(conjuntos_H(3,:),1);
%  title('Conjuntos Aire Acondicionado');

%  figure(5);
%  hold on;
%  dibujaTrapecio(conjuntos_C(1,:),1);
%  dibujaTrapecio(conjuntos_C(2,:),3);
%  dibujaTrapecio(conjuntos_C(3,:),1);
%  title('Conjuntos Calefactor');
pause; %VERY IMPORTANT

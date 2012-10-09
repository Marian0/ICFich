1;

#Generamos los conjuntos

#Entrada
T_MC = generarMC();
T_C = generarC();
T_N = generarN();
T_F = generarF();
T_MF = generarMF();

#Salidas
#Heladera
H_Ap = generarHA();
H_Me = generarHMe();
H_Ma = generarHma();
#Calefactor
C_Ap = generarCA();
C_Mi = generarCMi();
C_Me = generarCMe();
C_Ma = generarCma();

#Generamos las reglas

#Reglas: (DT; Heladera, Calefactor)
#DT = 0: Normal, no hago nada
# (Normal; Apagado, Apagado)

#DT<0: calor -> aumento frio
# (Calor; Medio, Apagado)
# (MuchoCalor; Maximo, Apagado)

#DT>0: frio -> aumento calefactor
# (Frio; Apagado, Medio);
# (MuchoFrio; Apagado; Maximo)

#Obtenemos las matrices M que identifican a las reglas
C_Me_Ap  = codificacionCorrelacionProducto(T_C, H_Me);
MC_Ma_Ap = codificacionCorrelacionProducto(T_MC, H_Ma);
F_Ap_Me  = codificacionCorrelacionProducto(T_F, C_Me);
MF_Ap_Ma = codificacionCorrelacionProducto(T_MF, C_Ma);






#Generamos las temperaturas deseadas
x18 = 18*ones(1,60);
x22 = 22*ones(1,60);

Tdeseada = [x18 x22 x18 x22 x18 x22];

#Generamos las temperaturas externas
Te = [22*ones(1,60) 15*ones(1,60) 22*ones(1,60) 10*ones(1,60) 22*ones(1,60) 25*ones(1,60)];

#Generamos los intervalos donde se va a abrir la puerta (es aleatorio)
apertura_puerta = zeros(1,360);
for i=1:360
    num = randn;
    if (num > 0.5)
        apertura_puerta(i) = 1;
    else
        apertura_puerta(i) = 0;
    end
end
apertura_puerta

#inicializamos la temperatura interior
Ti = zeros(1,361);
Ti(1) = 20;

#bucle principal
for w=1:360
    #comprueba si la puerta esta abierta y calcula el DT
    if (apertura_puerta(w) == 0)
        DT = Tdeseada(w) - 0.912*Ti(w) - 0.088*Te(w);
    else
        DT = Tdeseada(w) - 0.169*Ti(w) - 0.831*Te(w);
    end
    #convertir DT a conjunto borroso

    if (DT < 0) #Hace calor y debo aumentar heladera

    end

    if (DT > 0) #hace frio y debo aumentar calefactor

    end
    

    #Notar que se asigna Ti(w+1) para la iteracion siguiente
    Ti(w+1) = Tnueva; 
end

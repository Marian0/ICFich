function [M] = codificacionCorrelacionMinimo(A, B)
    n = size(A,2);
    m = size(B,2);
    M = zeros(n,m);
    for i=1:n
        for j=1:m
            M(i,j) = min([A(i) B(j)]);
        end
    end
endfunction

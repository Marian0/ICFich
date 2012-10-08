function [B] = correlacionMinimo(A,M)
    n = size(A,2);
    m = size(M,2);
    B = zeros(1,m);
    for j=1:m
        B(j) = max(min(A(:),M(:,j)));
    end
endfunction


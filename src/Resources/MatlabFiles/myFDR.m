%% The Benjamini & Hochberch (1995) False Discovery Rate (FDR) procedure
function [P] = myFDR(p)

cV = 1;
Ps = sort(p);
S = length(Ps);
Qs = Ps*S./(1:S)'*cV;
P = zeros( size(p) );
for i = 1:numel(p)
    I = find( Ps >= p(i), 1 );
    if isempty(I)
        P(i) = 1;
    else
        P(i) = min(Qs(I:S));
    end
end

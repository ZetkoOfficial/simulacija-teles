# Simulacija problema n-teles
## Uvod
Za del projekta želimo simulirati gibanje $n$ nebesnih teles pod vplivom gravitacijske sile med njimi. Tu bomo razložili matematiko v ozadju našega programa.

## Definicije in ideja
Nebesna telesa bomo predstavili kot $T_i = (M_i, \vec{x_i},\vec{v_i})$, kjer $M_i$ predstavlja maso telesa, $\vec{x_i},\vec{v_i}$ pa sta vektorja lege in hitrosti telesa. Če vemo v nekem trenutku vse $T_i$ potem lahko teoretično natančno predvidemo kaj se bo zgodilo v prihodnosti.

Simulacijo bomo izpeljali numerično z uporabo Eulerjeve metode. Namreč za $T_i$ veljata v nekem trenutku diferencialni enačbi $d\vec{v_i} = \frac{\vec{F}}{M_i} dt$ ter $d\vec{x_i} = \vec{v_i}dt$, kjer je $F$ rezultanta sil na to telo. Po Eulerjevi metodi za majhen $dt$ velja za telo: $$T_i' = (M_i,\vec{x_i}+d\vec{x_i}, \vec{v_i}+d\vec{v_i})$$

Za uspešno simulacijo potrebujemo le nekako učinkovito za vsako telo izračunati $\vec{F}$.

## Implementacija
Za izračun $\vec{F}$ za telo $T_i$ je dovolj sešteti vse gravitacijske sile med ostalimi $n-1$ telesi. Torej $\vec{F} = \sum^{j \neq i}_{j} \vec{F_{i,j}}$, kjer je $$\vec{F_{i,j}} = G \frac{M_i M_j}{|r_{i,j}|^3}  \vec{r_{i,j}}$$
$\vec{r_{i,j}}$ pa je vektor ki telo $T_i$ povezuje z $T_j$. Opazimo da je dovolj $\frac{\vec{r_{i,j}}}{|r_{i,j}|^3}$ izračunati le za vse pare $(i,j)$, kjer je $i < j \leq n$. Ker je računanje tega drago te vrednosti poračunajmo le enkrat in jih shranimo v tem vrstnem redu v matriko $R$.

Vsakemu telesu priredimo torej pred simulacijo še en *sparse vektor* (vektor z veliko ničlami) $\vec{m_i}$, kjer bo $\vec{m_i}$ imel v mestih $j$ kjer se v stolpcu matrike $R$ nahaja $r_{i,j}$ vrednost $M_j$ in v stolpcih kjer se nahaja $r_{j,i}$ vrednosti $-M_j$. 

Za vsak $T_i$ bo torej veljalo enostavno $d\vec{v_i} = G \cdot R\vec{m_i} \cdot dt$.

## Poraba prostora in težave
Trenutna implementacija porabi v teoriji za $n$ teles okoli $O(n^2)$ spomina torej za $n \leq 5000$ okoli 500MB. Trenutna implementacija je tudi zelo občutljiva ko sta si telesi blizu in zato v teh primerih pride do kaotičnega gibanja delecv. 
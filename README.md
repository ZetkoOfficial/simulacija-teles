# Simulacija teles
Projektna naloga za predmet UVP v 1. letniku FMF v kateri pridobimo podatke o nebesnih telesih,
naršemo grafike za njih in jih simuliramo v 3d prostoru in simulacijo vizualno prikažemo.

Pozor: iskalni ključi za nebesna telesa morajo biti zapisani v angleškem jeziku(npr. Earth, Mars, Moon). 

## Dokumentacija za manjše dele projekta
- [Horizon API](dokumentacija/01_horizon_api.md)
- [Simulacija problema n-teles](dokumentacija/02_simulacija.md)

## Namestitev
Za namestitev na sistemih Linux(testirano: Ubuntu 20.04.6 LTS) potrebujete:
- **C++ compiler** (testirano: g++ 9.4.0)
- **Eigen3 za C++** (libeigen3-dev)
- **python3** (testirano: Python 3.8.10)
- sposobnost ustvarjanja **python virtualnih okolij**

V direktoriji projekta ustvarite virtualno python okolje z imenom .venv in ga aktivirajte v terminalu:
```
python3 -m venv .venv
source .venv/bin/activate
```
Nato za prvo namestitev poženite makefile z pravilom `setup`:
```
make setup
```
Podobno pa lahko za odmestitev uporabite pravilo `remove` in nato virtualno okolje deaktivirate z bash ukazom `deactivate`.

## Zagon aplikacije
Aplikacijo zaženete tako da poženete makefile z pravilom `jupyter` in v notebooku izberite jedro `simulacija_teles_venv`, če že ni izbrano:
```
make jupyter
```


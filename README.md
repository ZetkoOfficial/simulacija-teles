# Simulacija teles
Projektna naloga za predmet UVP v 1. letniku FMF v kateri pridobimo podatke o nebesnih telesih,
naršemo grafike za njih in jih simuliramo v 3d prostoru in simulacijo vizualno prikažemo.

## Dokumentacija za manjše dele projekta
- [Horizon API](dokumentacija/01_horizon_api.md)
- [Simulacija problema n-teles](dokumentacija/02_simulacija.md)

## Namestitev
V direktoriji projekta ustvarite virtualno python okolje z imenom .venv in ga aktivirajte v terminalu:
```
python3 -m venv .venv
source .venv/bin/activate
```
Nato za prvo namestitev poženite makefile z pravilom `setup`:
```
make setup
```
Podobno pa lahko za odmestitev virtualno okolje deaktivirate in uporabite pravilo `remove`.

## Zagon aplikacije
Aplikacijo zaženete tako da poženete makefile z pravilom `jupyter` in v notebooku izberite jedro `simulacija_teles_venv`:
```
make jupyter
```


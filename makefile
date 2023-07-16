CXX=g++
CXXFLAGS=-O3 -fopenmp -march=native -fno-math-errno -DNDEBUG
PYC=python3
PIP=pip3

simulacija: simulacija.cpp
	@echo "| Prevajanje programa za simulacije..."
	@$(CXX) $(CXXFLAGS) simulacija.cpp -o simulacija
	@echo "|--> končano"

jupyter: simulacija
	@echo "| Zagon jupyter client-a na portu 8888"
	@jupyter notebook --port 8888 --no-browser

setup:
	@echo "| Ustvarjanje potrebnih map"
	@mkdir -p telesa
	@echo "|--> končano"
	@echo "| Namestitev potrebnih knjižnic"
	@$(PIP) install -r requirements.txt
	@echo "|--> končano"
	@echo "| Namestitev jupyter jedra"
	@$(PYC) -m ipykernel install --user --name=simulacija_teles_venv
	@echo "|--> končano"

remove:
	@echo "| Odstranitev jupyter jedra"
	@jupyter kernelspec remove simulacija_teles_venv
	@echo "|--> končano"

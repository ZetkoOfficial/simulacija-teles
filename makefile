simulacija: simulacija.cpp
	@echo "| Prevajanje programa za simulacije..."
	@g++ -O3 -fopenmp -march=native -fno-math-errno -DNDEBUG simulacija.cpp -o simulacija
	@echo "|--> končano"

jupyter: simulacija
	@echo "| Zagon jupyter client-a na portu 8888"
	@jupyter notebook --port 8888 --no-browser

setup:
	@echo "| Namestitev potrebnih knjižnic"
	@pip3 install -r requirements.txt
	@echo "|--> končano"
	@echo "| Namestitev jupyter jedra"
	@python3 -m ipykernel install --user --name=simulacija_teles_venv
	@echo "|--> končano"

remove:
	@echo "| Odstranitev jupyter jedra"
	@jupyter kernelspec remove simulacija_teles_venv
	@echo "|--> končano"
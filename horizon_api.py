import requests
import json

HORIZON_URL = 'https://ssd.jpl.nasa.gov/api/horizons.api'

# funkcija vrne URL za HTTP get request na horizon api z parametri v slovarju parameter_map
def create_query(parameter_map: dict):
    parameter_map["format"]="text"
    parameters_list = [key+'='+value for key,value in parameter_map.items()]
    return f"{HORIZON_URL}?{'&'.join(parameters_list)}"

# funkcije vrne slovar teles ki se ujemajo search query-u in njihovih indeksov, če ni rezultatov ali pride do napake vrne None
def search_body(search_query="MB"):
    try: 
        result = requests.get(create_query({"COMMAND":search_query}), timeout=5)
        lines = result.content.decode().splitlines()[:-2]
        widths_line = list(map(len, lines[7].split()))

        if "Multiple" not in lines[4] and "Matching" not in lines[-1]: return {"_single_": lines[4].split()[-1].strip()}

        parsed_body_map = {}
        for i in range(8, len(lines)):
            body_name = lines[i][widths_line[0]+4:widths_line[0]+4+widths_line[1]].strip()
            body_index = lines[i][0:widths_line[0]+2].strip()

            if len(body_name) == 0 or len(body_index) == 0: continue
            parsed_body_map[body_name] = body_index

        return parsed_body_map
    except:
        return None

# funkcija vrne objekt ki vsebuje podatke o izbranem telesu, če ni rezultatov ali pride do napake vrne None
# masa je v enotah 10^20 kg, polmer v km, lega v km, hitrost v km/s
def get_body_json(body_index, time, name=""):
    if not name: name = body_index

    # pomagalna funkcija, ki vrne prvo veljavno število v vrstcii po zanku =, 0 če pride nekje do napake
    def extract_first_num(line):
        num = ""; i = line.find("=")+1
        while i < len(line):
            if line[i] == " " or line[i] == "~": 
                i += 1; continue
            if line[i].isdigit(): 
                num += line[i]
                i += 1; continue
            if line[i] == ".":
                num += "."
                i += 1; continue
            break

        try:    return float(num)
        except: return 0

    try:
        result = requests.get(create_query({
            "COMMAND":      body_index,
            "OBJ_DATA":     "YES",
            "MAKE_EPHEM":   "YES",
            "EPHEM_TYPE":   "VECTORS",
            "CENTER":       "500@0",
            "TLIST":        f"%27{time}%27",
            "CSV_FORMAT":   "YES"
        }), timeout=5)
        lines = result.content.decode().splitlines()
        
        # če mogoče najdemo vrstici ki vsebujejo maso in polmer
        mass_line, radius_line = "", ""
        for line in lines: 
            if not mass_line and "gm" in line.lower():
                mass_line = line.lower()
            if not radius_line and "mean radius" in line.lower():
                radius_line = line.lower()


        # skarajšamo vrstici da v primeru da je v prvem stolpcu nepomembno število
        mass_line = mass_line[mass_line.find("gm"):]
        radius_line = radius_line[radius_line.find("mean radius"):]

        # ekstrahiramo ven števili ki predstavljata produkt GM in polmer
        (GM, radius) = (extract_first_num(mass_line), extract_first_num(radius_line))

        # najdimo vrstico ki vsebuje vekorja lege in hitrosti
        xv_data = []
        for i in range(len(lines)):
            if "$$SOE" in lines[i]:
                xv_data = lines[i+1].split(",")

        # če je mogoče parsirajmo vekotja lokacije in hitrosti za telo
        x_vec, y_vec = (), ()
        try: x_vec = tuple(map(float, xv_data[2:5]))
        except: pass

        try: y_vec = tuple(map(float, xv_data[5:8]))
        except: pass

        # masa je GM/G kjer je G gravitacijska konstanta
        result_json = {
            "name":     name,
            "time":     time, 
            "mass":     GM/6.6743015,
            "radius":   radius,
            "x_vec":    x_vec,
            "v_vec":    y_vec,
        }
        return result_json
    
    except:
        return None

#print(json.dumps(get_body_json("999", "2000-1-1"))) # Pluton
#print(json.dumps(get_body_json("399", "2000-1-1"))) # Zemlja 
#print(json.dumps(get_body_json("502", "2000-1-1"))) # Europa
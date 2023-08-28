#23846485.py

def main(filename):
    lines = readFile(filename) # Use the 'lines' variable as needed
    cleanData(lines)

def readFile(filename):
    try:
        with open(filename, 'r') as file:
            lines = file.readlines()
    except TypeError:
        print("The type of the file is wrong")
        return [] # Return an empty list in case of error
    except OSError:
        print("File", '"' + filename + '"', "was not found.")
        print("Program is terminated!")
        return []  # Return an empty list in case of error
    return lines

def cleanData(lines):
    if len(lines) > 0:
        column_names = [column_name.strip().lower() for column_name in lines[0].split(',')]
    else:
        print("No data found.")
        return []
    
    data = {}
    region_data = {}

    
    for line in lines[1:]:
        try:
            row = {keys: values.strip() for keys, values in zip(column_names, line.split(','))}
            region = row.get('regions', '').lower()
            population = int(row.get('population', 0))
            land_area = int(row.get('land area', 0))
            net_change = int(row.get('net change', 0))
            #country = row.get('country')
            #print(country)
        
            # Skip invalid rows
            if not region or not row['country'] or population <= 0 or land_area <= 0:
                continue
            
            if region not in data:
                data[region] = {
                    'population': [],
                    'land_area': []
                    }
            data[region]['population'].append(population)
            data[region]['land_area'].append(land_area)
            
            if region not in region_data:
                region_data[region][row['country']] = [
                    population,
                    net_change,
                    0.0,
                    0.0,
                    0
                    ]
                
        except (ValueError, IndexError):
            # Handle expected errors and skip invalid rows
            continue


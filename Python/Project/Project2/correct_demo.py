def calculate_mean(data):
    return sum(data) / len(data)

def calculate_standard_error(data):
    mean = calculate_mean(data)
    numerator = [(x - mean) ** 2 for x in data]
    stdev = (sum(numerator) / (len(data) - 1)) ** 0.5
    standard_error = stdev / (len(data) ** 0.5)
    return standard_error

def calculate_dot_product(x, y):
    return sum(a * b for a, b in zip(x, y))

def calculate_magnitude(vector):
    return sum(a * a for a in vector) ** 0.5

def calculate_cosine_similarity(x, y):
    dot_product = calculate_dot_product(x, y)
    magnitude_x = calculate_magnitude(x)
    magnitude_y = calculate_magnitude(y)
    cosine_sim = dot_product / (magnitude_x * magnitude_y)
    return cosine_sim

def process_csv_file(csvfile):
    try:
        with open(csvfile, 'r') as file:
            lines = file.readlines()
            headers = [header.strip().lower() for header in lines[0].split(',')]
            data = {}
            region_data = {}

            for line in lines[1:]:
                try:
                    row = {k: v.strip() for k, v in zip(headers, line.split(','))}
                    region = row.get('regions', '').lower()
                    population = float(row.get('population', 0))
                    land_area = float(row.get('land area', 0))
                    net_change = float(row.get('net change', 0))
                    #print(land_area)

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
                        region_data[region] = {}

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

            for region, region_stats in data.items():
                population_data = region_stats['population']
                land_area_data = region_stats['land_area']
                #print(region,land_area_data)

                if len(population_data) <= 1:
                    standard_error = 0.0
                else:
                    standard_error = calculate_standard_error(population_data)

                cosine_sim = calculate_cosine_similarity(population_data, land_area_data)

                data[region] = [round(standard_error, 4), round(cosine_sim, 4)]

                for region, country_data in region_data.items():
                    #print(land_area_data)
                    total_population = sum(country_data[0] for country_data in country_data.values())
                    sorted_countries = sorted(country_data.items(), key=lambda x: (x[1][0], -x[1][3], x[0]))
                

                    rank = 1
                    prev_population = None
                    prev_density = None

                    sorted_countries = sorted(sorted_countries, key=lambda x: x[1][0], reverse=True)  # Sort by population in descending order

                    for i, (country, country_data) in enumerate(sorted_countries, start=1):
                        population, net_change, _, _, _ = country_data
                        percentage_population = (population / total_population) * 100

                        if i - 1 < len(land_area_data):
                            density = population / land_area_data[i - 1]
                            country_data[2] = round(percentage_population, 4)
                            country_data[3] = round(density, 4)
                            
                            #print(country,population, land_area_data[i - 1])

                            if prev_population is not None and population == prev_population and density == prev_density:
                                rank = prev_rank  # Assign the same rank as the previous country
                            else:
                                rank = i  # Assign the current rank

                            country_data[4] = rank

                            prev_population = population
                            prev_density = density
                            prev_rank = rank

                    region_data[region] = {country: country_data[0:5] for country, country_data in sorted_countries}

            return data, region_data
    except FileNotFoundError:
        print("File not found. Please provide a valid file.")
    except Exception as e:
        print(f"An error occurred: {str(e)}")
    return {}, {}


def main(csvfile):
    data, region_data = process_csv_file(csvfile)
    return data, region_data

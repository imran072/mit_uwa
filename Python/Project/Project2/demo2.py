def calculate_mean(data):
    return sum(data) / len(data)

def calculate_standard_error(data):
    mean = calculate_mean(data)
    squared_errors = [(x - mean) ** 2 for x in data]
    standard_error = (sum(squared_errors) / (len(data) - 1)) ** 0.5
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
    with open(csvfile, 'r') as file:
        lines = file.readlines()
        headers = [header.strip().lower() for header in lines[0].split(',')]
        data = {}
        region_data = {}

        for line in lines[1:]:
            row = {k: v.strip() for k, v in zip(headers, line.split(','))}
            region = row.get('regions', '').lower()
            population = float(row.get('population', 0))
            land_area = float(row.get('land area', 0))
            net_change = float(row.get('net change', 0))

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

        for region, region_data in data.items():
            population_data = region_data['population']
            land_area_data = region_data['land_area']

            if len(population_data) <= 1:
                standard_error = 0.0
            else:
                standard_error = calculate_standard_error(population_data)

            cosine_sim = calculate_cosine_similarity(population_data, land_area_data)

            data[region] = [round(standard_error, 4), round(cosine_sim, 4)]

        for region, countries in region_data.items():
            total_population = sum(country_data[0] for country_data in countries.values())
            sorted_countries = sorted(countries.items(), key=lambda x: (x[1][0], -x[1][3], x[0]))

            rank = 1
            for i, (country, country_data) in enumerate(sorted_countries, start=1):
                population, net_change, _, _, _ = country_data
                percentage_population = (population / total_population) * 100
                density = population / land_area_data[i - 1]
                country_data[2] = round(percentage_population, 4)
                country_data[3] = round(density, 4)
                country_data[4] = rank

                if i < len(sorted_countries):
                    next_country_data = sorted_countries[i][1]
                    if next_country_data[0] == population and next_country_data[3] > density:
                        rank -= 1
                rank += 1

        return data, region_data

def main(csvfile):
    data, region_data = process_csv_file(csvfile)
    return data, region_data


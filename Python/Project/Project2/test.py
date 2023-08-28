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

def process_csv_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
        headers = [header.strip().lower() for header in lines[0].split(',')]
        data = {}

        for line in lines[1:]:
            row = {k: v.strip() for k, v in zip(headers, line.split(','))}
            region = row.get('region', '').lower()
            population = float(row.get('population', 0))
            land_area = float(row.get('land area', 0))

            if region not in data:
                data[region] = {
                    'population': [],
                    'land_area': []
                }

            data[region]['population'].append(population)
            data[region]['land_area'].append(land_area)

        for region, region_data in data.items():
            population_data = region_data['population']
            land_area_data = region_data['land_area']

            standard_error = calculate_standard_error(population_data)
            cosine_sim = calculate_cosine_similarity(population_data, land_area_data)

            print(f"Region: {region}")
            print(f"Standard Error for Population: {standard_error}")
            print(f"Cosine Similarity (Population vs Land Area): {cosine_sim}")
            print()


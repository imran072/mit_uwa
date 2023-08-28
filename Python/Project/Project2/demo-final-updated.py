def process_csv_file(csvfile):
    try:
        with open(csvfile, 'r') as file:
            lines = file.readlines()
            headers = [header.strip().lower() for header in lines[0].split(',')]
            data = {}
            region_data = {}
            country_names = set()

            for line in lines[1:]:
                try:
                    row = {k: v.strip() for k, v in zip(headers, line.split(','))}
                    region = row.get('regions', '').lower()
                    population = float(row.get('population', 0))
                    land_area = float(row.get('land area', 0))
                    net_change = float(row.get('net change', 0))

                    # Skip invalid rows
                    if (
                        not region
                        or not row['country']
                        or population <= 0
                        or land_area <= 0
                        or row['country'].lower() in country_names
                    ):
                        continue

                    country_names.add(row['country'].lower())

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
                sorted_countries = sorted(
                    countries.items(),
                    key=lambda x: (x[1][0], -x[1][3], x[0])
                )

                rank = 1
                for i, (country, country_data) in enumerate(sorted_countries, start=1):
                    population, net_change, _, _, _ = country_data

                    # Skip rows with zero population or land area
                    if population == 0 or region_data[region]['land_area'] == 0:
                        continue

                    percentage_population = (population / total_population) * 100
                    density = population / region_data[region]['land_area']
                    region_data[region][country] = [
                        population,
                        net_change,
                        round(percentage_population, 4),
                        round(density, 4),
                        rank
                    ]

                    rank += 1

            return data, region_data
    except FileNotFoundError:
        print("File not found. Please provide a valid file.")
    except Exception as e:
        print(f"An error occurred: {str(e)}")
    return {}, {}


def main(csvfile):
    data, region_data = process_csv_file(csvfile)
    return data, region_data


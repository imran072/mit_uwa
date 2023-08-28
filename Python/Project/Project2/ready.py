#23846485.py

# Script Summary:
# This script reads a CSV file containing population, land area, and country data.
#It performs data processing, statistical calculations, and generates dictionaries containing region statistics
#and country statistics.

# Function Definitions:
# - meanCalculator(n_num): Calculates the mean of a list of numbers.
# - standardErrorCalculator(n_num): Calculates the standard error of a list of numbers.
# - dotProductCalculator(x, y): Calculates the dot product of two lists of numbers.
# - magnitudeCalculator(vector): Calculates the magnitude (Euclidean norm) of a list of numbers.
# - cosineSimilarityCalculator(x, y): Calculates the cosine similarity between two lists of numbers.
# - readFile(filename): Reads the contents of a file and returns the lines as a list.
# - cleanData(lines): Processes the lines of data, cleans the values, and creates dictionaries for data storage.
# - getRegionStats(data): Calculates region statistics based on the data stored in the 'data' dictionary.
# - getRegionCountryStats(region_data): Calculates country statistics based on the data stored in the 'region_data' dictionary.

# Main Execution:
# - Reads the CSV file and retrieves the lines of data.
# - Cleans the data, removing invalid rows and preparing it for further processing.
# - Calculates region statistics, including the standard error and cosine similarity for each region.
# - Calculates country statistics, including the percentage population, population density, and rank for each country in each region.
# - Returns dictionaries containing the region statistics and country statistics.


def meanCalculator(n_num):
    return sum(n_num) / len(n_num)

def standardErrorCalculator(n_num):
    mean = meanCalculator(n_num)
    numerator = [(x - mean) ** 2 for x in n_num]
    stdev = (sum(numerator) / (len(n_num) - 1)) ** 0.5
    standard_error = stdev / (len(n_num) ** 0.5)
    return standard_error

def dotProductCalculator(x, y):
    return sum(a * b for a, b in zip(x, y))

def magnitudeCalculator(vector):
    return sum(a * a for a in vector) ** 0.5

def cosineSimilarityCalculator(x, y):
    dot_product = dotProductCalculator(x, y)
    magnitude_x = magnitudeCalculator(x)
    magnitude_y = magnitudeCalculator(y)
    cosine_similarity = dot_product / (magnitude_x * magnitude_y)
    return cosine_similarity


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
    country_counts = {}  # To track country occurrences

    for line in lines[1:]:
        row = {keys: values.strip() for keys, values in zip(column_names, line.split(','))}
        country = row.get('country')
        
        if country not in country_counts:
            country_counts[country] = 1
        else:
            country_counts[country] += 1

    
    for line in lines[1:]:
        try:
            row = {keys: values.strip() for keys, values in zip(column_names, line.split(','))}
            region = row.get('regions', '').lower()
            population = int(row.get('population', 0))
            land_area = int(row.get('land area', 0))
            net_change = int(row.get('net change', 0))
            country = row.get('country')
        
            # Skip invalid rows
            if not region or not row['country'] or population <= 0 or land_area <= 0 or country_counts[country] >= 2:
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
                land_area
                ]
                
        except (ValueError, IndexError):
            # Handle expected errors and skip invalid rows
            continue
    return data, region_data

def getRegionStats(data):
    try:
        for region, stats in data.items():
            population_data = stats['population']
            land_data = stats['land_area']
            
            try:
                standard_error = standardErrorCalculator(population_data)

                cosine_sim = cosineSimilarityCalculator(population_data, land_data)
            
            except ZeroDivisionError:
                print("Not enough data to calulate statistics for region:",region)
                continue

            data[region] = [round(standard_error, 4), round(cosine_sim, 4)]

        return data, land_data
        
    except Exception:
        print(f"An error occurred: {str(Exception)}")
        return {}

def getRegionCountryStats(region_data):
    nested_dict = {}
    for region, countries in region_data.items():
        nested_dict[region] = {}
        country_list = []
        for country, data in countries.items():
            population, net_change, land_area = data
            density = round(population / land_area, 4)
            country_list.append([country, population, net_change, density])

        # Sort country_list based on population, density, and alphabetical order
        country_list.sort(key=lambda x: (x[1], -x[3], x[0]), reverse=True)

        # Calculate total population of the region
        total_population = sum(data[1] for data in country_list)

        # Add additional information to each country's data
        rank = 1
        prev_values = None
        for country_data in country_list:
            country, population, net_change, density = country_data
            percentage_population = round((population / total_population) * 100, 4)
            if prev_values and prev_values[:2] != [population, density]:
                rank += 1
            prev_values = [population, density]
            country_data.extend([percentage_population, density, rank])
            nested_dict[region][country.lower()] = country_data[1:]

    return nested_dict


def main(filename):
    try:
        lines = readFile(filename) 
        data, region_data = cleanData(lines)
        dic1 = getRegionStats(data)
        dic2 = getRegionCountryStats(region_data)
        
        return dic1, dic2
    
    except TypeError:
        print('The is a problem in the data set')
    except ValueError:
        print()
        print("Not enough values. Please check the data file")


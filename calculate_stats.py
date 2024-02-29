#23846485.py

# the program performs various calculations on the data from the CSV file
# related to the given region and returns a list of results.

def main(csvfile, region):
    # open csvfile for reading
    with open(csvfile, 'r') as file:
        # read the contents of the file into a list of lines
        lines = file.readlines()
        
        # initialize empty lists and variables for calculating data
        max_population_country = ''
        min_population_country = ''
        max_population = 0
        min_population = 0
        population_region_list = []
        density_region_list = []
        land_area_region_list = []
        population_land_region_sublist = []
        # loop over the lines in the file (skipping the first line to exclude column names)
        for line in lines[1:]:
            # split the line values for delimiter , and store them in different variables
            country, population, yearly_change, net_change, land_area, region_name = line.split(",")
            
            # find the countries that have positive net change for the given region
            # remove newline character from the end of region_name
            if region_name.rstrip() == region and int(net_change) > 0:
                # find the country with maximum and minimum population
                if min_population == 0: # check if min_population got assigned a value from a country of the region
                    min_population = int(population) # assign value for the first time from target countries
                    min_population_country = country
                elif int(population) < min_population: # comapre values to get minimum population and the country
                    min_population = int(population)
                    min_population_country = country
                if max_population == 0: # check if max_population got assigned a value from a country of the region
                    max_population = int(population) 
                    max_population_country = country
                elif int(population) > max_population: # comapre values to get maximum population and the country
                    max_population = int(population)
                    max_population_country = country
            
            # get the list of all population and land areas for the given region
            if region_name.rstrip() == region:
                population_region_list.append(int(population))
                land_area_region_list.append(int(land_area))
                density = int(population) / int(land_area) # calculate density for each country of the region
                density_region_list.append([country, round(density, 4)]) # make a list with all countries and their density 
                population_land_region_sublist.append([int(population), int(land_area)]) # make list with sublist for population and area of each country
        
        # calculate average population and land
        if len(population_region_list) == 0:
            avg_population = 0
            print("Error: not enough sample to calculate average population of the region\n")
        else:
            avg_population = sum(population_region_list) / len(population_region_list)
        if len(land_area_region_list) == 0:
            avg_land = 0
            print("Error: not enough sample to calculate average land areas of the region\n")
        else:
            avg_land = sum(land_area_region_list) / len(land_area_region_list)
        
        # sort in descending order by 2nd element of the sublist   
        density_region_list.sort(key=lambda x: x[1], reverse=True)

        # calculate standard deviation of population
        sum_sq_diff = 0
        for p in population_region_list:
            sum_sq_diff += (p - avg_population) ** 2
        if (len(population_region_list) - 1) == 0:
            stddev_population = 0
            print("Error: not enough sample to calculate standard deviation")
        else:
            stddev_population = (sum_sq_diff / (len(population_region_list) - 1)) ** 0.5

        # calculate correlation between population and land area
        numerator = 0
        denominator = 0
        for p, l in population_land_region_sublist:
            numerator += (p - avg_population) * (l - avg_land)
        for l in land_area_region_list:
            denominator += (l - avg_land) ** 2
        denominator = (sum_sq_diff * denominator) ** 0.5
        if denominator == 0:
            # return zero if denominator is zero
            correlation = 0
            print("Error: denominator for correlation is zero")
        else:
            correlation = numerator / denominator

        return [[max_population_country, min_population_country], [round(avg_population, 4), round(stddev_population, 4)],
                density_region_list, round(correlation, 4)]



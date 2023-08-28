region_date = region_data = {'asia': {'China': [1439323776, 5540090, 9388211], 'India': [1380004385, 13586631, 2973190], 'Indonesia': [273523615, 2898047, 1811570], 'Sri Lanka': [21413249, 89516, 62710], 'Kazakhstan': [18776707, 225280, 2699700], 'Syria': [17500658, 430523, 183630], 'Cambodia': [16718965, 232423, 176520], 'United Arab Emirates': [9890402, 119873, 83600], 'Tajikistan': [9537645, 216627, 139960], 'Israel': [8655535, 136158, 21640], 'Hong Kong': [7496981, 60827, 1050], 'Laos': [7275560, 106105, 230800], 'Timor-Leste': [1318445, 25326, 14870], 'Cyprus': [1207359, 8784, 9240], 'Bhutan': [771608, 8516, 38117], 'Macao': [649335, 8890, 30], 'Maldives': [540544, 9591, 300], 'Brunei': [437479, 4194, 5270]}, 'africa': {'Nigeria': [206139589, 5175990, 910770], 'Ethiopia': [114963588, 2884858, 1000000], 'Egypt': [102334404, 1946331, 995450], 'Angola': [32866272, 1040977, 1246700], 'Ghana': [31072940, 655084, 227540], 'Mozambique': [31255435, 889399, 786380], 'RÈunion': [895312, 6385, 2500], 'Comoros': [869601, 18715, 1861], 'Western Sahara': [597339, 14876, 266000], 'Cabo Verde': [555987, 6052, 4030], 'Mayotte': [272815, 6665, 375], 'Sao Tome & Principe': [219159, 4103, 960], 'Seychelles': [98347, 608, 460], 'Saint Helena': [6077, 18, 390]}, 'europe': {'Russia': [145934462, 62206, 16376870], 'Germany': [83783942, 266897, 348560], 'United Kingdom': [67886011, 355839, 241930], 'France': [65273511, 143783, 547557], 'Italy': [60461826, -88249, 294140], 'Spain': [46754778, 18002, 498800], 'Norway': [5421241, 42384, 365268], 'Ireland': [4937786, 55291, 68890], 'Croatia': [4105267, -25037, 55960], 'Moldova': [4033963, -9300, 32850], 'Bosnia and Herzegovina': [3280819, -20181, 51000], 'Gibraltar': [33691, -10, 10], 'Holy See': [801, 2, 20837]}, 'latin america & caribbean': {'Brazil': [212559417, 1509890, 8358140], 'Mexico': [128932753, 1357224, 1943950], 'Colombia': [50882891, 543448, 1109500], 'Argentina': [45195774, 415097, 2736690], 'Peru': [32971854, 461401, 1280000], 'French Guiana': [298682, 7850, 82200], 'Aruba': [106766, 452, 180], 'U.S. Virgin Islands': [104425, -153, 350], 'Antigua and Barbuda': [97929, 811, 440], 'Falkland Islands': [3480, 103, 12170]}, 'northern america': {'United States': [331002651, 1937734, 9147420], 'Canada': [37742154, 331107, 9093510], 'Bermuda': [62278, -228, 50], 'Greenland': [56770, 98, 410450]}, 'oceania': {'Australia': [25499884, 296686, 7682300], 'Papua New Guinea': [8947024, 170915, 452860], 'New Zealand': [4822233, 39170, 263310], 'Fiji': [896445, 6492, 18270], 'New Caledonia': [285498, 2748, 18280], 'French Polynesia': [280908, 1621, 3660], 'Samoa': [198414, 1317, 2830], 'Nauru': [10824, 68, 20], 'Niue': [1626, 11, 260], 'Tokelau': [1357, 17, 10]}}

def create_nested_dictionary(region_data):
    nested_dict = {}
    for region, countries in region_data.items():
        nested_dict[region] = {}
        country_list = []
        for country, data in countries.items():
            population, net_change, land_area = data
            density = round(population / land_area, 4)
            country_list.append([country, population, net_change, density])

        # Sort country_list based on population, density, and alphabetical order
        country_list.sort(key=lambda x: (x[1], -x[3], x[0]))

        # Calculate total population of the region
        total_population = sum(data[1] for data in country_list)

        # Add additional information to each country's data
        for rank, country_data in enumerate(country_list, start=1):
            country, population, net_change, density = country_data
            percentage_population = round((population / total_population) * 100, 4)
            country_data.extend([percentage_population, density, rank])
            nested_dict[region][country.lower()] = country_data[1:]

    return nested_dict

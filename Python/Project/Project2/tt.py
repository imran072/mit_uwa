def process_csv_file(csvfile):
    with open(csvfile, 'r') as file:
        lines = file.readlines()
        headers = [header.strip().lower() for header in lines[0].split(',')]
    return headers
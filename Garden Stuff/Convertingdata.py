
file_name = 'DATALOG.CSV'

f_object = open(file_name, 'r');

lines = f_object.readlines()
f_object.close()

for i in range(0, len(lines)):
    for number in lines[i].split():
        number[5]





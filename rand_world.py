import random

def colour():
    result = " "
    for i in range(3):
        result += str(random.randint(0,255))+" "
    return result
def pos():
    result = " "
    for i in range(3):
        mult = -1 if random.randint(0,1) == 0 else 1
        result += str(mult*random.uniform(0,50000000))+" "
    return result
def vel():
    result = " "
    for i in range(3):
        mult = -1 if random.randint(0,1) == 0 else 1
        result += str(mult*random.uniform(0,250))+" "
    return result
def mass():
    return " "+str(random.uniform(1,10000000000000000000))
def radius():
    return " "+str(random.uniform(1,100000))





file = open("random.dat",'w')
planets_num = random.randint(400,600)
i = 0
file.write("camera_center 0 0 0")
file.write("camera_angles 0 0\n\n")
while i < planets_num:
    file.write("planet_colour"+colour()+"\n")
    file.write("planet_position"+pos()+"\n")
    file.write("planet_velocity"+vel()+"\n")
    file.write("planet_mass"+mass()+"\n")
    file.write("planet_radius"+radius()+"\n")
    file.write("planet_texture 0"+"\n")
    file.write("create_planet"+"\n")
    file.write("\n")
    i+=1
file.write("end_file")
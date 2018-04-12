
f = open("myfile.txt", "w")

for i in range(1, 101):
    print i
    f.write(str(i))
    f.write("\n")
f.close()

file = open("adc.txt", "r")
data = file.readlines()
file.close()
# print(data)
length = len(data)

result = []

for i in range(length):
    currLen = len(data[length-i-1])
    currEle = data[length-i-1]
    result.append(int(currEle[:currLen-1]))

# print(result)
out = []
for k in range(300):
    out.append(result[k])
print(out)

output = open("output.TXT", "w")
for j in range(length):
    output.write(str(result[j])+'\n')

output.close()
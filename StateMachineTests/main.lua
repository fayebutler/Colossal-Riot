print("In Lua")

a = {}
for i=1,100 do
    a[i]=i*2
end
a["Emily"]="White"
a["Will"]="Green"
a["Stuart"]="Black"

for i,line in ipairs(a) do
    print(line)
end

print(a["Emily"])

print (2^2)

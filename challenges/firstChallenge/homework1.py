word=input("Insert the string to analize: ")
a=set()
ans=0
temp=0
for c in word:
    if c in a:
        if temp >= ans:
            ans=temp
        a.clear()
        temp=1
        a.add(c)
        continue
    else:
        a.add(c)
        temp+=1
if temp >= ans:
	ans=temp
print(ans)


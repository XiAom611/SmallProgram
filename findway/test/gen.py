from random import random as rf
from random import randint as rd
sc=lambda :print('$ 3 wefaewfawef\t d \tawfe\n \n'*rd(0,2),end='')
width=rd(0,35)
height=rd(0,35)
nu=[0,0,0,0] #sx,sy,tx,ty
for i in range(4):
    if rf()>0.95:
        if rf()<0.5:
            nu[i]=-1
        else:
            if i&1==0:
                nu[i]=width if width!=0 else 1
            else:
                nu[i]=height if height!=0 else 1

    else:
        if i&1==0:
            nu[i]=rd(0,width if width==0 else width-1)
        else:
            nu[i]=rd(0,height if height==0 else height-1)


sc()
print(nu[0],nu[1])
if rf()>0.9:nu[2],nu[3]=nu[0],nu[1]
sc()
print(nu[2],nu[3])
sc()
for i in range(height):
    for j in range(width):
        if i==height-1-nu[1] and j==nu[0]:
            print('.',end='')
        elif i==height-1-nu[3] and j==nu[2]:
            print('.',end='')
        else:
            if rf()>0.25:
                print('.',end='')
            else:
                print('X',end='')
    print()
    if rf()>0.7:sc()
sc()

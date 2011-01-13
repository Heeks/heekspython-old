import HeeksPython as cad
def cut_it(obj1,obj2):
    cad.cut(obj1,obj2)
    newobj = cad.getlastobj()
    cad.remove(obj1)
    cad.changed()
    cad.remove(newobj)
    return newobj


cad.cuboid(0,0,0,1,1,1)
c1 = cad.getlastobj()
cad.cylinder(0,0,0,.5,1)
c2 = cad.getlastobj()
n1 = cut_it(c1,c2)

cad.cylinder(1,0,0,.5,1)
c3 = cad.getlastobj()
n2 = cut_it(n1,c3)

cad.cylinder(1,1,0,.5,1)
c4 = cad.getlastobj()
n3 = cut_it(n2,c4)

cad.cylinder(0,1,0,.5,1)
c5 = cad.getlastobj()
n4 = cut_it(n3,c5)

cad.ve()



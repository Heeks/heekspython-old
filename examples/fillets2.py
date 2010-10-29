# this example shows how to create fillets between two lines

import HeeksPython as cad
cad.sketch()
sketch = cad.getlastobj()
cad.line(0,1,4,1)
l1= cad.getlastobj()
cad.line(4,1,4,2)
l2= cad.getlastobj()

cad.line(4,2,0,2)
l3= cad.getlastobj()

cad.line(0,2,0,1)
l4= cad.getlastobj()

cad.add(sketch,l1)
cad.add(sketch,l2)
cad.add(sketch,l3)
cad.add(sketch, l4)
cad.fillet2d(sketch,4,1,0,.1)
f1= cad.getlastobj()
cad.fillet2d(sketch,4,2,0,.1)
f2= cad.getlastobj()
cad.fillet2d(sketch,0,2,0,.1)
f3= cad.getlastobj()
cad.fillet2d(sketch,0,1,0, .1)
f4= cad.getlastobj()
cad.add(sketch,f1)
cad.add(sketch,f2)
cad.add(sketch,f3)
cad.add(sketch, f4)
cad.reorder(sketch)

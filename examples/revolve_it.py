#this little script shows how to use the 'revolve' function
#used for revolving sketches around X axis to form a solid
#like a turned part on a lathe

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
cad.reorder(sketch)
cad.revolve(sketch,360)


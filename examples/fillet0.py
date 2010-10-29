#very simple fillet demo
#the end points of the lines must be trimmed first or you 
#will get an error from HeeksCAD


import HeeksPython as cad
cad.sketch()
sketch = cad.getlastobj()
cad.line(0,1,4,1)
l1= cad.getlastobj()
cad.line(4,1,4,2)
l2= cad.getlastobj()
cad.add(sketch,l1)
cad.add(sketch,l2)
cad.fillet2d(sketch,4,1,0,.1)
cad.reorder(sketch)





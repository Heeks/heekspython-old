import HeeksPython as cad
scale = 25.4 #needed to get to inch units
width=6.0
length=6.0
height = 4.0
originx = 0;originy=0
corner_rad= .25


def make_rect(originx,originy,width,length,rad,height,scale):
    #originx,originy= origin
    originx = originx*scale;originy = originy*scale
    width= width*scale;length= length*scale
    rad = rad*scale
    cad.sketch()
    sketch = cad.getlastobj()
    cad.line(originx,originy,originx+width,originy)
    l1= cad.getlastobj()
    cad.line(originx+width,originy,originx+width,originy+length)
    l2= cad.getlastobj()
    cad.line(originx+width,originy+length,originx,originy+length)
    l3= cad.getlastobj()
    cad.line(originx,originy+length,originx,originy)
    l4= cad.getlastobj()
    cad.add(sketch,l1)
    cad.add(sketch,l2)
    cad.add(sketch,l3)
    cad.add(sketch, l4)
    cad.fillet2d(sketch,originx,originy,0, rad)
    cad.fillet2d(sketch,originx+width,originy,0,rad)
    cad.fillet2d(sketch,originx+width,originy+length,0,rad)
    cad.fillet2d(sketch,originx,originy+length,0,rad)
    cad.reorder(sketch)
    cad.extrude(sketch,height*scale)

cad.setcolor(100, 200, 55)
make_rect(originx,originy,width,length,corner_rad,height,scale)
b1= cad.getlastobj()
box2_width=width*.6
box2_length=length*.6
make_rect(((originx+width*.5)-box2_width*.5),((originy+length*.5)-box2_length*.5),box2_width,box2_length,corner_rad*.75,height,scale)
b2 = cad.getlastobj()
cad.cut(b1,b2)
cad.view_extents()

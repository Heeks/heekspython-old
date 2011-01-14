import HeeksPython as cad
from math import pi

def cut_it(obj1,obj2):
    cad.cut(obj1,obj2)
    newobj = cad.getlastobj()
    cad.remove(obj1)
    cad.changed()
    cad.remove(newobj)
    return newobj

def blank(outer_dia,inner_dia,length):
    x0,y0,z0 = 0,0,0
    rad1 = (outer_dia)*.5
    length = length
    cad.cylinder(x0,y0,z0,rad1,length)
    c1 = cad.getlastobj()

    rad2 = (inner_dia)*.5

    cad.cylinder(x0,y0,z0,rad2,length)
    c2 = cad.getlastobj()

    ring = cut_it(c1,c2)

    cad.rotate(ring,x0,y0,z0,(x0+1),y0,z0,(pi/2))
    cad.changed()
    return ring


def tooth_form(tilt,length):
    cad.sketch()
    sketch = cad.getlastobj()
    cad.line(-.0625,.1083,.0625,.1083)
    l1 = cad.getlastobj()
    cad.line(.0625,.1083,0,0)
    l2 = cad.getlastobj()
    cad.line(0,0, -.0625,.1083)
    l3 = cad.getlastobj()    
    cad.add(sketch,l1)
    cad.add(sketch,l2)
    cad.add(sketch,l3)
    cad.fillet2d(sketch,0,0,0,.002)
    cad.reorder(sketch)
    cad.extrude(sketch,length)
    form = cad.getlastobj()
    cad.rotate(form,0,0,0,1,0,0,(tilt*pi)/180)
    cad.changed()
    cad.remove(sketch)
    return form        

def polar_array(obj,tilt,num_teeth):
    circle_division=(360/num_teeth)
    pitch_angle = (circle_division*(pi*2))/360
    angle = 0
    count = 0
    while (count < num_teeth):
        t1 = tooth_form(tilt,.5)
        cad.rotate(t1,0,0,0,0,1,0,angle)
        cad.cut(obj,t1)
        obj2 = cad.getlastobj()
        cad.remove(obj)
        obj = obj2
        
        angle = angle + pitch_angle
        cad.changed() 
        count = count +1
    
    cad.remove(obj)         
    return obj2

units = 25.4 #units are in mm normally so multiply inch units by this    
b1 = blank(.49,.3,.1)
cad.translate(b1,0,0,0)
gear1= polar_array(b1,4,72)
cad.rotate(gear1,0,0,0,1,0,0,pi)
cad.rotate(gear1,0,0,0,0,1,0,(2.5*pi)/180)
cad.setcolor(0,188,180)
gear2= polar_array(b1,4,72)
cad.scale(gear1,0,0,0,units)
cad.scale(gear2,0,0,0,units)
cad.translate(gear2,0,.009*units,0)






  

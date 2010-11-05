#!/usr/bin/env python
"""Utility for translating between 2D dxf closed profiles and HeeksPython format. Dan Falck 10/31/10
inspiration from Doug Blanding's Cadvas.py dxf.py module- thanks Doug!
"""
import math

##########################
#
# parse the dxf file
# 
##########################
def _parsepair(a):
    """Parse a pair of lines containing 'group code' and 'value'."""
    groupcode = a.next().strip()
    value = a.next().strip()
    return (groupcode, value)

def _gotosection(a, secname):
    """Go to secname and stop."""
    while 1:
        gc, val = _parsepair(a)
        if gc == '2' and val == secname:
            return
            
def _get_units(a):
    """Parse through HEADER section and detect whether units
    are set for metric(1) or english(0)."""
    _gotosection(a, 'HEADER')
    units = 0   # assume inches by default
    while 1:
        gc, val = _parsepair(a)
        if gc == '9' and val == '$MEASUREMENT':
            gc, val = _parsepair(a)
            if gc == '70':
                units = int(val)
        elif gc == '0' and val == 'ENDSEC':
            return units

def _process_entity(a):
    """Return a dictionary of groupcodes : values for the next
    entity in the ENTITIES section. Go until groupcode == 0."""
    entitydict = {}
    flag = 1
    while 1:
        gc, val = _parsepair(a)
        if gc == '0':
            if val == 'ENDSEC':
                flag = 0    # Done with ENTITIES section
            return (entitydict, flag, val)
        else:
            entitydict[gc] = val

def _parse_file(f):
    """Parse contents of the dxf file, looking for units and
    all the drawing entities."""
    a = iter(open(f))
    units = _get_units(a)
    _gotosection(a, 'ENTITIES')
    lines = []
    circles = []
    arcs = []
    entities = [lines, circles, arcs]
    gc, val = _parsepair(a)
    while 1:
        if val == 'LINE':
            ed, f, val = _process_entity(a)
            lines.append(ed)
            #print 'line\n'
        elif val == 'CIRCLE':
            ed, f, val = _process_entity(a)
            circles.append(ed)
            #print 'circle\n'
        elif val == 'ARC':
            ed, f, val = _process_entity(a)
            arcs.append(ed)
            #print 'arc\n'
        else:
            ed, f, val = _process_entity(a)
        if not f:
            return (units, entities)

##########################
#
# dxf to HeeksPython
# DF added 10/31/10
##########################
def gen_heekspython_entities(f,sketch_num):
    """Generate HeeksPython objects from dxf entities."""
    units, entities = _parse_file(f)
    lines, circles, arcs = entities
    if units:
        scale = 1.0
    else:
        scale = 25.4
    mldict = {}
    mcdict = {}
    madict = {}
    k = 0
    i = 0
    collector=[] 
    collector.append("cad.sketch()\n")
    collector.append("sketch"+str(sketch_num)+" = cad.getlastobj()\n")   
    for line in lines:
        p1 = (float(line['10'])*scale,
              float(line['20'])*scale)
        p2 = (float(line['11'])*scale,
              float(line['21'])*scale)
        coords = (p1, p2)
        #print"#Line,"+str(p1[0])+","+str(p1[1])+",0,"+str(p2[0])+","+str(p2[1])+",0\n"
        collector.append("cad.line("+str(p1[0])+","+str(p1[1])+","+str(p2[0])+","+str(p2[1])+")\n")
        collector.append("i"+str(i)+"= cad.getlastobj()\n")
        collector.append("cad.add(sketch"+str(sketch_num)+",i"+str(i)+")\n")
        i+=1
        k+=1
        
        mldict[k] = coords
    string = "".join(collector)     
 
    k = 0
    for circ in circles:
        cntr = (float(circ['10'])*scale,
                float(circ['20'])*scale)
        radius = float(circ['40'])*scale
        coords = (cntr, radius)
        #print "#Circle,"+str(cntr[0])+","+str(cntr[1])+",0,"+str(radius)+"\n"
        collector.append("cad.circle("+str(cntr[0])+","+str(cntr[1])+","+str(radius)+")\n")
        collector.append("i"+str(i)+"= cad.getlastobj()\n")
        collector.append("cad.add(sketch"+str(sketch_num)+",i"+str(i)+")\n")
        i+=1
        k+=1
        mcdict[k] = coords
    string = "".join(collector)         
    k = 0
    for arc in arcs:
        cntr = (float(arc['10'])*scale,
                float(arc['20'])*scale)
        radius = float(arc['40'])*scale
        a0 = float(arc['50'])
        a1 = float(arc['51'])
        coords = (cntr, radius, a0, a1)
        angle1=((math.pi)/180)*a0
        angle2=((math.pi)/180)*a1
        #print "#Arc,"+str(cntr[0])+","+str(cntr[1])+",0,"+str(radius)+","+str(angle1)+","+str(angle2)+"\n"
        collector.append("cad.arc("+str(cntr[0])+","+str(cntr[1])+",0,"+str(radius)+","+str(angle1)+","+str(angle2)+",0,0,1)\n")
        #hardcoded dir vector and need to convert angles to radians, I 
        collector.append("i"+str(i)+"= cad.getlastobj()\n")
        collector.append("cad.add(sketch"+str(sketch_num)+",i"+str(i)+")\n")
        i+=1
        k+=1
        madict[k] = coords
    string = "".join(collector)
    string= string + "cad.reorder(sketch"+str(sketch_num)+")\n" 
    #string= string + "cad.revolve(sketch,360)"            
    return string         




#example of how to use it
'''
import HeeksPython as cad
import sys
sys.path.insert(0,'/home/dan/heeks/heekspython2/examples')
import dxf_to_heekspython

file_in='/home/dan/Documents/drawings/blob.dxf'
l = dxf_to_heekspython.gen_heekspython_entities(file_in,1)
exec(l)

or scale the object/sketch like this:
l = dxf_to_heekspython.gen_heekspython_entities(file_in,1)
l = l +"cad.scale(sketch1,0,0,0,.25)"
exec(l)

or even revolve a solid after returning l
l = dxf_to_heekspython.gen_heekspython_entities(file_in,1)
l = l +"cad.revolve(sketch1,360)"
exec(l)

'''


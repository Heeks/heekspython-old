#***************************************************************************
#*                                                                         *
#*   Copyright (c) 2010 Dan Falck <ddfalck@gmail.com>                      *
#*   derived from Yorik van Havre's <yorik@gmx.fr>  importDXF.py           *
#*   script that is part of the Draft plugin for FreeCAD                   *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License (GPL)            *
#*   as published by the Free Software Foundation; either version 2 of     *
#*   the License, or (at your option) any later version.                   *
#*   for detail see the LICENCE text file.                                 *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU Library General Public License for more details.                  *
#*                                                                         *
#*   You should have received a copy of the GNU Library General Public     *
#*   License along with this program; if not, write to the Free Software   *
#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
#*   USA                                                                   *
#*                                                                         *
#***************************************************************************

__title__="dxf_import.py  HeeksPython - DXF importer"
__author__ = "Dan Falck <ddfalck@gmail.com>"
__url__ = ["http://code.google.com/p/heekspython/"]

'''
This script uses a DXF-parsing library created by Stani,
Kitsu and Migius for Blender

imports:
line, polylines, lwpolylines, arcs, circles, texts,
mtexts, layers (as groups), colors

'''
import math
import numpy
from dxfReader import readDXF

def equals(p1,p2):
    if p1 == p2:
        return True
    else:
        return False


def importLine():
    return ("import HeeksPython as cad\n")


def drawLine(line):
    "returns a HeeksPython line3d from a dxf line"
    if (len(line.points) > 1):
        v1=(line.points[0][0],line.points[0][1],line.points[0][2])
        v2=(line.points[1][0],line.points[1][1],line.points[1][2])

        x0=line.points[0][0];y0=line.points[0][1];z0=line.points[0][2]
        x1=line.points[1][0];y1=line.points[1][1];z1=line.points[1][2]

        if not equals(v1,v2):
            try: return ("cad.line3d("+str(x0)+","+str(y0)+","+str(z0)+","+str(x1)+","+str(y1)+","+str(z1)+")\n")
            except: warn(line)
    return None

def drawArc(arc):
    "returns a Part shape from a dxf arc"
    cen=(arc.loc[0],arc.loc[1],arc.loc[2])
    firstangle=(arc.start_angle/180)*math.pi
    lastangle=(arc.end_angle/180)*math.pi
    rad = arc.radius

    center = (str(arc.loc[0])+ ", " +str(arc.loc[1])+ ", " + str(arc.loc[2]))
    radius =  str(rad)
    angle1 = str(firstangle)
    angle2 = str(lastangle)
    dir_vec = "0,0,1"
    try: return ("cad.arc(" + center + ", " + radius + ", " + angle1 + ", " + angle2 + ", " + dir_vec + ")\n")
    except: warn(arc)
    return None
#***************************************************************************
# functions copied from fcvec for polyline bulges
#***************************************************************************
def precision():
    return 6

def isNull(vector):
    '''isNull(vector): Tests if a vector is nul vector'''
    p = precision()
    return (round(vector[0],p)==0.0 and round(vector[1],p)==0.0 and round(vector[2],p)==0.0)

def equals(u,v):
    "returns True if vectors differ by less than precision (from ParamGet), elementwise "
    #typecheck ([(u,Vector), (v,Vector)], "equals")
    return isNull(numpy.subtract(u,v))

def isColinear(vlist):
    '''isColinear(list_of_vectors): checks if vectors in given list are colinear'''
    #typecheck ([(vlist,list)], "isColinear");
    if len(vlist) < 3: return True
    #first = vlist[1].sub(vlist[0])
    first = numpy.subtract(vlist[1],vlist[0])
    for i in range(2,len(vlist)):
        #if angle(vlist[i].sub(vlist[0]),first) != 0:
        if angle(numpy.subtract(vlist[i],vlist[0]),first) != 0:
            return False
    return True

def angle(u,v=(1,0,0),normal=(0,0,1)):
    '''angle(Vector,[Vector],[Vector]) - returns the angle in radians between the two vectors.
    If only one is given, angle is between the vector and the horizontal East direction.
    If a third vector is given, it is the normal used to determine the sign of the angle.
    '''
    #typecheck ([(u,Vector), (v,Vector)], "angle")
    #ll = u.Length*v.Length
    ll = numpy.linalg.norm(u)*numpy.linalg.norm(v)
    if ll==0: return 0
    #dp=u.dot(v)/ll
    dp=numpy.dot(u,v)/ll
    if (dp < -1): dp = -1 # roundoff errors can push dp out of the ...
    elif (dp > 1): dp = 1 # ...geometrically meaningful interval [-1,1]
    ang = math.acos(dp)
    #normal1 = u.cross(v)
    normal1 = numpy.cross(u,v)
    #coeff = normal.dot(normal1)
    coeff = numpy.dot(normal,normal1)
    if coeff >= 0:
        return ang
    else:
        return -ang

#*******************************************************************
#   polyline related functions
#*******************************************************************
def calcBulge(v1,bulge,v2):
    '''
    calculates intermediary vertex for curved segments.
    algorithm from http://www.afralisp.net/lisp/Bulges1.htm
    not needed yet for HeeksPython and not really tested yet either
    '''
    #chord = v2.sub(v1)
    chord = numpy.subtract(v2,v1)
    #sagitta = (bulge * chord.Length)/2
    chord_length= numpy.linalg.norm(chord)
    sagitta = (bulge*chord_length)/2
    #startpoint = v1.add(fcvec.scale(chord,0.5))
    startpoint = numpy.add(v1,(numpy.multiply(chord,(.5,.5,.5))))
    #perp = chord.cross(Vector(0,0,1))
    perp = numpy.cross(chord,(0,0,1))
    #if not isNull(perp): perp.normalize()
    if not isNull(perp): numpy.linalg.norm(perp)
    #endpoint = fcvec.scale(perp,sagitta)
    endpoint = numpy.multiply(perp,sagitta)
    #return startpoint.add(endpoint)
    return numpy.add(startpoint,endpoint)

def calc_center(v1,bulge,v2):
    '''
    calculates center of arc- this one works
    ''' 
    chord = numpy.subtract(v2,v1)
    chord_length= numpy.linalg.norm(chord)
    sagitta = (bulge*chord_length)/2.0
    inc_angle = numpy.arctan(bulge)*4.0
    radius = (chord_length/2.0)/numpy.sin(inc_angle/2.0)
    if bulge >= 0:
        perp = (numpy.cross(chord,(0,0,-1)))
    else:
        perp = (numpy.cross(chord,(0,0,1)))
    chord_mid_pt = numpy.add(numpy.multiply(chord,(.5,.5,.5)),v1)
    unit_vec = perp/ numpy.linalg.norm(perp)
    arc_center = numpy.add(numpy.multiply((radius-sagitta),unit_vec),chord_mid_pt)
    return arc_center

        

def drawPolyline(polyline,i):
    "returns a Part shape from a dxf polyline"
    if (len(polyline.points) > 1):
        collector = []
        edges = []
        for p in range(len(polyline.points)-1):
            p1 = polyline.points[p]
            p2 = polyline.points[p+1]
            v1 = (p1[0],p1[1],p1[2])
            v2 = (p2[0],p2[1],p2[2])
            if not equals(v1,v2):
                
                if polyline.points[p].bulge:
                    #return "bulge\n"
                    #cv = calcBulge(v1,polyline.points[p].bulge,v2)
                    cv = calc_center(v1,polyline.points[p].bulge,v2)
                    if isColinear([v1,cv,v2]):

                        #try: edges.append(Part.Line(v1,v2).toShape())
                        try: 
                            collector.append("cad.line3d("+str(v1[0])+","+str(v1[1])+","+str(v1[2])+","+str(v2[0])+","+str(v2[1])+","+str(v2[2])+")\n")
                            collector.append("e"+str(i)+ "=cad.getlastobj()\n")
                            collector.append("cad.add("+rename(polyline.layer)+",e"+str(i)+")\n") 
                            i+=1
                        except: 
                            warn(polyline)
                    else:
                        start=(str(v1[0])+","+str(v1[1])+","+str(v1[2]))
                        center=(str(cv[0])+","+str(cv[1])+","+str(cv[2]))
                        end = (str(v2[0])+","+str(v2[1])+","+str(v2[2]))
                        dir_vec = "0,0,1"
                            #try: edges.append(Part.Arc(v1,cv,v2).toShape())
                        try: 
                            collector.append("cad.arc2("+ start+","+end+"," + center + ", " + dir_vec + ")\n")
                            collector.append("e"+str(i)+ "=cad.getlastobj()\n")
                            collector.append("cad.add("+rename(polyline.layer)+",e"+str(i)+")\n") 
                            i+=1
                        except:
                            warn(polyline)
                else:
                    try: 
                        collector.append("cad.line3d("+str(v1[0])+","+str(v1[1])+","+str(v1[2])+","+str(v2[0])+","+str(v2[1])+","+str(v2[2])+")\n")
                        collector.append("e"+str(i)+ "=cad.getlastobj()\n")
                        collector.append("cad.add("+rename(polyline.layer)+",e"+str(i)+")\n") 
                        i+=1
                    except: 
                        warn(polyline)
                
            else:
                return "no way!\n", i

        if polyline.closed:
            p1 = polyline.points[len(polyline.points)-1]
            p2 = polyline.points[0]
            v1 = (p1[0],p1[1],p2[2])
            v2 = (p2[0],p2[1],p2[2])
            if not equals(v1,v2):
                try: 
                    collector.append("cad.line3d("+str(v1[0])+","+str(v1[1])+","+str(v1[2])+","+str(v2[0])+","+str(v2[1])+","+str(v2[2])+")\n")
                    collector.append("e"+str(i)+ "=cad.getlastobj()\n")
                    collector.append("cad.add("+rename(polyline.layer)+",e"+str(i)+")\n") 
                    i+=1
                except: 
                    warn(polyline)
        string = "".join(collector)
        return string, i
    else:
        return "No\n", i
#*******************************************************************
#   end of polyline related functions
#*******************************************************************
def warn(dxfobject):
    "outputs a warning if a dxf object couldn't be imported"
    print "dxf: couldn't import", dxfobject

#*******************************************************************
#   rename layers to object titles so that Heeks doesn't complain 
#   in the case of the layer being named a number
#*******************************************************************
def rename(layer):
    return str("layer_"+layer)
    
#*******************************************************************
#   process the file
#*******************************************************************
def process(filename):
    "this does the translation of the dxf contents into HeeksPython Part objects"
    global drawing 
    header_collector = []
    collector = []
    layer_collector = []
    i = 1
    drawing = readDXF(filename)
    global layers
    layers = []


    # drawing lines    
    lines = drawing.entities.get_type("line")    
    for line in lines:
        shape = drawLine(line)
        if shape:
            collector.append(shape)
            layers.append(line.layer)
            i+=1
            collector.append("e"+str(i)+ "=cad.getlastobj()\n")
            collector.append("cad.add("+rename(line.layer)+",e"+str(i)+")\n")
    string = "".join(collector) 

    # drawing arcs
    arcs = drawing.entities.get_type("arc")    
    for arc in arcs:
        shape = drawArc(arc)
        if shape:
            collector.append(shape)
            layers.append(arc.layer)
            i+=1
            collector.append("e"+str(i)+ "=cad.getlastobj()\n")
            collector.append("cad.add("+rename(arc.layer)+",e"+str(i)+")\n")
    string = "".join(collector) 

    # drawing polylines
    polylines = drawing.entities.get_type("polyline")
    polylines.extend(drawing.entities.get_type("lwpolyline"))

    for polyline in polylines:
        shape,poly_index = drawPolyline(polyline,i)
        i = poly_index
        if shape:
            collector.append(shape)
            layers.append(polyline.layer)

    string = "".join(collector)  

    #print "import HeeksPython as cad"
    #heeksimport = importLine()
    #print heeksimport
    #collector.append(heeksimport)
    #string = "".join(collector) 
    #print string
    #string = collector

    #string1 = "import HeeksPython as cad\n"

    header_collector.append("import HeeksPython as cad\n")
    string1 = "".join(header_collector) 
    print string1

    LayerList = list(set(layers))
    for LL in LayerList:
        print "cad.sketch()\n"
        print (rename(LL) + "=cad.getlastobj()\n")
        #layer_collector.append("cad.sketch()\n")
        #layer_collector.append(rename(LL) + "=cad.getlastobj()\n")
        #string2 = "".join(layer_collector)
        #print string2


 
    print string 

    for LL in LayerList:
        print ("cad.reorder("+rename(LL)+ ")\n" )


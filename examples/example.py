import HeeksPython as cad
import math

beam_width = 2.0;
beam_height = 2.0;
beam_thickness = .25;

bend_radius = 20.0;
line_length = 40*12.0;
dead_zone_width = 3*12.0;

top_overhang = 1.1;
bottom_overhang = 4.1;
side_overhang = 1.0;
gripper_thickness = .5;
beam_clearance = .1;

rail_height = 1;

bearing_clearance = .25;
bearing_height = 1;
dist_between_bearings = 3;
rail_width = 1;

def make_beam_profile(beam_width,beam_height,beam_thickness):
    cad.sketch()
    sketch = cad.getlastobj()

    cad.line(-beam_width/2.0,-beam_height/2.0,beam_width/2.0,-beam_height/2.0)
    cad.add(sketch,cad.getlastobj())
    cad.line(-beam_width/2.0,-beam_height/2.0,-beam_width/2.0,-beam_height/2.0+beam_thickness)
    cad.add(sketch,cad.getlastobj())
    cad.line(-beam_width/2.0,-beam_height/2.0+beam_thickness,-beam_thickness/2.0,-beam_height/2.0+beam_thickness)
    cad.add(sketch,cad.getlastobj())
    cad.line(beam_thickness/2.0,-beam_height/2.0+beam_thickness,beam_width/2.0,-beam_height/2.0+beam_thickness)
    cad.add(sketch,cad.getlastobj())
    cad.line(beam_width/2.0,-beam_height/2.0+beam_thickness,beam_width/2.0,-beam_height/2.0)
    cad.add(sketch,cad.getlastobj())
    
    cad.line(beam_thickness/2.0,-beam_height/2.0+beam_thickness,beam_thickness/2.0,beam_height/2.0-beam_thickness)
    cad.add(sketch,cad.getlastobj())
    cad.line(-beam_thickness/2.0,-beam_height/2.0+beam_thickness,-beam_thickness/2.0,beam_height/2.0-beam_thickness)
    cad.add(sketch,cad.getlastobj())
    
    cad.line(-beam_width/2.0,beam_height/2.0,beam_width/2.0,beam_height/2.0)
    cad.add(sketch,cad.getlastobj())
    cad.line(-beam_width/2.0,beam_height/2.0,-beam_width/2.0,beam_height/2.0-beam_thickness)
    cad.add(sketch,cad.getlastobj())
    cad.line(-beam_width/2.0,beam_height/2.0-beam_thickness,-beam_thickness/2.0,beam_height/2.0-beam_thickness)
    cad.add(sketch,cad.getlastobj())
    cad.line(beam_thickness/2.0,beam_height/2.0-beam_thickness,beam_width/2.0,beam_height/2.0-beam_thickness)
    cad.add(sketch,cad.getlastobj())
    cad.line(beam_width/2.0,beam_height/2.0-beam_thickness,beam_width/2.0,beam_height/2.0)
    cad.add(sketch,cad.getlastobj())
    
    cad.reorder(sketch);
    return sketch

def make_square_beam_profile(beam_width,beam_height):
    cad.sketch()
    sketch = cad.getlastobj()

    cad.line(-beam_width/2.0,-beam_height/2.0,beam_width/2.0,-beam_height/2.0)
    cad.add(sketch,cad.getlastobj())

    cad.line(beam_width/2.0,-beam_height/2.0,beam_width/2.0,beam_height/2.0)
    cad.add(sketch,cad.getlastobj())

    cad.line(beam_width/2.0,beam_height/2.0,-beam_width/2.0,beam_height/2.0)
    cad.add(sketch,cad.getlastobj())

    cad.line(-beam_width/2.0,beam_height/2.0,-beam_width/2.0,-beam_height/2.0)
    cad.add(sketch,cad.getlastobj())
    
    cad.reorder(sketch);
    return sketch

def getprofile(type):
    if type == "i":
       return  make_beam_profile(beam_width,beam_height,beam_thickness)    
    else:
       return  make_square_beam_profile(rail_width,rail_height)

def make_quarter_beam(type,radius):
    profile = getprofile(type)
    #cad.extrude(profile,10)
    cad.arc(-radius,0,0,.0001,math.pi/2,math.pi,0,1,0)
    cad.linearc2wire(cad.getlastobj())
    wire = cad.getlastobj()
    cad.pipe(wire,profile);
    return cad.getlastobj()

def make_beam(type,length):
    profile = getprofile(type)
    cad.extrude(profile,length)
    return cad.getlastobj()
    

def make_track(type,line_length,dead_zone_width,bend_radius):
    cad.group()
    group = cad.getlastobj()
    
    beam1 = make_beam(type,line_length)
    beam2 = make_beam(type,line_length)
    cad.translate(beam2,-dead_zone_width-bend_radius*2,0,0)
    beam3 = make_quarter_beam(type,bend_radius)
    beam4 = make_beam(type,dead_zone_width)
    cad.rotate(beam3,0,0,0,0,1,0,math.pi/2)
    cad.rotate(beam4,0,0,0,0,1,0,-math.pi/2)
    cad.translate(beam3,-bend_radius-dead_zone_width,0,-bend_radius)
    cad.translate(beam4,-bend_radius,0,-bend_radius)
    
    beam5 = make_quarter_beam(type,bend_radius)
    beam6 = make_beam(type,dead_zone_width)
    cad.rotate(beam5,0,0,0,0,1,0,-math.pi/2)
    cad.rotate(beam6,0,0,0,0,1,0,-math.pi/2)
    cad.translate(beam5,-bend_radius,0,line_length+bend_radius)
    cad.translate(beam6,-bend_radius,0,line_length+bend_radius)
    
    beam7 = make_quarter_beam(type,bend_radius)
    cad.rotate(beam7,0,0,0,0,1,0,math.pi)
    cad.translate(beam7,-bend_radius*2-dead_zone_width,0,line_length)
    
    beam8 = make_quarter_beam(type,bend_radius)
    
    
    cad.add(group,beam1)
    cad.add(group,beam2)
    cad.add(group,beam3)
    cad.add(group,beam4)
    cad.add(group,beam5)
    cad.add(group,beam6)
    cad.add(group,beam7)
    cad.add(group,beam8)
    return group	

def run():
    make_track("i",line_length,dead_zone_width,bend_radius)
    
    bot_offset = -rail_height/2-beam_height/2-bottom_overhang-bearing_clearance-bearing_height
    offset = dist_between_bearings/2 + rail_width/2
    track2 = make_track("s",line_length,dead_zone_width,bend_radius-offset)
    cad.translate(track2,-offset,bot_offset,0)
    
    track2 = make_track("s",line_length,dead_zone_width,bend_radius+offset)
    cad.translate(track2,offset,bot_offset,0)


run()

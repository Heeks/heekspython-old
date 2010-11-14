def ikeaize(ox,oy,oz,width,depth,thick,hole1dist,hole2dist,hole3dist,doweldist,dowelhole,camhole):
   import HeeksPython as cad
   cad.cuboid(10,10,0,width,depth,thick)
   cad.setcolor(191,240,191)
   shelf_slab2 = cad.getlastobj()
   cad.cylinder(ox + doweldist,depth - hole1dist,oz + thick - camdepth,camhole / 2,camdepth * 2)
   cad.setcolor(191,191,240)
   hole1 = cad.getlastobj()





$fn = 60;


dInterno=100;

union() {
    translate([0, 0, -9]) {
        // Fondo
        color([1.0, 1.0, 0.30])
            cube([9800, 8900, 2.9], center=true);

        // Campo di gioco
        color([0.90, 0.90, 1.0])
            cube([1800, 900, 3], center=true);
    }
    color([1.0, 1.0, 0.0]) {
        translate([0, -50, 0])
        hull() {
            translate([-70, -55, 60]) 
                cylinder(d=10, h=3);
            translate([-70, 55, 60]) 
                cylinder(d=10, h=3);
            translate([70, 0, 60]) 
                cylinder(d=10, h=3);
        }
    }
    color([.75, .75, .75, 0.3]) {
        difference() {
            /**/
            union() {
                cylinder(d=300, h=3);
                translate([0, 0, 38])
                    cylinder(d=300, h=3);
                translate([0, 0, 1])
                    cylinder(d=dInterno-1, h=37);
            }
            /**/
            /**/
            union() {
                translate([0, 0, -5]) {
                    difference() {
                        cylinder(d=220, h=55);
                            translate([-20, -125, -1])
                                cube([40, 250, 57]);
                            translate([-125, -20, -1])
                                cube([250, 40, 57]);
                            translate([0, 0, -2])
                               cylinder(d=dInterno, h=60);
                    }
                    difference() {
                        translate([0, 0, 9])
                            cylinder(d=dInterno, h=32);
                        translate([0, 0, -4])
                            cylinder(d=dInterno-6, h=63);
                    }
                    cylinder(d=35, h=60);
                    translate([-3, -25, 0])
                        cube([6, 50, 60]);
                    translate([-25, -3, 0])
                        cube([50, 6, 60]);
                }
            }
            /**/    
        }
    }
    
    color([.25, .25, .25]) {
        difference() {
            translate([0, 0, 3.1])
                cylinder(d=250, h=34.8);
            translate([0, 0, 1])
                cylinder(d=dInterno, h=37);
        }
    }
        
} 
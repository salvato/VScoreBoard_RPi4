module playField() {
    union() {
        // Fondo
        color([1.0, 1.0, 0.30])
            cube([9800, 8900, 2.9], center=true);
        
        // Campo di gioco
        color([0.90, 0.90, 1.0])
            cube([1800, 900, 3], center=true);
        
        // Linee
        color("blue") {
            cube([40, 900, 3.1], center = true);
            translate([0,-445, 0])
                cube([1800, 40, 3.1], center=true);
            translate([0, 445, 0])
                cube([1800, 40, 3.1], center=true);
            translate([0, 445, 0])
                cube([1800, 40, 3.1], center=true);
               
            translate([295, 0, 0])
                cube([40, 900, 3.1], center = true);
            translate([-295, 0, 0])
                cube([40, 900, 3.1], center = true);
               
            translate([895, 0, 0])
                cube([40, 900, 3.1], center = true);
            translate([-895, 0, 0])
                cube([40, 900, 3.1], center = true);
        }
        // Pali
        color("orange") {
            translate([0,-545, 0])
                cylinder(d=40, h=250);
            translate([0, 545, 0])
                cylinder(d=40, h=250);
        }
        
        // Rete
        color("white") {// Bande
            translate([0, 0, 225])
                cube([1, 1090, 25], center=true);
            translate([0, 0, 125])
                cube([1, 1090, 25], center=true);
             translate([0, 445, 175])
                cube([1, 25, 100], center=true);
             translate([0,-445, 175])
                cube([1, 25, 100], center=true);
      }
      color("red") {
          for(y=[20:40:860]) {
              translate([0, y-440, 175])
                cylinder(d=4, h=100, center=true);
          }
          for(z=[20:40:80]) {
                translate([0, 0, 120+z])
                    rotate([90, 0, 0])
                        cylinder(d=4, h=885, center=true);
          }
      }
    }
}

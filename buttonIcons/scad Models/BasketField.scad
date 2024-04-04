$fn = 80;
module basketField() {
    fieldColor = [0.15, 0.15, 1.0];
    lineColor =  [1.0, 1.0, 0.5];
    
    union() {
        color(fieldColor)
            cube([2800,1500,1],center=true);
        color(lineColor) {
            // Linea di metà campo
            cube([50, 1500, 1.1],center=true);
            difference() {
                cylinder(r=180, h=1.1);
                cylinder(r=130, h=1.2);
            }
            // 
            translate([1400-580, 0, 0])
                difference() {
                    cylinder(r=180, h=1.1);
                    cylinder(r=130, h=1.2);
                }
            translate([-1400+290, 0, 0])
                difference() {
                    cube([580, 490, 1.1], center=true);
                    cube([480, 390, 1.2], center=true);
                }
            translate([1400-290, 0, 0])
                difference() {
                    cube([580, 490, 1.1], center=true);
                    cube([480, 390, 1.2], center=true);
                }
            translate([-1400+580, 0, 0])
                difference() {
                    cylinder(r=180, h=1.1);
                    cylinder(r=130, h=1.2);
                }

            // Linee esterne
            translate([0, 725, 0])
                cube([2800, 50, 1.1],center=true);
            translate([0,-725, 0])
                cube([2800, 50, 1.1],center=true);
            translate([1375, 0, 0])
                cube([50, 1500, 1.1],center=true);
            translate([-1375, 0, 0])
                cube([50, 1500, 1.1],center=true);
            
        }
    }
}

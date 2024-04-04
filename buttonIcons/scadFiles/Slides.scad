$fn= 180;
union() {
    /**/
    color([0.8, 0.85, 1.0]) {
        intersection() {
            translate([0, -350, 0])
                cube([1600, 700, 900], center=true);
            scale([1.0, 0.05, 1.0])
                sphere(d=1900);
        }
    }
    /**/
    color([1.0, 1.0, 1.0]) {
        difference() {
            cube([1800, 50, 1100], center= true);
            cube([1599, 60, 899], center= true);
            translate([-850,-170,0])
                cylinder(d=300, h=1000, center=true);
            translate([850,-170,0])
                cylinder(d=300, h=1000, center=true);
            translate([0,-170, 500])
                rotate([0, 90, 0])
                    cylinder(d=300, h=1760, center=true);
            translate([0,-170,-500])
                rotate([0, 90, 0])
                    cylinder(d=300, h=1760, center=true);
        }
    }
}
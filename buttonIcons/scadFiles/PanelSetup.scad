$fn = 80;

include <MCAD/involute_gears.scad>

font1 = "FreeSans:style=Bold";
font2 = "FreeSans:style=Bold";

fontSize1 = 100;
fontSize2 = 200;

union() {
    /**/
    color([0.1, 0.1, 0.1]) {
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
    /**/
    color([2.0, 2.0, 0.0])
        translate([-450, -10, 300])
            rotate([90, 0, 0])
                linear_extrude(height = 50.0) 
                    text("Home",
                    font = font1, 
                    size = fontSize1, 
                    halign = "center");
    
    color([2.0, 2.0, 0.0])
        translate([400, -10, 300])
            rotate([90, 0, 0])
                linear_extrude(height = 50.0) 
                    text("Guests",
                    font = font1, 
                    size = fontSize1, 
                    halign = "center");
    
    color([2.0, 2.0, 0.0])
        translate([-450, -10, -330])
            rotate([90, 0, 0])
                linear_extrude(height = 50.0) 
                    text("21",
                    font = font1, 
                    size = fontSize2, 
                    halign = "center");
    
    color([2.0, 2.0, 0.0])
        translate([400, -10,-330])
            rotate([90, 0, 0])
                linear_extrude(height = 50.0) 
                    text("19",
                    font = font1, 
                    size = fontSize2, 
                    halign = "center");
}
/**/
color([0.8, 0.8, 2.2])
    translate([0, -100, 0])
        rotate([90, 0, 0])
            scale([40, 40, 40])
                gear (number_of_teeth=12,
                      circular_pitch=300,
                      bore_diameter=7,
                      hub_diameter=12,
                      rim_width=15,
                      hub_thickness=3,
                      rim_thickness=6,
                      gear_thickness=4,
                      pressure_angle=28);
/**/

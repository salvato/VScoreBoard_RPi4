
module player(hairColor, skinColor, shirtColor, pantsColor, kneeColor, shoeColor) {
    // Head
    color(hairColor)
        translate([0, 0, 205])
            sphere(d=30);
    // Collo
    color(skinColor)
        translate([0, 0, 185])
            cylinder(d1=14, d2=16, h=10);
    // Busto
    color(shirtColor)
        translate([0, 0, 115])
            scale([0.5, 1, 1])
                cylinder(d1=50, d2=60, h=70);
    // Braccia
    color(shirtColor) {
        translate([0,-30.0, 175])
            scale([0.75, 0.75, 1])
                sphere(d=20);
        translate([0, 30.0, 175])
            scale([0.75, 0.75, 1])
                sphere(d=20);
        translate([0,-30.0, 160])
            cylinder(d=15, h=15);
        translate([0, 30.0, 160])
            cylinder(d=15, h=15);
    }
    color(skinColor) {
        translate([0,-30.0, 135])
            cylinder(d1=12, d2=15, h=25);
        translate([0, 30.0, 135])
            cylinder(d1=12, d2=15, h=25);
        translate([0,-30.0, 110])
            cylinder(d1=8, d2=12, h=25);
        translate([0, 30.0, 110])
            cylinder(d1=8, d2=12, h=25);
    }
    // Mani
    color(skinColor) {
        translate([0,-30.0, 100]) {
            scale([0.35, 0.75, 1])
                cylinder(d1=15, d2=12, h=10);
            translate([0, 2.0, -8.5])
                cylinder(d=2.0, h=8.5);
            translate([0, 0.0, -12])
                cylinder(d=2.0, h=12);
            translate([0, -2.0, -9])
                cylinder(d=2.0, h=10);
            translate([0, -4.0, -7])
                cylinder(d=2.0, h=7);
        }
        translate([0, 30.0, 100]) {
            scale([0.35, 0.75, 1])
                cylinder(d1=15, d2=12, h=10);
            translate([0, -2.0, -8.5])
                cylinder(d=2.0, h=8.5);
            translate([0, 0.0, -12])
                cylinder(d=2.0, h=12);
            translate([0, 2.0, -9])
                cylinder(d=2.0, h=10);
            translate([0, 4.0, -7])
                cylinder(d=2.0, h=7);
        }
    }
    // Pantaloncini
    color(pantsColor)
        translate([0, 0, 85])
            scale([0.75, 1, 1])
                cylinder(d1=55, d2=50, h=30);
    // Gambe
    color(skinColor) {
        translate([0,-12.0, 50])
            scale([1, 1, 1])
                cylinder(d2=25, d1=15, h=35);
        translate([0, 12.0, 50])
            scale([1, 1, 1])
                cylinder(d2=25, d1=15, h=35);
        translate([0,-12.0, 5])
            scale([1, 1, 1])
                cylinder(d2=15, d1=10, h=35);
        translate([0, 12.0, 5])
            scale([1, 1, 1])
                cylinder(d2=15, d1=10, h=35);
    }
    color(kneeColor) {
        translate([0,-12.0, 45])
            sphere(d=18);
        translate([0, 12.0, 45])
            sphere(d=18);
    }
    // Scarpette
    color(shoeColor) {
        intersection() {
            translate([0,-12.0, 0])
                scale([1, .35, .25])
                    sphere(d=45);
            translate([-6,-17.5, -0.1])
                cube([46, 11, 6]);
        }
        intersection() {
            translate([0, 12.0, 0])
                scale([1, .35, .25])
                    sphere(d=45);
            translate([-6, 6.5, -0.1])
                cube([46, 11, 6]);
        }
    }
}

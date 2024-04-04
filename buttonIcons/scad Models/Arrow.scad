$fn = 80;
module arrow() {
    union() {
        cylinder(d=200, h=1400, center=true);
        translate([0, 0, 699])
            cylinder(d1=400, d2=0, h=350);
    }
}

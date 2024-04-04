$fn = 80;
include <BasketField.scad>;
include <Arrow.scad>

union(){
    basketField();
    translate([0, -1000, 300])
        rotate([0, 90, 0])
            arrow();
    translate([0, 1000, 0])
        rotate([0, -90, 0])
            arrow();
}
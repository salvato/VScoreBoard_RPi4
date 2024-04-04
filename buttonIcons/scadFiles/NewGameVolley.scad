
include <VolleyPlayer.scad>
include <VolleyField.scad>


// player(hairColor, skinColor, shirtColor, pantsColor, kneeColor, shoeColor);

union() { 
    seed = 421;
    skinColor1 = rands(0, 2, 12, seed);
    skinColor2 = rands(0, 2, 12, seed+44);
    skinColors= ["brown", "pink"];
    playField();
    for(i=[0:11]) {
        translate([75*i+50, 500, 0])
            rotate([0, 0, -90])
                player(skinColors[skinColor1[i]], skinColors[skinColor1[i]], "yellow", "darkblue", "yellow", "white");
    }
    for(i=[0:11]) {
        translate([-75*i-50, 500, 0])
            rotate([0, 0, -90])
                player(skinColors[skinColor2[i]], skinColors[skinColor2[i]], "red", "white", "white", "white");
    }
}
$fn=60;

include <VolleyPlayer.scad>
include <VolleyField.scad>


// player(hairColor, skinColor, shirtColor, pantsColor, kneeColor, shoeColor);

union() {
    seed = 421;
    skinColor1 = rands(0, 2, 12, seed);
    skinColor2 = rands(0, 2, 12, seed+44);
    skinColors= ["brown", "pink"];
    playField();
    // Team in Ricezione
    translate([525, -350, 0])
        rotate([0, 0, 180])
           player(skinColors[skinColor1[0]], skinColors[skinColor1[0]], "yellow", "darkblue", "yellow", "white");
    translate([625, -250, 0])
        rotate([0, 0, 180])
           player(skinColors[skinColor1[1]], skinColors[skinColor1[1]], "yellow", "darkblue", "yellow", "white");
    translate([700, -150, 0])
        rotate([0, 0, 180])
           player(skinColors[skinColor1[2]], skinColors[skinColor1[2]], "yellow", "darkblue", "yellow", "white");
    translate([720, 0, 0])
        rotate([0, 0, 180])
           player(skinColors[skinColor1[4]], skinColors[skinColor1[4]], "yellow", "darkblue", "yellow", "white");
    translate([600, 150, 0])
        rotate([0, 0, 180])
           player(skinColors[skinColor1[5]], skinColors[skinColor1[5]], "yellow", "darkblue", "yellow", "white");
    translate([680, 200, 0])
        rotate([0, 0, 180])
           player(skinColors[skinColor1[6]], skinColors[skinColor1[6]], "yellow", "darkblue", "yellow", "white");
    // Team in Battuta
    translate([-75, 0, 0])
        player(skinColors[skinColor2[1]], skinColors[skinColor2[1]], "red", "white", "white", "white");
    translate([-75, -100, 0])
        player(skinColors[skinColor2[2]], skinColors[skinColor2[2]], "red", "white", "white", "white");
    translate([-75, 100, 0])
        player(skinColors[skinColor2[3]], skinColors[skinColor2[3]], "red", "white", "white", "white");
    translate([-475, 0, 0])
        player(skinColors[skinColor2[4]], skinColors[skinColor2[4]], "red", "white", "white", "white");
    translate([-575, 250, 0])
        player(skinColors[skinColor2[5]], skinColors[skinColor2[5]], "red", "white", "white", "white");
    translate([-975,-250, 0])
        player(skinColors[skinColor2[6]], skinColors[skinColor2[6]], "red", "white", "white", "white");
}
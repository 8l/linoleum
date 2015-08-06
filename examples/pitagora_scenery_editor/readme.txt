
	Pitagora's Scenery Editor
	or PSE for short.
	-------------------------

    *	Short instructions.

	This is short 'cos the PSE was structured to be efficient,
	but not very user-friendly for how people commonly considers
	an user-friendly application. The PSE is a developement tool,
	targeted to creation of highly optimized 3D sceneries.
	It is, pratically, sort of "low-level scenery editor",
	where you can manage the virtual space at its fundamental
	levels (triangles in the first place). It's been mainly coded
	for the purpose of editing irregular shapes which cannot be
	built from programs' code, so don't expect to find primitives
	for solids such as spheres, cylinders and suchs...

    *	The space.

	PSE works with Pitagora's space conventions.
	Assuming you're seeing that "grid" as the program opens:
	 - the X axis goes from left to right;
	 - the Y axis comes from "outside" the screen and goes "inside";
	 - the Z axis goes from bottom to top.
	As the program opens, you're facing the grid from "above" it.

    *	PSE interface.

	Apart from a small set of pushbuttons placed in the upper-left
	corner of the PSE display, and from mouse controls which will
	be explained later, the PSE only accepts commands from the
	keyboard. This simplifies programming, for I don't have time
	or reasons to care for pull-down menus, sliders and so on.
	The pushbuttons are:

	H S M T R

	Apart from "H", which is to be clicked as normal,
	the others are to be held down while dragging the mouse
	to perform their duties... explanations are short, and
	after all you just have to try them out to see what they do.

	H: Brings the viewpoint to its "home position".
	S: "Strafes" along the X-Z plane.
	M: Moves the viewpoint towards the direction pointing to the
	   centre of the display (mouse Y axis) or strafes in the
	   perpendicular direction (mouse X axis).
	T: Turns around.
	R: Resizes the display.

    *	The primary reference plane.

	It is that wide grid giving the scenery's "horizon".
	Its center is marked and gives the origin of the 3D space,
	being for X=0, Y=0, Z=0. Its purpose is just to stay there
	and serve as a reference point. It can be turned on/off by
	pressing the space bar.

    *	The secondary reference plane.

	When the PSE is started, this other plane is not visible,
	because it overlaps the primary reference plane. It becomes
	visible if you press A, Z, S or X. In facts:

		A - moves secondary reference plane 1 unit up;
		Z - moves secondary reference plane 1 unit down;
		S - moves secondary reference plane 10 units up;
		X - moves secondary reference plane 10 units down;

		also

		T - brings back the secondary ref. plane over the primary,
		    therefore returning it to its initial position.

	The secondary reference plane is used to select points in space.
	If you click the mouse over the secondary reference plane, the
	respective point will be selected (identified by 3 coordinates,
	X and Z selected by the mouse, and Y selected by the position of
	the secondary reference plane).

	Therefore, moving the secondary reference plane up and down,
	and then clicking it, you should be able to access all of the
	points in the virtual space. Now, the grid has a fixed resolution
	of 512x512 "virtual length units", say VLU, and can be moved up
	and down without further limitations. The units mentioned while
	explaining the A, Z, S and X keys are also VLUs.

    *	Adding and removing markers.

	"Markers" are small arrows indicating some points in space.
	The green marker appearing when you click the secondary reference
	place could be an example, but that marker is moveable and is only
	used to target the points while you work. A real "marker", instead,
	is an arrow appearing when, after targeting a certain point, you
	hit the Q key. You'll see a smaller cyan arrow appearing over the
	green arrow. Now, if you target another point and press Q again,
	another marker will be fixed, and so on... until a maximum of 1024
	markers.

	Markers are used to fix polygon's vertexs before building the
	polygons themselves. Markers can be:

	 - deleted one by one, by pressing the W key;
	 - deleted all at once, by pressing the E key;
	 - moved along X and Z with cursor arrows;
	 - moved along Y (up and down) with PgUp and PGDn.

	Also, when you fixed 2 or more markers, you can enlarge and
	shrink the shape outlined by the markers by pressing + or -

    *	Once you fixed some markers, you can build polygons in 2 ways.
	The first way is by "raising walls", the other is "pure polygons".

    *	Raising walls:
	 - fix 2 or more markers WITHOUT moving the secondary ref. plane;
	 - move the secondary reference plane UP to the level which will
	   give the "height" of the wall;
	 - press the R key.

	Now, you have to first know about a certain rule regarding vectors
	in space. It is called "rule of the right hand". It says that if
	there are two vectors oriented like the middle finger and the thumb
	of one's right hand, the third vector given by their cross product,
	will be oriented in the direction of the index finger.
	Ok, if you've not studied vectors, I know it sounds obscure, but
	just don't worry 'cos you don't need to study all of the vectors'
	theory to understand how to raise walls with the PSE. POLYGONS have
	ONLY ONE VISIBLE SIDE, and I mentioned that rule just to let you
	know that *there is a reason* if the visible side is a certain one
	AND it depends on how you displace the points.

	Said that, just remember this other rule (a lot more simple to
	understand, but deriving from the rule of the right hand). Now,
	consider you're building a closed shape, like a perimeter...

	 - if you displace the points in clockwise order, the visible
	   side of the wall will be the internal side;
	 - if you displace the points in anticlockwise order, the
	   visible side of the wall will be the external side.

	Try it out for more clearance...

    *	Pure polygons:
	 - fix 3 or more markers, at any position;
	 - press the Y key.

	By the same (yeah, boring) rule of the right hand, the visible
	side of each polygon built in this way will be the one facing
	which the vertexs appear in clockwise order. Try, try! The best
	way is to learn it by doing it...

    *	Ok, now (when you will have practiced those things) you know
	enough to build the shapes. Now, let's say you want to make
	some changes to the existing polygons.

	For instance, you'll notice they're all green.
	That's the default color... you can change it, but first you
	have to learn how to select a polygon. It's simply that:

	 - click over the polygon's surface to select corresponding
	   polygon; the selected polygon will be highlighted and 3
	   letters will appear near its vertex... the letters' meaning
	   is to be explained later, so for now have some patience.

	Only one polygon can be selected at a time, at least in this
	first version of the PSE. If you select another polygon, it
	will de-select the previously selected one.

	To make changes to the selected polygon, there are a number of
	ways. They're explained below...

    *	Deleting polygons:
	 - select the target polygon;
	 - press the H key.

	This won't exactly delete the polygon. It will "mark it for
	future deletion". You can think to this operation like if the
	polygon had been hidden, or thrown in a trashcan. But hidden
	polygons are still selectable (if you can remind where they
	are, for you can't see them anymore) and can be restored into
	"existence" by pressing H again once selected.

	To effectively delete hidden polygons, you'll have to also
	press the N key. N stands for "narrow buffers". It will physically
	remove all hidden polygons from memory, and also will unload any
	eventual textures that were attached to them, UNLESS those textures
	were used by other polygons still present in the scenery.
	Later I'll give more informations about loading textures...

    *	Changing polygon's color:
	 - select the target polygon;
	 - press the # (hash) key;
	 - type a HEX color value;
	 - press the ENTER key.

	If you change the color while a polygon is selected, you'll be
	changing that polygon's color. If no polygons are selected and
	you hit the # key, you'll be changing the default color for
	all future polygons. Now, if you know how things go with HEX
	color values, you can skip the following paragraph...

	Hex values are in 24-bit scheme. The same used by HTML documents.
	It means that the first two digits give the RED component of the
	final color, the second two digits give the GREEN component, and
	the third pair of digits give the BLUE component. Each pair is an
	HEX number. Here are some examples:

		FF - is the highest possible number with 2 hex digits,
		     and it means 255 in decimal;
		80 - means 128 in decimal;
		40 - means 64 in decimal;
		1F - means 31 in decimal;
		06 - means 6 (also) in decimal;
		00 - means zero (also) in decimal.

	The numbers can be converted to decimal by knowing that letters
	A, B, C, D, E and F mean, respectedly, 10, 11, 12, 13, 14 and 15.
	They are "a way to write 10, 11, 12, 13, 14 and 15 using only one
	(hexadecimal) digit".

	At this point, hexadecimal means numbers in base 16,
	while decimal means numbers in base 10. In base 10,
	you build a two-digit number, for example by writing:

		65

	which means, mathematically, 6 x 10 + 5.
	In hexadecimal, you don't multiply 6 by 10.
	You have to multiply it by 16. So 65, if it was seen
	as an hexadecimal number, would mean 6 x 16 + 5 = 101.

	Now, if you take the FF number, and remember F means 15,
	you get: 15 x 16 + 15 = 255. That's how it's done.

	So, if you wanted a polygon's color to be formed by 25% red,
	50% green and 100% blue, you'd need the RED component to be
	set to the 25% of 255, which is "pratically" 64, the GREEN
	component to be set to the 50% of 255, which is 128, and the
	BLUE component to be set to the 100% of 255, which is 255.
	So the parameters to find the final 24-bit color code are:

		R = 64
		G = 128
		B = 255

	And translating them to hexadecimal, you get:

		R = 40
		G = 80
		B = FF

	now, you only have to put them one after the other, and you'll
	get the corresponding 24-bit color code, which will be:

		4080FF

	Here are further examples you should try to see how the colors'
	components affect the final color. They just follow optical rules.

		FFFFFF	is white
		000000	is black
		808080	is gray
		404040	is dark gray
		C0C0C0	is light gray

		FF0000	is "full" red
		00FF00	is "full" green
		0000FF	is "full" blue

		FFFF00	is yellow
		FF00FF	is magenta
		00FFFF	is cyan

	Translating decimal to hexadecimal, thought you don't really need
	it if you learn to "think in hexadecimal", you may do the following:

	 - divide the decimal number to be translated by 16;
	 - the integer part of the result will give you the left digit;
	 - now multiply the remainder of that division by 16;
	 - the result will give you the right digit.

	Of course, you have to translate a result of 10 into letter A,
	a result of 11 into letter B, and so on until 15 = F.
	An example... translating decimal number 165 to hex.

	 - 165 divided by 16 gives 10 with a remainder of 0.3125
	 - 10 is written as "A" in hexadecimal, so the left digit is A
	 - now, 0.3125 multiplied 16 is 5, so the the right digit is 5
	 - SO, the corresponding hex value is A5.

    *	Changing surface's attributes:
	you can see which attributes are used to build polygons because
	they are listed in the upper left corner of the PSE display.
	There can be one or more of the following notes:

		- Texture
		- ZBuffer
		- Blend
		- Shading
		- Spanned
		- Metal
		- DblFace
		- In trashcan
		- Composite
		- Part of CG: "XXXX"

	ZBuffer is toggled on/off by pressing key "1".
	Blend is toggled on/off by pressing key "2".
	Shading is toggled on/off by pressing key "3".
	Spanned is toggled on/off by pressing key "4".
	Metal is toggled on/off by pressing key "5".
	DblFace is toggled on/off by pressing key "6".

	The others are special, and are toggled on/off depending on
	some other actions performed on the polygon. Now, about the
	ones that can be toggled on/off at your choice...

	ZBuffer
	Specifies that the surface is to be covered by nearer surfaces.
	I think that's of vital importance unless you're drawing a
	totally FLAT shape or some sort of  background.
	So keep it always on.

	Blend
	Specifies that the surface is semi-transparent.
	It will merge the surface's color with the background.

	Shading
	Specifies that the surface's color is to be influenced by
	light sources. The default light source is located south-west.
	Light sources must be managed by the program that's going to
	play the scenery data. So the default light source serves just
	to "test" the polygons edited with PSE, and it's fixed.

	Spanned
	Specifies that the surface is going to be traced by spans,
	not in full-quality rendering. If you turn this off, you'll be
	slowing the graphics of the final scenery. It is advisable to
	keep it on, unless your scenery is going to be static (and not
	for real-time exploration) or executed on a very fast computer.
	Full-quality rendering pratically removes any graphics glitches,
	which normally appear as distortions in the textures when
	polygons are seen at high angles, or as missing dots near the
	edges of polygons. But there are really few of those glitches,
	and speed improves between 2 or 3 times when using spans.

	Metal
	Specifies that, providing the surface is shaded, Pitagora's going
	to use metal shading for that surface. Metal shading means more
	contrast in shading, and on a large number of polygons it makes
	objects look more reflective. When metal shading is off, the light
	diffuses more over the surface, simulating something more similar
	to plastic or wood.

	DblFace
	Specifies that BOTH sides of the polygon must be visible,
	ignoring the rule of the right hand. Well, be careful: even
	though it may look like a good idea to simplify things, building
	double-faced polygons is rarely applicable, because the polygons
	won't behave correctly in relation to shading and basic hidden
	surface removal. This means they'll also slow down rendering.
	While for shading, double-faced polygons will still be shaded in
	relation to their "normally visible" side, and the other side will
	look dark or light only if the visible side is dark or light.

    *	Applying textures.

	Textures are, of course, very important when building realistic
	sceneries. Basically, textures are pictures that can be stick to
	polygons' surfaces. Before you can stick one, however, you need to
	load the corresponding picture from a file. The file must be a
	TrueVision(TM) Targa file, having TGA extension, and it must be
	24 or 32-bit truecolor. If you don't have any painting tools for
	creating such files, please get one. ULead's Finishingtouch, Adobe
	Photodeluxe and MGI Photosuite are some examples...
	Also, textures cannot exceed 512x512 pixels, and try to keep them
	small enough if you're going to use a lot of different textures,
	because the memory available inside the PSE for loading textures
	is limited to 8 Megabytes complessively.
	Remember, finally, to PLACE THE TGA FILES inside the PSE directory,
	or they won't be accessible. PSE is a Linoleum program.

	Said all that, to load a texture from a file, press the F key.
	PSE will ask for the file name. No windowing file-selectors, so
	type in the correct name and add the TGA extension to it, separated
	by a single dot. An example: WALL.TGA

	Once you have loaded one or more textures, you can glue them to
	polygons. First, select the target polygon. There you'll see it
	highlighted by a semi-transparent representation of the currently
	selected texture. To select other textures you may have loaded,
	press the B key. B will rotate throught all of the loaded textures.

	Once you can see the proper texture appearing over the polygon,
	press the G key to glue it to the polygon's surface. That's all.

	Oh, by the way, if you want to detach the texture from a polygon,
	press the D key. And now for some more confusing informations :o)

    *	Parents and childs.

	This is quite complicated. Not of vital importance, but it's
	important if you want to make polygons that are not triangles.

	First of all, you have to know Pitagora always uses triangles
	to build sceneries. It is unavoidable if you want it to be a
	fast and reliable 3D engine. More complex shapes, and especially
	quadrilaterals, must be managed as "composite" polygons.

	Composite polygons are formed by a parent triangle, plus one or
	more child triangles. Generally, they are used to texture the
	walls (built by using the R key), where parent and child are set
	up automatically.

	Another thing: you see those four letters appearing when you
	select a polygon? They are O, X, Y and a lowercase d.
	They are the range indicators for the selected polygon.

	O is the origin vertex, the first vertex of the polygon.
	X makes the X range, corresponding to the second vertex.
	Y makes the Y range, corresponding to the third vertex.

	d, well, d is called the "domain" of the triangle, and is calculated
	as the dot product of vectors X-O and Y-O. For those who don't
	understand vectors, see the following scheme:

		Y...........d
	       ...	   .
	      .....	  .
	     .......	 .
	    .........	.
	   ........... .
	  O...........X

	The triangle to be considered is always the OXY triangle.
	Possible childs of that triangle must always fit into the
	"imaginary" edges connecting X with d and Y with d.
	If the surface of one or more child triangles gets out of those
	edges, the parts of the surfaces laying out of the range will
	not be shown by Pitagora, because they cannot be properly indexed.

	The vertexs of a certain polygon can be rotated (therefore changing
	the domain's extension) by pressing the O (change Order) key.
	Also the vertexs can be swapped in such a way that the visible side
	becomes the other side, if it's needed, by pressing the J key.

	Now, if you want to define a child triangle, build it so that it
	will fit the parent's domain range, and once you're done, you can
	set the parent and child triangles to form a composite shape by:

	 - selecting the parent;
	 - pressing the P key;
	 - selecting the child.

	After which, the triangles will merge in a composite polygon.
	The texture eventually pasted upon the parent's surface will
	extend and be shown over the childs.

    *	A very needed utility: "picking a vertex's position".

	It is invoked by pressing the V key.
	The result is that the selected point in space will move to the
	nearest vertex, in respect to the previous position of the green
	arrow which was marking the selected point. Also, a new marker
	will be added in the new position of the green arrow, as if you
	pressed the W key.

	I think there's no need to explain this function is a lot useful
	to join an arbitrary polygon to the edge of another arbitrary
	polygon.

    *	We're not done.
	Don't hurry. It's not really easy to create 3D models,
	what the heck do you believe 3D designers are paid for?
	Take a good look at the following 3 exercises. They may
	taste boring, but they explain important things.

    *	EXERCISE 1: BUILDING A CONVESS CUBE.

	It may look like a mess at the first attempt,
	but remember it's just a matter of practice...

	1.
	Mark four vertexs in anticlockwise order, and make sure they are at
	the corners of a perfect square (the grid will help you if you zoom
	in from the "home" position of the viewpoint).

	2.
	Press the C key.
	C stands for "Close figure".
	It will just add another marker overlapping the first marker.

	3.
	Raise the secondary reference plane to a level which equals the
	distance between the markers. Of course you may decide to be
	very accurate and count the VLUs separating the markers (well,
	remember 1 VLU is a single "pixel" on the grid), but it's not
	needed. Also, zooming until you almost "touch" the primary
	reference plane and then turning the camera by about 90 degree
	up, will surely help in seeing how much you're raising the
	secondary reference plane. You'll also notice the markers will
	turn to red. Nevermind that, it's normal.

	4.
	Press the R key.
	Well, it will build the first 4 sides of the cube.
	The sides will be visible from outside the cube itself,
	according to the rule of the right hand applied to that function.

	5.
	Press the Y key.
	Because you didn't move the markers, the same positions can be
	used to build the lower side of the cube. After you pressed Y,
	you may move the viewpoint below the primary reference plane to
	see the lower side of the cube.

	6.
	Lift the markers by pressing the PgUp key.
	You'll need to keep that key pressed until the markers will be
	coloured in cyan (no longer RED). Markers are coloured in cyan
	when they match the surface of the secondary reference plane.

	7.
	Press the Y key.
	This way you'll be building the upper side of the cube, BUT it
	won't be visible from above the cube, because the markers were
	selected in anticlockwise order. So there is one more step to do
	for getting the cube ready. But anyway, at this point you don't
	need the secondary reference plane and the markers anymore, so
	press E and T to erase all markers and bring the secondary
	reference plane to "ground level". Both the plane and the markers
	may just be rather encumbering at that point...

	8.
	Get inside the cube.
	From there you should be able to see only one of its sides,
	the upper one. It is WRONG that the upper side should be visible
	from inside the cube and not from the outside, SO...
	select the two triangles forming the upper side (one at a time),
	and for both of them press the J key. J is "Jump side" and it will
	reverse the order of the vertexs forming the triangles of the
	cube's upper side. Thus, the upper side will be finally visible
	from above the cube, as expected. You're done...

	At that point it would be nice to save the work.
	Well, press the DOT key (or period, if you prefere).
	PSE will prompt for the name of the scenery... say Cube.BIN
	and press ENTER... the .BIN extension is not needed, but it's
	advisable, because your scenery will be saved as plain binary
	data, so you could not open the file from applications (you
	may crash applications by opening files which have a certain
	extension but not containing what the application expects to
	find). You may invent a new extension to identify your sceneries,
	but it's your choice and you should not choose an extension which
	already means something to the system (for instance, TGA means a
	Targa image file).

    *	EXERCISE 2: TEXTURING THE CUBE.

	If you're done with Ex.1, prepare a texture file in TGA format.
	If you still can't, use the sample texture included with the PSE
	package, which is called WALL.TGA

	1.
	Load up the cube.
	Press the comma (,) and you'll be prompted for the scenery file
	to be loaded. Type Cube.BIN and press the ENTER key.

	2.
	Load the texture file.
	Press the F key. Say you want to load WALL.TGA, for instance,
	and when you typed the name press ENTER.

	3.
	Select one of the polygons forming the cube.
	For now, let's avoid selecting the upper and lower sides.
	Press the G key to glue the texture to the selected side,
	and repeat the thing for the other 3 sides.

	4.
	Try selecting one of the triangles forming the upper side.
	Press G to glue the texture and see what happens.

	5.
	Well, if you select the other triangle of the upper side and
	try to glue the texture, you'll be getting it textured but you'll
	notice the two triangles will be still divided, and look ugly.

	6.
	That's where we set parents and childs.
	Select one of the upper side's triangles.
	Look at where the "d" letter is shown.
	Press the "O" key (O, not zero) to rotate the vertexs until
	the "d" letter matches the missing vertex of the square shape
	being the upper side of the cube.

	7.
	Press the P key and select the other triangle forming the
	cube's upper side. The triangles will be merged and will
	form a square shape, now uniformly textured.

	8.
	Repeat steps 6 and 7 for the lower side of the cube,
	and you'll be done. Oh, since the lower side should be laying
	on the ground, and visible from outside the cube, you might be
	unable to select it until you move the viewpoint below the cube.

	This time, how is the scenery fully saved?
	Well, since you've got a texture, you also have to save it to
	a separate file called the "textures bank" for a certain scenery:
	the textures bank may contain one or MORE textures, upto 8 megs.
	Apart from saving the Cube.BIN again by pressing the DOT key,
	you'll also have to save the textures bank by pressing COLON
	(the two dots, the sign above the single DOT). Give a name to
	the textures bank that:
	 - associates it with the scenery file (call it Cube)
	 - explains it's a textures bank AND it's a binary file,
	   so append -TB.BIN (-TexturesBank.BINary) to the name
	 - so, call the textures bank "Cube-TB.BIN"

    *	EXERCISE 3: MAKING THE CUBE CONSISTENT.

	Well, the cube you've just designed is not consistent (eventually
	the players of a game containing that cube won't collide with the
	cube and will be allowed to ENTER the cube by simply crossing its
	sides) even if the texture gives the idea they're made of solid
	rock. It doesn't matter how the texture looks like, and the simple
	shape is not sufficient. "Collision control" is another issue, a
	totally different thing. So here are the basic rules:

	Assumption: SHAPES CAN BE IRREGULAR
		so they cannot be checked by using simple maths, so that
		for example you'd check the distance from the center of
		the object and say, when that distance drops below a
		certain value which is the "diameter" of the object, the
		player is colliding with that object. It could be that
		simple IF the only possible shape was a perfect sphere.
		Since shapes can be of any type, including irregulars,
		such maths based on the position of the player don't work,
		and you need something that's more complicated. Sorry.

	Solution: BUILDING "COLLISION GROUPS"
		so what are "collision groups"?
		They are shapes formed by polygons, exactly like the cube,
		but they're formed by SPECIAL polygons. Those polygons are
		not shown when the scenery is rendered (even though they
		can be displayed by the scenery editor, because you, the
		3D designer, need to see where collision groups are).
		The polygons being part of a collision group are shapes
		for which applies the following simple rule: whenever the
		viewpoint takes a position from which NONE OF THE POLYGONS
		forming the collision group is visible, then a collision is
		DETECTED. The rule is simple, but its consequences are not.

	Management:
		collision groups are to be edited inside the scenery editor,
		but they're finally managed by the program that will take
		care of rendering the scenery, moving the viewpoint, and so
		on... of course such programs will be helped in this quite
		difficult task by the subroutines from the "Scenery" library;
		quite unfortunately, however, I still didn't have enough time
		to code that library. I will do that as soon as possible.
		As long as you're just designing the scene, or if your duty
		is not coding the program that will render the scene, you
		just have to bear in mind that rule explained by the above
		paragraph. It's the only, and most important, thing to know
		about collision groups.

	How to create a collision group:
		in general (then we'll practice with the cube) every
		collision group needs an univoque identificator, so it's
		said to be the "collision group ID". The ID is formed by
		4 alphanumeric characters (or symbols, or signs, whatever
		in the ASCII code). When you create a new collision group,
		press the K key to select the group ID of the group you're
		going to create. This will be needed just a bit later.

	Creating the polygons that are part of the group:
		well, let's get back to our sample cube. Load it if you
		closed PSE, and eventually load the textures bank (but
		that's not really necessary to edit collision groups).
		[Load Scenery = comma key, Load Textures Bank = semicolon]
		Select the collision group ID by pressing the K key.
		You may choose the collision group ID "CUBE": it fits.
		Select each polygon of the cube (of course, one polygon
		at a time, as PSE wants it). For each polygon, press
		first the D key and then the M key. And why??
		D will "Duplicate the polygon", creating a perfect copy
		of it. You won't probably be aware of the second polygon
		'cos it will overlap the first entirely. But it will be
		there anyway. Also D will select the copy of the previously
		selected polygon as the newely selected polygon. When you
		press the M key at that point, it will mark the new polygon
		(a copy of the existing one) as part of current collision
		group (and current group is "CUBE"). You'll notice, after
		pressing M, that the new polygon intersects with the
		original polygon, because the one being part of the
		collision group will be visually marked by a pattern of
		black-and-yellow stripes. Now, when you've selected all
		of the cube's polygons and pressed D and M for each,
		the collision group for the cube will be ready.
		Save the scenery again and you're done.

	A tip:	you can hide collision groups by pressing the U key.
		"U" toggles collision groups visualization on/off.

	Quest:	what if I don't have a visible polygon to set as part of
		a collision group?
		It's easy: build the polygon using V to mark its vertexs
		(V marks the nearest vertex to the selected point, the
		selected point being the point identified by the tip of
		the green arrow), then build a pure polygon using Y, then
		select that new polygon and press just M to add the
		polygon to the collision group.
		Of course, in this case you don't need the D key.

	Remark: ALWAYS CLOSE COLLISION GROUPS!!
		The cube is a closed figure. So is its collision group.
		Remember THE RULE: a collision is detected when NONE OF
		THE POLYGONS in the same collision group are VISIBLE!

	Remark: SPLIT COMPLEX SHAPES IN MORE COLLISION GROUPS!!
		The cube is a simple figure. If you have, for instance,
		something looking like a torus (a "donut") you CANNOT
		select all of its polygons and assign them to the same
		collision group, because at least some of its polygons
		would be always visible. You have to split the torus in
		longitudinal sections and mark them by assigning each
		section to a different group ID. You don't have to split
		the physical shape, just the collision groups... that's
		also why I say BEWARE of making the viewpoint capable of
		reaching, in your program, complex shapes which may just
		form a 3D background or decorative scenery to be set
		aside from the interactive region of the scenery.
		Of course you could, but then you'd have to care for those
		complex shapes and of collisions with them, and it's quite
		a long and boring work... so just ask yourself: are you
		sure the program needs these complexities?

	----------------------------------------------------------------

    *	Well, there are further things that can be done, but they don't
	need to be fully explained... I guess. So here's a quick reference
	to all commands (all of the alphabetic keys are associated to a
	different command, plus there are . , : ; and some function keys).
	Read carefully and try to understand what all keys are for...

	SPACE BAR	Toggle primary reference plane;
	I		Toggle surface indexing;
	Q		Add Marker;
	W		Undo Marker;
	E		Erase All Markers;
	R		Raise Wall;
	T		Take Ground Level;
	Y		Pure Polygon;
	A		Raise SRP;
	Z		Drop SRP;
	S		Raise SRP more;
	X		Drop SRP more;
	D		Duplicate polygon;
	F		Fetch texture from disk;
	G		Glue texture to polygon;
	C		Close figure;
	V		Crop Vertex;
	B		Branch to next texture;
	H		Hide polygon;
	N		Narrow buffers;
	.		Save scenery;
	,		Load scenery;
	:		Save textures bank;
	;		Load textures bank;
	O		Change vertexs order;
	P		Parent of;
	L		Split composite polygon;
	J		Jump side;
	<		Send triangle to background (index 0);
	>		Send to foreground (index n-1);
	K		Select collision group ID;
	M		Mark for collision group;
	U		Toggle collision groups visulization;
	+		Magnify markers;
	-		Shrink markers;
	#		Set plain polygon color;
	|		Find infinity triangles;

	CR RIGHT	Translate markers right;
	CR LEFT 	Translate markers left;
	CR UP		Translate markers up (forward along Z, to north);
	CR DOWN 	Translate markers down (backward along Z, to south);
	PG UP		Translate markers up (along Y);
	PG DN		Translate markers down (along Y);

	F1		Set cooperative display mode (windowed mode);
	F2		Set 320x240 exclusive display mode;
	F3		Set 512x384 exclusive display mode;
	F4		Set 640x480 exclusive display mode.

	Oh, please note - the 512x384 display mode may NOT be working on
	all the video cards and monitors. It's not very standard, so also
	don't use it in a program as the ONLY possible mode. For instance,
	my own adapter can set it, BUT THEN it's misaligned on the monitor
	and the monitor's OSD says "unsupported mode".

	"Infinity triangles" are those triangles that tend to be aligned
	on a straight line, with all of their three vertexs. They sometimes
	occur by mistake, especially after forgetting to erase the markers
	before creating another polygon (not pressing E before fixing the
	new vertexs for the Y function). They're vicious, because they can't
	normally be selected or even be seen, but may cause glitches in an
	application's graphics using such a dirty scenery (dirty because it
	includes infinity triangles), and anyway they make the scenery file
	bigger than needed. To find and select (mostly for erasing them) the
	infinity triangles, the function associated with the pipe key (|) is
	provided: it will find the first infinity triangle. Normally, to
	clean them all out of the scenery, the thing to do is pressing | to
	match one of those, then H to hide it, then N to delete the hidden
	triangle; repeat |, H, N until no more infinity triangles are found.

	----------------------------------------------------------------

    *	It's a crap! I can't do this, I can't to that!

	You have the source. If you want to improve your personal copy
	of PSE, do it yourself. I told you: this program is not precisely
	"user-friendly", and in the beginning intended to be for my OWN
	work. Then I was talking about it with friends and I thought to
	make it available for everyone. Yet, it's a program I KNEW I would
	be able to use, and it doesn't take care about being easy to manage.

	Seriously however, there isn't really much to do for simplifying
	existing functions. You may ADD functions such as copy and paste,
	and so on... but after all, there aren't easier ways to build
	optimized sceneries. And of course, if you needed to duplicate a
	certain object, you could do that from the program that will
	manage the sceneries...

	----------------------------------------------------------------

    *	Limits:

		max. 65536 triangles (including collision groups)
		max. 8 megs of textures data

    *	Questions:

		http://anywherebb.com/tellalex.html

    *	Home:

		http://anywherebb.com/linoleum.html


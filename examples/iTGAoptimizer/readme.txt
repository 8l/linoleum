



	Well, this folder contains UNOPTIMIZED versions of the graphics parts forming
	the default iGUI skin ("Toffee"), and the program that was used to optimize
	them. The optimization consists in reducing the 256-color palette of TGA images
	so that the palette contains only the range of color indices that are used by
	at least a pixel of the picture. An index is associated with a RRGGBB triplet,
	so the savings are of 3 bytes per unused color removed. TGA format allows this
	by indicating which is the first color of the 256-color palette given in the
	file's color table, and how many colors the palette contains. However, it seems
	this kind of TGA picture isn't used very much, because none of the painting
	programs I have can actually handle optimized files properly, and that's why
	I'm keeping originals of the skin graphics in this folder.

	iTGAoptimizer works by simply dragging the icon of a 256-color TGA picture over
	the program's icon. If some slices of the palette from the beginning or from the
	end of the palette are unused, iTGAoptimizer will save back the file without
	those unused slices. Specifically for tiny pictures such as the iGUI's buttons,
	these savings turn out to be quite convenient.

	Note that some of the versions included in this folder are in truecolor 24-bit,
	and they'd need to be converted to 256 colors before attempting to optimize
	them again. However, for these specific pictures of course you don't need doing
	that again, because I already optimized all of them and saved their optimized
	versions within main/lib/igui for access from the igui library's stockfile. All
	of them are always included as part of the skin packet of an iGUI application.
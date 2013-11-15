
static char *intro_help[] =
{
  "",
  "@C1@l@c Introduction",
  "",
  "@C4@bRNA Movies",
  "is a system for the visualization of RNA secondary structure spaces. Its input",
  "is a script consisting of primary and secondary structure information.  From",
  "this script, the system fully automatically generates animated graphical",
  "structure representations. In this way, it creates the impression of an RNA",
  "molecule exploring its own 2D structure space.",
  "",
  "@C4@bRNA Movies",
  "has been used to generate animations of a switching structure in the spliced",
  "leader RNA of L. collosoma and sequential foldings of PSTV transcripts.",
  "",
  "Demonstrations of the animations mentioned here can also be viewed on our",
  "Bioinformatics Webserver under the following address:",
  "@C1@fhttp://BiBiServ.TechFak.Uni-Bielefeld.DE/rnamovies/",
  NULL
};

static char *navigation_help[] =
{
  "",
  "@C1@l@cNavigating in an RNA Movie",
  "",
  "@C4@bRNA Movies",
  "@bprovides the well established metaphors of multimedia",
  "@bpresentation, displaying the secondary structure animation as a",
  "@bmovieplayer.",
  "",
  "Use the buttonbar navigation panels",
  "",
  "@f@bPause Play Forward Backward Rewind",
  "",
  "buttons to guide the animation process. The scrollbar above the",
  "movie canvas displays the progress and can also be used to move",
  "directly to a certain part of the movie script.",
  "",
  "The frame counter below the movie canvas indicates the currently",
  "displayed absolute movie frame. The structure counter displays",
  "the two structures being interpolated or the current structure",
  "number.",
  "",
  "@C4@bJump to frame",
  "by clicking on the frame counter.",
  "",
  "@C4@bJump to structure",
  "by clicking on the structure counter.",
  NULL
};

static char *speed_help[] =
{
  "",
  "@C1@l@cSetting the Speed of a Movie",
  "",
  "@bClick on the rate counter below the movie canvas to",
  "@bset the frame rate in milliseconds.",
  NULL
};

static char *saving_help[] =
{
  "",
  "@C1@l@cSaving an RNA Movie",
  "",
  "@bSaving here, means exporting into foreign format.",
  "",
  "@C4@bSave",
  "To save choose the save option in the file pulldown menue.",
  "",
  "@C4@bAdjusting Size",
  "You can change the size of the frame indepently of the movie canvas",
  "using the X-Size and Y-Size counters.",
  "",
  "@C4@bWindow Size",
  "press this button to readjust the output size to the movie canvas size.",
  "The Movie canvas stays active while the save dialog is displayed.",
  "So you may readjust the size, zoom, panning, rotation of the frame.",
  "",
  "@C4@bFrames",
  "The Frames area is used to choose the range of frames to be saved.",
  "",
  "@C4@bFile Format",
  "Choose the approriate file format.",
  "@f@bGIF: Bitmap Format (Graphics Interchange Format)",
  "@f@bEPS: Vector Format (Encapsulated PostScript)",
  "The \"Use Background\" Option colors the background (transparent, otherwise).",
  "@f@b2D Coords: Data Dump Format (Allways Complete Movie)",
  "",
  "@C4@bPath",
  "The directory into which the files will be saved.",
  "",
  "@C4@bPrefix",
  "The prefix name the saved files.",
  "Filenames are of the form:",
  "@f@b<prefix>_<frame number>.<file id>",
  NULL
};

static char *snap_help[] =
{
  "",
  "@C1@l@cSnapshot",
  "",
  "@bThe snapshot button saves the current frame",
  "@bin the format chosen in the save dialog.",
  NULL
};

static char *print_help[] =
{
  "",
  "@C1@l@cPrinting",
  "",
  "@bThe printing dialog prints PostScript using the print command.",
  "",
  "@C4@bPrint command",
  "Default: \"lpr %s\" the %s is replaced by the temporary filename",
  "of the PostScript file",
  NULL
};

static char *zoom_help[] =
{
  "",
  "@C1@l@cPanning, Zooming, and Rotating Frames",
  "",
  "@C4@bPanning",
  "Press the left mousebutton in the movie canvas.",
  "Move the structure by dragging the mouse.",
  "@f@bShift + left mousebutton resets coordinates.",
  "",
  "@C4@bZoom",
  "Press the middle mousebutton in the movie canvas.",
  "Zoom the structure by dragging the mouse up or down.",
  "@f@bShift + middle mousebutton resets to the initial zoom factor.",
  "",
  "@C4@bRotate",
  "Press the right mousebutton in the movie canvas.",
  "Rotate the structure by dragging the mouse up or down.",
  "@f@bShift + right mousebutton realigns the display.",
  "",
  "@C4@bReset",
  "to reset all view parameters to their initial values press",
  "@f@bCtrl + left mouse button",
  NULL
};

static char *interp_help[] =
{
  "",
  "@C1@l@cInterpolation",
  "",
  "@bChoose the number of interpolating frames between two structures.",
  "@bThis functions generates a new movie, and resets the frame counter to zero.",
  NULL
};


static char *layout_help[] =
{
  "",
  "@C1@l@cLayout",
  "",
  "@bAll features in the layout dialog work interactively.",
  "@bYou can view the results of your changes in the movie canvas online.",
  "",
  "@C4@bBackbone",
  "Choose the way the backbone is displayed.",
  "@f@bSpline: Spline interpolation. Smooth curves, but expensive",
  "@f@b        beyound 500 bases.",
  "@f@bLinear: Straight lines. Simple, the dafault.",
  "@f@bStrips: Thicker lines, You may set a width.",
  "",
  "@C4@bNucleotides",
  "Choose the labeling of bases.",
  "@f@bLabeled:  Turn on labeling. The default.",
  "@f@bColored:  Color the bases according to the color options.",
  "@f@bDistance: Set the distance of the labels from the backbone.",
  "",
  "@C4@bBonds",
  "Base pair display options",
  "@f@bWC/GU: Display Wattson/Crick pairs as lines",
  "@f@b       and GU-wobble pairs as dots.",
  "@f@bLines: Display base pairs as lines.",
  "@f@bNone:  Don't display base pairs.",
  "",
  "@C4@bEnds",
  "Mark backbone ends",
  "@f@b5': Mark 5 Prime End with string \"5'\".",
  "@f@b3': Mark 3 Prime End with string \"3'\".",
  "",
  "@C4@bNumbering",
  "Choose the base numbering frequency and distance of labels to backbone.",
  NULL
};

static char *color_help[] =
{
  "",
  "@C1@l@cColors",
  "",
  "@bClick on the colored button of the feature color you want to change.",
  "@band choose the color from the color palette selector.",
  "(The lower button cancels the selection process.)",
  NULL
};

static char *font_help[] =
{
  "",
  "@C1@l@cFont Choice",
  "",
  "@bChoose the font type and size.",
  "(You may edit the test string.)",
  NULL
};

static char *references_help[] =
{
  "",
  "@C1@l@cReferences",
  "",
  "@C4@bRNA Movies",
  "Evers, D. and Giegerich, R. RNA Movies: Visualizing RNA Secondary Structure",
  "Spaces, Bioinformatics, 1999 15(1), 32-37.",
  "",
  "@C4@bNAVIEW",
  "Bruccoleri, R. E. and Heinrich, G., An improved algorithm for nucleic",
  "acid secondary structure display, CABIOS, 1988 4(1), 167-173.",
  NULL
};

static char *acknowledgement_help[] =
{
  "",
"@C1@l@cAcknowledgements",
"",
"The author would like to thank Gerhard Steger and his colleagues at the",
"Institute for Physical Biology, Heinrich-Heine University Düsseldorf for",
"many stimulating discussions on structure formation and Marc Rachen for his",
"data on PSTVd. Marc Rehmsmeier of the DKFZ-Heidelberg provided me with the",
  "minimal energy paths for L. collosoma.",
"",
"Part of the source code (saving in different formats) was written by",
"Alexander Sczyrba (now Rockefeller University New York).",
"",
"The work presented here (mainly my time) was partially supported by the",
"German Ministry for Education and Sciences (BMBF), the Ministry of Science",
"of North Rhine Westphalia (MWF-NRW) and the German Research Council",
"Graduate Programme on structure formation (DFG-GK \"Strukturbildungsprozesse\").",
  NULL
};

static char *copyright_help[] =
{
"",
"@C1@l@cCopyright",
"",
"@C4@bRNA Movies",
"@bwould not have been realized without the help of many people who",
"@bcontributed to free software development.",
"",
"",
"@C4@bLibraries",
"@-",
"",
"@bXFORMS Library",
"Copyright(c) by T.C. Zhao and Mark Overmars",
"@C1@fhttp://bragg.phys.uwm.edu/xforms",
"Press ctrl+(alt|meta)+middle-click for the xforms version number.",
"",
"@bMESA 3-D graphics library",
"Author: B. Paul",
"@C1@fhttp://www.mesa3d.org/",
"GNU LIBRARY GENERAL PUBLIC LICENSE",
"Version 2, June 1991",
"",
"",
"@bGD",
"A graphics library for fast GIF creation",
"@C1@fhttp://www.boutell.com/gd/",
"",
"     Portions copyright 1994, 1995, 1996, 1997, 1998, by Cold Spring",
"     Harbor Laboratory. Funded under Grant P41-RR02188 by the National",
"     Institutes of Health.",
"     ",
"     Portions copyright 1996, 1997, 1998, by Boutell.Com, Inc.",
"     ",
"     GIF decompression code copyright 1990, 1991, 1993, by David Koblas",
"     (koblas@netcom.com).",
"     ",
"     Non-LZW-based GIF compression code copyright 1998, by Hutchison",
"     Avenue Software Corporation (http://www.hasc.com/, info@hasc.com).",
"     ",
"     Permission has been granted to copy and distribute gd in any",
"     context, including a commercial application, provided that this",
"     notice is present in user-accessible supporting documentation.",
"     ",
"     This does not affect your ownership of the derived work itself, and",
"     the intent is to assure proper credit for the authors of gd, not to",
"     interfere with your productive use of gd. If you have questions,",
"     ask. \"Derived works\" includes all programs that utilize the",
"     library. Credit must be given in user-accessible documentation.",
"     ",
"     Permission to use, copy, modify, and distribute this software and",
"     its documentation for any purpose and without fee is hereby",
"     granted, provided that the above copyright notice appear in all",
"     copies and that both that copyright notice and this permission",
"     notice appear in supporting documentation. This software is",
"     provided \"as is\" without express or implied warranty.",
"     ",
"",
"@C4@bSource Code",
"@-",
"",
"@bNAVIEW",
"A program to make a modified radial drawing of an RNA secondary structure.",
"@C1@fftp://snark.wustl.edu/pub/naview.tar.Z",
"",
"     Copyright (c) 1988 Robert E. Bruccoleri Copying of this software, in whole",
"     or in part, is permitted provided that the copies are not made for",
"     commercial purposes, appropriate credit for the use of the software is",
"     given, this copyright notice appears, and notice is given that the copying",
"     is by permission of Robert E. Bruccoleri. Any other copying requires",
"     specific permission.",
"",
"@brendereps.c",
"Copyright (c) Mark J. Kilgard, 1997.",
"@C1@fhttp://reality.sgi.com/opengl/tips/Feedback.html",
"",
"     This program is freely distributable without licensing fees ",
"     and is provided without guarantee or warrantee expressed or ",
"     implied. This program is -not- in the public domain.",
  NULL
};

static char **help_arr[] =
{
  intro_help,
  navigation_help,
  speed_help,
  zoom_help,
  saving_help,
  snap_help,
  print_help,
  interp_help,
  layout_help,
  color_help,
  font_help,
  references_help,
  acknowledgement_help,
  copyright_help
};


# Dirty South Supercomputing Technical Reports

Imagining ourselves to be some kinds of academic or who knows what, DSSCAW
endeavors to produce quality writeups of various projects with which we are
involved. Papers are only released if permission is given by our clients, but
they are not allowed editorial control.

The Makefile generates one PDF per paper. I'm most recently using
[XƎTEX](https://tug.org/xetex/) 3.14159265 and
[Biber](http://biblatex-biber.sourceforge.net/) 2.12. Diagrams are usually [Graphviz](https://www.graphviz.org/) 2.40.1.
Most images have had some degree of processing through the [GIMP](https://www.gimp.org/) 2.x.

| № | Paper | Topic | Client | Release | DOI |
|---|-------|-------|--------|---------|-----|
|003|Heat Hacks: Reverse Engineering of minkaAire Remote Controls.|The remote controls for two [minkaAire Light Wave](https://www.minkagroup.net/f844-sl.html) fans are reverse-engineered, and replaced with a cheap Chinese transmitter.|[Personal](https://nick-black.com/dankwiki/index.php/Hack_on)|Unreleased|
|[002](http://dx.doi.org/10.13140/RG.2.2.20418.43202)|[µnandfs: A NAND Blobstore for Memory-Starved Platforms.](https://dsscaw.com/techreports/002-µnandfs.pdf)|A log-structured filesystem requiring only 2KB of RAM is implemented for the [Nordic Semiconductor nRF52840](https://www.nordicsemi.com/?sc_itemid=%7B2DC10BA5-A76E-40F8-836E-E2FC65803A71%7D) SoC and [Winbond W25N01GV](https://www.winbond.com/hq/product/code-storage-flash-memory/serial-nand-flash/index.html?__locale=en&partNo=W25N01GV) 1Gbit SLC NAND flash.|[Vakaros](https://www.vakaros.com/)|2019-06-04|
|001|Give, Sympathize, Control. Subverting the GZM 48S Lawnmower via CAN.|The electromechanical systems (particularly the CAN bus) of the [Greenworks GZM 48S](http://dev.greenworkscommercial.com/shop-by-tool/lithium-z-48-inch-stand-on-mower) commercial mower are explored, because robots gonna robomow.|[Greenzie](https://www.greenzie.co/)|Unreleased|

Copyright © 2019 Dirty South Supercomputers and Waffles (Atlanta, GA). All rights reserved.

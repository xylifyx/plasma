	Plasma by Jan Møller & Erik Hansen.

	It was compiled in Borland c++, in ANSI mode, using the HUGE memory 
	model.
	Feel free to use it as you desire, you may even name it after your
	grandmother. We don't care.
	   The reason why we made it? Well... First of all we wanted to make 
	some plasma, just for fun. But when we had finished it turned out to 
	be much faster than the plasma we have seen in intros, demos etc...
	Normally the color-cells are 2x4 4x4 or even larger, the 2x4 cell-
	plasma was awfully slow, bot ours ain't, even though it is 2x2 cells.
	   Well how can that be???                                 ^^^^^^^^^
	Our secret lies in the plasma-calculation!
	(I assume you have guessed that part already)
	We simply calculate as much as possible before we start showing the 
	goddies. The table 'Tab1' is a simple table (320x200 yields 64k) with 
	the distance from (x,y) to the center (rounded off to char by simple 
	overflow). The second table 'Tab2' is similar to 'Tab1', except we 
	molested it a bit with sine. In the mainloop we calculate a body 
	(160x100) by accessing the two tables with different pairs of (x,y) 
	and add them. (see for yourself in 'CalculateBody')
	And KaPoW. We have the fastest plasma...
	(i.e. the fastest we have ever seen.) If I am not correct then please 
	notify me. 
	On a 486 is bubbles around like melted chease in different colors.

	If you have any questions, comments or whips, then we would be happy 
	to answer.

	Contact us trough E-Mail:

	fwiffo@daimi.aau.dk

			or

	martino@daimi.aau.dk

	(If you can optimize it (e.g. write it in asm) we would be very 
	interested to see the results!)

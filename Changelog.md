# ChangeLog
# 0.0.1 - 2023-03-17 
>I didn’t know where to start, and then I found the skeletons for TCP UDP in the repres- tition and took them as a basis.
### Added
Initial program skeleton
# 0.1.0 - 2023-03-19 
>Realizing that this skeleton is not quite suitable for our task, so I corrected main : arguments, etc.
### Added
Main program functionality
# 1.0.0 - 2023-03-20 
>There were errors with working with binary code, when I did not take into account the zero bit at the beginning in the size of the encoded message.
Then for a long time I could not understand the work of CTR-C, but then I quickly realized.
And there was a very stupid mistake, because of which UDP did not work for me, because I mixed up "udp" and "upd".
And in the end I worked on Readme for a long time, since I wrote something like this for the first time, and was not familiar with the syntax, and also fiddled with the UML diagram, also because of the unfamiliar syntax
### Added
* UML diagram with cosmetic updates
* Ctrl-C function for program termination
* README file with documentation instructions
### Fixed
* Bugs related to UDP and UPD protocols
* Structure of UML diagram
# 1.0.1 - 2023-03-20 
>Created a Makefile with "missing separator" error
### Added
Makefile
### Fixed
Missing separator error
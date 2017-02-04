#CIS*3760 Software Systems Design & Integration Assignment 4

Description
-----------

The Assignment throughout CIS*3760 was to build an application that converts plain text to html format, following a specific set of 
requirements. The Initial assignment was to build a C program that does simple formatting to text, replacing newline charaters, 
tabs, CRNLs, and other whitespace characters with appropriate HTML tags. The second assignment was to send the partially formated 
text to a python program using pipes. Note, embedding the python code within C was strictly forbidden. The python script was to 
add HTML bold, italics, and underline tags to substrings as defined in a .info file, as well as adding all the required HTML 
standard segments such as header and body. The third assignment was to build a HTML/PHP based webpage with 2 buttons, the first 
allowing users to upload text files and .info files to the webserver, and the second to call the existing C program to convert 
the text to HTML. The final assignment was to implement an additional button to call the C program to send converted files to a 
seperate MYSQL database server. All files stored on the webserver and database server had to be listed on the main webpage as 
clickable hyperlinks that would take the user to view the files contents. For the final assignment we were also required to build 
an additional C file that had some basic MYSQL commands built in. Note, we were strictly forbidden from accessing the database 
using PHP libraries and they were uninstalled from the webserver we were using. 


Original Readme.txt contents:
-----------------------------

make:

	builds the a4 files
	
make db:

	builds the db program
	
make python:

	sets executalbe privledges for python
	
	
Problems:

	Cannot load a text file larger than 64KB into the database
	
	With A3 the files downloaded from courselink would not properly convert to HTML, however if copied
	
	the copies would properly convert Still waiting on re-grade to know significance. 
	

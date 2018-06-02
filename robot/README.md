# Robot

This project is about implementing [TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol) Server and Client for a simple robot game.

## Task Description

### Situation

Robot moves across city. The city consists of 2-D panels with (x,y) coordinates. There are no physical obstacles in the city. The robot can go forth, turn to the left or pick up flag. It is also capable of informing about its city position.

The robot can move just across the city panels, if it gets out of the city it crashes immediately. The city dimensions are 37 x 37. Both these coordinates (x and y) must be within <-36,36> range.

There is a flag at the (0,0) position. The robot's mission is to get at the flag's position, pick it up and read the secret text.

### General scheme of communication

The server represents the robot, the robot is to be controlled by client. We take advantage of the TCP protocol. The server listens on the chosen port, the port must be within <3000, 3999>. The protocol is text-oriented and can be tested using command telnet server_address 3998 (in this case using the port 3998).

After the client connects to the server, the robot's coordinates and orientation are generated randomly by the server. The robot's coordinates must be within <-17,17> interval for both x and y. The client must figure out the coordinates of the robot. The client cannot know the initial robot's coordinates or its orientation, so it must move the robot to get this kind of information (e.g. turn the robot, do 1 step and then compare the data). The client must navigate the robot at the flag's place. Then the flag may be picked up. Once it is done the server reveals the secret text and closes the connection. If the flag is not there, the server reports error and also closes the connection (thus the client has only one change to pick the flag up).

The server must send the identification text (ended with \r\n letters – see the code 210 in answer list) immediately after the connection is established. Then it just awaits client's commands and sends the relevant answers.

### Data structure

#### Commands

It is the client who sends the commands.
Structure: robot_name + space character + command ended by \r\n
Example: Magdalena STEP\r\n

#### Answers

The server must answer to every single command. Structure: code <space> answer ended with \r\n
where code is some number (in text representation). Possible answers:
answer	explanation
210 Robot's greeting 	Initial identification sent by the server immediately after the connection is estabilished.
240 OK (x,y) 	The last operation was succesfull. x a y are the new robot coordinates.
260 SUCCESS secret text 	Successful flag lift. At the same time the secret text (character string) must be revealed.
500 UNKNOWN COMMAND 	Command was not recognized. No action is to be done.
530 CRASHED 	The robot got out of the city and probably crashed.
550 CANNOT PICK UP THE SIGN 	There is no flag at this place.
571 THIS BLOCK IS OK 	Attempt to repair a block that is OK.
572 ROBOT FELLS TO PIECES 	The robot's block was broken but you tried to move it anyway.
580 FAILURE OF BLOCK x 	Robot block's number x is out of control (number within the <1, 9> range, text representation).

The server must close the connection after it sends away an answer with code 260,530,550,571 or 572. On the other hand, answer with code 500 or 580 means that connection stays active.

You must find the robot's name from its initial greeting (answer 210) so that you may address commands to it. The name can be anywhere in the greeting text. It can be found as a substring with following format: “My name is ROBOT_NAME.”. The robot's name is a string sequence of an arbitrary characters except \r, \n, \0 and full stop. The space character must occur neither at the beginning nor at the end of the name. However the it may occur in the middle of the name. The name can be from 1 byte up to 512 bytes long. The name is case-sensitive. There may be a problem in case of UTF coding if the character's second byte contains \r, \n, space char or full stop.

The robot may not carry out the STEP command because of some block failure (answer 580). In this case we need to repair the block (REPAIR command) and then send the original command once again. We may fix only the block that is being broken, othwewise the server will close the connection (answer 571). If the STEP command is requested but the robot's block is broken, the connection will be closed as well (answer 572). At the beginning, all robot's blocks are in good state. However the robot does not stand much moving, there will definitely be some block failure after 10 sequential STEP commands. Thus it might be much sooner.

#### Command list
##### STEP command

Syntax: robot_name STEP\r\n
Meaning: The robot will step ahead in its current direction. One of coordinates (x or y) will be incremented or decremented.
The robot_name must be gained out of answer 210.
The robot must not be broken, otherwise it will fall apart (answer 572). If the robot is in the good shape, it may get broken (answer 580).
Possible answers: 240,530,580,572

##### LEFT command

Syntaxe: robot_name LEFT\r\n
Meaning: Changes the robot's direction. The robot will get turned to the left by 90 degrees.
The robot_name must be gained out of answer 210.
Possible answers: 240

##### PICK UP command

Syntaxe: robot_name PICK UP\r\n
Meaning: The robot will pick the flag up. If the robot is not at the right coordinates (0,0) the robot will fall apart. The server will report it and will close the connection. The robot_name must be gained out of answer 210.
Possible answers: 260,550

##### REPAIR command

Syntax: robot_name REPAIR n\r\n where n is the number of failed block (n ranges from 1 to 9).
Meaning: The command will fix the robot's block. We may repair only a block that was marked as broken (answer 580). If there is some broken block, we cannot proceed with any STEP commands. Ostatní příkazy jsou vykonány.

The robot_name must be gained out of answer 210.
Possible answers: 240,571

These are all commands that can be recognized by the robot. The robot will answer the code 500 in case of any unknown command structure.

### Example of communication

italics = the text that gets displayed in terminal after connection launching or closing
bold = server (robot)
normal = client

	xsmitka@fray1:/home/zam/xsmitka>telnet baryk.fit.cvut.cz 3998
	Trying 147.32.232.173…
	Connected to baryk.fit.cvut.cz.
	Escape character is '^]'.
	210 Hello, here is Robot version 1.6. My name is Magdalena.
	Magdalena LEFT
	240 OK (13,7)
	Magdalena STEP
	240 OK (12,7)
	Magdalena STEP
	580 FAILURE OF BLOCK 8
	Magdalena REPAIR 8
	240 OK (12,7)
	Magdalena STEP
	580 FAILURE OF BLOCK 3
	Magdalena REPAIR 3
	240 OK (12,7)
	Magdalena STEP
	240 OK (11,7)
	Magdalena STEP
	580 FAILURE OF BLOCK 3
	Magdalena REPAIR 3
	240 OK (11,7)
	Magdalena STEP
	240 OK (10,7)
	Magdalena STEP
	580 FAILURE OF BLOCK 9
	Magdalena REPAIR 9
	240 OK (10,7)
	Magdalena STEP
	240 OK (9,7)
	Magdalena STEP
	240 OK (8,7)
	Magdalena STEP
	240 OK (7,7)
	Magdalena STEP
	240 OK (6,7)
	Magdalena STEP
	240 OK (5,7)
	Magdalena STEP
	240 OK (4,7)
	Magdalena STEP
	240 OK (3,7)
	Magdalena STEP
	240 OK (2,7)
	Magdalena STEP
	240 OK (1,7)
	Magdalena STEP
	240 OK (0,7)
	Magdalena LEFT
	240 OK (0,7)
	Magdalena STEP
	240 OK (0,6)
	Magdalena STEP
	240 OK (0,5)
	Magdalena STEP
	240 OK (0,4)
	Magdalena STEP
	240 OK (0,3)
	Magdalena STEP
	580 FAILURE OF BLOCK 7
	Magdalena REPAIR 7
	240 OK (0,3)
	Magdalena STEP
	240 OK (0,2)
	Magdalena STEP
	240 OK (0,1)
	Magdalena STEP
	240 OK (0,0)
	Magdalena PICK UP
	260 SUCCESS Hello world.
	Connection to baryk.fit.cvut.cz closed by foreign host.
	xsmitka@fray1:/home/zam/xsmitka>

### Requirements

* both client-side and server-side part must be implemented
* client must pick the secret text up automatically with no human interference (this applies for the whole session)
* both client-side and server-side part must operate with its adverse party on baryk.fit.cvut.cz
* client must be able to estabilish more connections at one time as well as multiple sequential connections (without server restart)
* both server and client must be stable and resistant to unexpected input
* program should echo some basic information about its state (e.g. messages that are being processed, …)
* IP address or DNS name and port number are to be passed as parameters upon program invocation (thus not hard-coded), the right syntax can be seen below)
* client must print the secret text in case the flag gets picked up successfully
* server must print information about currently served client (so a recipient is known)
* the source code must be well-commented, author's name must be mentioned in the source code's upper part
* among permitted programming languages are: Java, C, C++, Python, Perl
* both client and server implementation must be in the one single file. This file may be named in whatever manner, but its extension must be .c, .cpp or .java.

The file must be compilable by the relevant command-line oriented compiler (gcc/g++/javac). The file is the only one to be submitted.

if you use java language, add package robot at the beginning of your source file and name the class with main method Robot.

There is java, C or C++ templates that you can use – see examples.

program can be invoked as server or as client according to passed parameters:

client role: ./robot <server> <port>
server role: ./robot <port>
where <server> is the name or server's IP address and <port> is the port number, which the server listens on.



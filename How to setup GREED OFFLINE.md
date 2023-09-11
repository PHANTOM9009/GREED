# Steps to setup GREED OFFLINE in your computer:

## 1. Setup Visual Studio:
Download Visual Studio from its offical website. Download C++ tools.

## 2. Create a New Empty Project:
Select a new Empty C++ console project.

## 3. Setup GREED libraries:
Copy two files from the downloaded GREED_OFFLINE: *greed_offline.hpp* and *greed_offline.lib* and paste in the main folder of your visual studio project.

Now you need to create a new c++ file in the project, then go to *Project > (Project name) Properties > Linker > Input.
In *additional dependencies* paste the name of the library *greed_offline.lib* click on ok, and return to the C++ file.

## 4. Writing your first algorithm:
Now when you have set the project, we are ready to write your first GREED algorithm. You can write your own or get a sample algorithm from this repository.
To write your own, Copy the template of the program available in the downloaded folder. To know how to write your algorithm check [Quick Start](quick_start.md)

## 5. One more thing:
In order to see your agents perfor in front of you, you need to copy *display package* from the downloaded folder and paste in the folder where your algorithm file is present.

## 6. How to start the lobby server:
Till now we have seen how to setup the place where you will code your algorithms, now lets see how to start GREED OFFLINE.
You will find the servers in *Server Package* folder of the downloaded folder.
Start the lobby_server.exe (Lobby_server is the server which connects with many game_servers and then tell the incoming client which game server it has connect to)
Enter the number of players that will play in one game server (in community version maximum number of players in a game server is 4)
your lobby server should be listening for the game servers now.

## 7. How to start the game server:
game server is the main server which will really run your game.
Start game_server.exe, enter the same number of players you entered in the lobby_server.
Now your servers are ready.
Make sure the lobby_server.exe and game_server.exe should run in the same machine.

## 8. Run your algorithm file:
If you have followed every step correctly when you run your algorithm file in visual studio, you must see a terminal asking for the ip address of the servers.
if you are running server and client from the same machine enter: *127.0.0.1* if not then input the ip address of the lobby_server's machine.
then the client will ask do you want to start with the display unit or not.
if you select the option to start without the display unit, then only your algorithm will run but you will not be able to see the game happening. this feature is made if you want to 
run many clients in a single machine, in that you will only need one display unit, so when you start the last client choose the option to start with display unit.

After you have started all the client units you should see the game running.
If you have any issues with this process please report to us at our [Discord](https://discord.gg/2CBeaMAAay).


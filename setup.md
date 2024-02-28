# Steps to setup GREED in your computer:
# Download the pre-compiled binaries from the website.

You can download the game from [AI_PLAYGROUNDS](https://aiplaygrounds.in)
## 1. Setup Visual Studio:
Download Visual Studio from its offical website. Download C++ tools.

## 2. Create a New Empty Project:
Select a new Empty C++ console project.

## 3. Setup GREED libraries:
Copy two files from the downloaded GREED_OFFLINE: *greed_offline.hpp* found in Include directory and *greed_offline.lib* found in lib directory and paste in the main folder of your visual studio project.

Now you need to create a new c++ file in the project, then go to *Project > (Project name) Properties > Linker > Input.
In *additional dependencies* paste the name of the library *greed_offline.lib* click on ok, and return to the C++ file.

## 4. Writing your first algorithm:
Now when you have set the project, we are ready to write your first GREED algorithm. You can write your own or get a sample algorithm from this repository.
To write your own, Copy the template of the program available in the downloaded folder. To know how to write your algorithm check [Quick Start](quick_start.md)

## 5. One more thing:
In order to see your agents perfor in front of you, you need to copy *display package* from the downloaded folder and paste in the folder where your algorithm file is present.



## 8. Run your algorithm file:
If you have followed every step correctly when you run your algorithm file in visual studio,you must be seeing a message asking to open the game in which mode?
if you select the option to start without the display unit, then only your algorithm will run but you will not be able to see the game happening. this feature is made if you want to 
run many clients in a single machine, in that you will only need one display unit, so when you start the last client choose the option to start with display unit.

After you have started all the client units you should see the game running.
If you have any issues with this process please report to us at our [Discord](https://discord.gg/2CBeaMAAay).


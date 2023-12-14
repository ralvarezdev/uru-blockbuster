# Blockbuster
This repository contains the source code of the Data Management Program for a Fictional Movie Rental Company, developed by:

## Developers
Ramón Álvarez <a href="https://github.com/ralvarezdev">(ralvarezdev)</a>.

Roberto Belmonte <a href="https://github.com/rb6422">(rb6422)</a>.

## Programming Languages
Only C++.

## Installation

<ol>
<li>Clone this repository to your local machine using the following command: 

```git clone https://github.com/ralvarezdev/ProyectoTeoria_RR```
</li>

<li>Change your Current Working Directory to where the Repository was Cloned</li>

<li>There are Two Ways to Compile the Program: </li>

<ol>
<li>With <strong>G++</strong> (You must have Installed <strong>MinGW Compiler</strong>). Use the following command:

<details>
<summary>Command for Windows</summary>

```g++ -o bin\Blockbuster.exe src\main.cpp src\lib\namespaces.h src\lib\clients\clientsOp.h src\lib\clients\clientsOp.cpp src\lib\data\dataOp.h src\lib\data\dataOp.cpp src\lib\datatables\output.h src\lib\datatables\output.cpp src\lib\movies\moviesOp.h src\lib\moviesOp.cpp src\lib\terminal\ansiEsc.h src\lib\terminal\ansiEsc.cpp src\lib\terminal\input.cpp src\lib\terminal\input.h```

</details>

<details>
<summary>Command for Linux</summary>

```g++ -o bin/Blockbuster.exe src/main.cpp src/lib/namespaces.h src/lib/clients/clientsOp.h src/lib/clients/clientsOp.cpp src/lib/data/dataOp.h src/lib/data/dataOp.cpp src/lib/datatables/output.h src/lib/datatables/output.cpp src/lib/movies/moviesOp.h src/lib/moviesOp.cpp src/lib/terminal/ansiEsc.h src/lib/terminal/ansiEsc.cpp src/lib/terminal/input.cpp src/lib/terminal/input.h```

</details></li>

<li>With <strong>CMake</strong> (You must have Installed <strong>CMake and Make</strong>). First, Get to the <code>bin</code> Folder inside the Repository. Use the following command

<details>
<summary>Command for Windows</summary>

```cmake -S ..\ -G "MinGW Makefiles" -B .\ && make```

</details>

<details>
<summary>Command for Linux</summary>

```cmake -S ../ -G "MinGW Makefiles" -B ./ && make```

</details></li></ol>

<li>Run the program by clicking the <code>Blockbuster.exe</code>.</li></ol>

<strong>For a Better User Experience, Resize the Terminal Window to Fullscreen</strong>

# Main Menu
![Screenshot (942)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/c6752013-1ccf-4bbd-9f6a-8292a630d209)

# Commands
This is the Full List of Commands for Each Action in the Program

# Parameters

![Screenshot (910)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/d54ff36b-f7db-4977-abef-120e37a2ea70)

![Screenshot (911)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/8fbf9b9c-f8ae-4be5-8da8-fa682cbbf92a)

![Screenshot (912)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/a8070b60-02d7-4e09-988d-e5b54e1107f7)

![Screenshot (913)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/ba759acc-8707-46b7-8d7d-25ea724913c9)

# View Movies Command Examples
![Screenshot (905)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/ffdd1479-94dc-4fd3-b8fd-6802297285a7)

# Filter Movies Command Examples
![Screenshot (906)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/942685c6-b9dd-4413-918b-9ae6a854439f)

# View Clients Command Examples
![Screenshot (907)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/b8018392-28f1-405c-a670-6a4329a07d76)

# Search Clients Command Examples
![Screenshot (908)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/b053d443-5842-44ba-9a88-7fd79a916174)
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
![Screenshot (769)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/d1355dc3-ec22-4cbd-bcc3-eed77c21761e)

# Commands
This is the Full List of Commands for Each Action in the Program

# Parameters
![Screenshot (770)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/cc3a5ca7-3482-49d3-b71c-0fc9e8cfdd59)

![Screenshot (771)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/6a4676f8-b499-46bf-9991-9a45ecbcefa3)

![Screenshot (772)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/20e76420-4aaf-4cd6-a98b-8532a220bf3d)

![Screenshot (773)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/dc776d15-ec4d-42c4-bf90-43ea0593f9b6)

# View Movies Command Examples
![Screenshot (774)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/fa724ec9-876f-46cf-bbf9-7942c32ad744)

# Filter Movies Command Examples
![Screenshot (775)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/f28ffbb4-9ad6-4978-a49a-c687cac72e09)

# Filter Clients Command Examples
![Screenshot (776)](https://github.com/ralvarezdev/ProyectoTeoria_RR/assets/86166683/3a1e2aa0-ef9d-4732-8432-fa1a461a6267)